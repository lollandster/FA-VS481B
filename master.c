//Made for Arduino nano by John Arild Lolland

//psudo code:
//Master skal fungere som ledd mellom RS485 og RS232
//RS485 brukes for å kommunisere med slavene.
//RS232 brukes for å kommunisere med VS481B
//Det forventes ikke kommunikasjon fra VS481B hvis ikke kommando er sendt først
//To seriell porter er nødvendig for master. Software serial må brukes.
//hardware seriell brukes for RS232 og sofrware seriell for RS485.

//Slave kommandoer:
//s = status request
//t = power on
//u = power off
//v = sw01
//w = sw02

#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX
int statusByte = 33;
int incommingByte = 0;
String inputString = ""; // a string to hold incoming data from VS481B
String debugString = "";
boolean stringComplete = false; // whether the string is complete
bool powerOn = false;
bool sw01 = true;
bool sw02 = false;
int RS485_Send_pin = 9;
bool AskForStatus = false; //Set true to make loop send "read" through RS232
int counter = 0;

void setup() {
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

  // Open hardware serial communications:
  Serial.begin(19200);

  // set the data rate for the SoftwareSerial port
  mySerial.begin(4800);
  pinMode(RS485_Send_pin, OUTPUT);

  //Set pin13 (LED on Arduino board) as output for debug
    pinMode(13, OUTPUT);
}

void loop() { // run over and over
  if (mySerial.available()) {
    incommingByte = mySerial.read();
  }
  if (incommingByte == 115) //ASCII s
  {
    //ask VS481B for status
    AskForStatus = true;
  }
  else if (incommingByte == 116) //ASCII t
  {
    //ask VS481B to turn on ouputs
    Serial.println("sw on");
    //ask VS481B for status
    AskForStatus = true;
  }
  else if (incommingByte == 117) //ASCII u
  {
    //ask VS481B to turn off outputs
    Serial.println("sw off");
    //ask VS481B for status
    AskForStatus = true;
  }
  else if (incommingByte == 118) //ASCII v
  {
    //ask VS481B to change input to 01
    Serial.println("sw i01");
    //ask VS481B for status
    AskForStatus = true;
  }
  else if (incommingByte == 119) //ASCII w
  {
    //ask VS481B to change input to 02
    Serial.println("sw i02");
    //ask VS481B for status
    AskForStatus = true;
  }
  incommingByte = 0;

  //-------------------------------------------------

  if (stringComplete)  //data recieved from VS481B
  {
    //Used under development:
    //digitalWrite(13, HIGH); //onboard LED
    //digitalWrite(RS485_Send_pin, HIGH); //turn on RS485 send pin
    //mySerial.println(inputString);
    //mySerial.println("sw01: " + String(sw01));
    //mySerial.println("sw02: " + String(sw02));
    //mySerial.println("powerOn: " + String(powerOn));
    //mySerial.println("debugString:" + debugString + ":");
    //mySerial.println("statusByte:" + String(statusByte) + ":");
    //digitalWrite(RS485_Send_pin, LOW); //turn off RS485 send pin

    statusByte = 33 + powerOn + (sw01 * 2) + (sw02 * 4); //makes statusByte to be sendt to slave
    digitalWrite(RS485_Send_pin, HIGH); //turn on RS485 send pin
    mySerial.write(statusByte);
    digitalWrite(RS485_Send_pin, LOW); //turn off RS485 send pin
    inputString = "";
    stringComplete = false;
  }

  if (AskForStatus == true)
  {
    counter ++;
    if(counter > 10000)
    {
      AskForStatus = false;
      counter = 0;
      Serial.println("read"); //the command 'read' asks VS481B to return config
    }
  }
}

void serialEvent()
{
  while (Serial.available())
  {
    // get the new byte:
    char inChar = (char)Serial.read();
    inputString += inChar;

    //Search for Input port number. This is part of the "read" response
    if (inputString.substring(inputString.length() - 12,inputString.length() - 1) == "Input:port ")
    {
      debugString = inputString.substring(inputString.length() - 1);
      if(inputString.substring(inputString.length() - 1) == "2")
      {
        sw02 = true;
        sw01 = false;
      }
      if(inputString.substring(inputString.length() - 1) == "1")
      {
        sw01 = true;
        sw02 = false;
      }
    }

    //Search for HDMI output on/off. This is part of the "read" response
    if (inputString.substring(inputString.length() - 9,inputString.length() - 2) == "Output:")
    {
      debugString = inputString.substring(inputString.length() - 2);
      if(inputString.substring(inputString.length() - 2) == "ON")
      {
        powerOn = true;
      }
      if(inputString.substring(inputString.length() - 2) == "OF") //This is "OFF", but I only check 2 characters
      {
        powerOn = false;
      }
    }

    //When F/W is recieved, then no more usefull information will come.
    //Set stringComplete to true indicating that data can be sendt to slaves
    //This also clears inputString to clear up memory.
    if (inputString.substring(inputString.length() - 3) == "F/W")
    {
        stringComplete = true;
        //inputString = "";
    }
    //if for any reason the inputString gets too long, then delete it
    //This 'if' should never be 'true'
    if (inputString.length() > 1000) {inputString = "";}
  }
}
