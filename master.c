//psudo code:
//Master skal fungere som ledd mellom RS485 og RS232
//RS485 brukes for å kommunisere med slavene.
//RS232 brukes for å kommunisere med VS481B
//Det forventes ikke kommunikasjon fra VS481B hvis ikke kommando er sendt først
//slavene kan komme til å sende meldinger uoppfordret og bør derfor konstant
//overvåkes.
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
boolean stringComplete = false; // whether the string is complete
bool powerOn = false;
bool sw01 = true;
bool sw02 = false;
int RS485_Send_pin = 9;
bool AskForStatus = false; //Set true to make loop send "read" through RS232

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
    digitalWrite(13, HIGH); //light the LED if data is detected
    delay(100);
    if (inputString == "Command OK") {/*Do nothing*/;}
    else if (inputString == "Command incorrect") {/*Do nothing*/;}
    else if (inputString == "Power On") {powerOn = true;}
    else if (inputString == "Power Off") {powerOn = false;}
    else if (inputString == "sw i01") {
      sw01 = true;
      sw02 = false;
    }
    else if (inputString == "sw i02") {
      sw02 = true;
      sw01 = false;
    }
    else if (inputString == " F/W:V1.0.062\n\r") {
      //last line of configuration is sendt, time to send info through RS485
      statusByte = 33 + powerOn + (sw01 * 2) + (sw02 * 4);
      digitalWrite(RS485_Send_pin, HIGH); //turn on RS485 send pin
      mySerial.write(statusByte);
      digitalWrite(RS485_Send_pin, LOW); //turn off RS485 send pin
    }
    else
    {
      //Deconstruct config information:

    }
    digitalWrite(13, LOW); //Turn of LED
  }
  if (AskForStatus == true)
  {
    AskForStatus = false;
    Serial.println("read"); //the command 'read' asks VS481B to return config
  }
}

void GetStatus()
{
  //ask VS481B for status
  Serial.println("read"); //the command 'read' asks VS481B to return config
  //Wait for reply
  //return status
  statusByte = 33 + powerOn + (sw01 * 2) + (sw02 * 4);
  digitalWrite(RS485_Send_pin, HIGH); //turn on RS485 send pin
  mySerial.write(statusByte);
  digitalWrite(RS485_Send_pin, LOW); //turn off RS485 send pin
}

void serialEvent()
{
  while (Serial.available())
  {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    if (inChar != '\n')
    {
      inputString += inChar;
    }
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n')
    {
      stringComplete = true;
    }
  }
}
