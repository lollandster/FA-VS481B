//Psudo code:
//Button for PC1 shall send: serial.println("v")
//Button for PC2 shall send: serial.println("w")
//Button for Switch ON shall send: serial.println("t")
//Button for Switch OFF shall send: serial.println("u")
//for status update send: serial.println("s")
//
//Incomming data is status information only
//when incomming data is detected then LEDs must be changed
//LEDs shall not change when pressing buttons, only when receiving status
//When switch reports off then only the RED button shall be lit.
//RED LED may be turned off after a short while if the light is too bright.

//Lets define some constants to bake changes easier in the future
const int GrnPin = 11;
const int GrnLED = 12;
const int RedPin = 9;
const int RedLED = 10;
const int PC1Pin = 5;
const int PC1LED = 6;
const int PC2Pin = 7;
const int PC2LED = 8;
const int ReadyToSend = 2;
long counter0 = 0; //used for adding delay after button press (kind of debounce)
long counter1 = 0;
long counter2 = 0;
long counter3 = 0;
int incommingByte = 0; //incomming communication is stored here

//Configure switches, LEDs and Serial
void setup()
{
  //Setting up inputs for buttons
  pinMode(GrnPin, INPUT); //Green button
  pinMode(RedPin, INPUT); //Red button
  pinMode(PC1Pin, INPUT); //PC2 button
  pinMode(PC2Pin, INPUT); //PC1 button
  //Setting the inputs high to activate internal Pull-Up resistors
  digitalWrite(GrnPin, HIGH);
  digitalWrite(RedPin, HIGH);
  digitalWrite(PC1Pin, HIGH);
  digitalWrite(PC2Pin, HIGH);

  //Setting up outputs for LEDs
  pinMode(GrnLED, OUTPUT); //Green button LED
  pinMode(RedLED, OUTPUT); //Red button LED
  pinMode(PC1LED, OUTPUT); //PC2 button LED
  pinMode(PC2LED, OUTPUT); //PC1 button LED

  //Turn on all LEDs to verify that they all work.
  digitalWrite(12, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(6, HIGH);

  //setting up serial
  pinMode(2, OUTPUT); //this is the ReadyToSend pin on the RS485 board
  Serial.begin(4800); //I'm using a fairly slow bitrate as I only send one byte back and forth
  delay(500);
  digitalWrite(ReadyToSend, HIGH); //sets RTS high
  Serial.println(115); //request status from master
  digitalWrite(ReadyToSend, LOW); //sets RTS high

  //Turn off LEDs. this may be changed so status update turns off LEDs, indicating connection is OK
  delay(500);
  digitalWrite(12, LOW);
  digitalWrite(10, LOW);
  digitalWrite(8, LOW);
  digitalWrite(6, LOW);
}

void loop()
{
  if (Serial.available())
  {
    incommingByte = Serial.read();
    if ((incommingByte > 34) && (incommingByte < 39)) //if outside then the byte is invalid
    {
      incommingByte -= 33;
      if (((incommingByte >> 2)  & 0x01) == 1) //bit for sw01
      {
        digitalWrite(PC1LED, HIGH);
        digitalWrite(PC2LED, LOW);
      }
      else
      {
        digitalWrite(PC1LED, LOW);
        digitalWrite(PC2LED, HIGH);
      }

      if (((incommingByte >> 0)  & 0x01) == 1) //bit for on
      {
        digitalWrite(GrnLED, HIGH);
        digitalWrite(RedLED, LOW);
      }
      else
      {
        digitalWrite(GrnLED, LOW);
        digitalWrite(RedLED, HIGH);
        digitalWrite(PC1LED, LOW);
        digitalWrite(PC2LED, LOW);
      }
    }
  incommingByte = 0;
  }


  if (counter0 > 0) {counter0 --;} //count down
  if (counter1 > 0) {counter1 --;} //count down
  if (counter2 > 0) {counter2 --;} //count down
  if (counter3 > 0) {counter3 --;} //count down

  if (digitalRead(GrnPin) == LOW)
  {
    if (counter0 < 1)
    {
      digitalWrite(ReadyToSend, HIGH); //turn on RS485 send pin
      Serial.write(116); //116 = t
      delay(100);
      digitalWrite(ReadyToSend, LOW); //turn off RS485 send pin
      counter0 = 100000;
      counter1 = 0;
      counter2 = 0;
      counter3 = 0;
    }
  }

  if (digitalRead(RedPin) == LOW)
  {
    if (counter1 < 1)
    {
      digitalWrite(ReadyToSend, HIGH); //turn on RS485 send pin
      Serial.write(117); //116 = u
      delay(100);
      digitalWrite(ReadyToSend, LOW); //turn off RS485 send pin
      counter1 = 100000;
      counter0 = 0;
      counter2 = 0;
      counter3 = 0;
    }
  }

  if (digitalRead(PC1Pin) == LOW)
  {
    if (counter2 < 1)
    {
      digitalWrite(ReadyToSend, HIGH); //turn on RS485 send pin
      Serial.write(118); //118 = v
      delay(100);
      digitalWrite(ReadyToSend, LOW); //turn off RS485 send pin
      counter2 = 100000;
      counter1 = 0;
      counter0 = 0;
      counter3 = 0;
    }
  }

  if (digitalRead(PC2Pin) == LOW)
  {
    if (counter3 < 1)
    {
      digitalWrite(ReadyToSend, HIGH); //turn on RS485 send pin
      Serial.write(119); //118 = w
      delay(100);
      digitalWrite(ReadyToSend, LOW); //turn off RS485 send pin
      counter3 = 100000;
      counter1 = 0;
      counter2 = 0;
      counter0 = 0;
    }
  }
}
