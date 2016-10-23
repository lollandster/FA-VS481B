//Psudo code:
//Switch PC1: serial.println("sw i01")
//Switch PC1: serial.println("sw i02")
//Switch ON: serial.println("sw on")
//Switch OFF: serial.println("sw off")
//status update: serial.println("read")

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
int sw00;
boolean powerOn;

void setup()
{
  // initialize serial:
  Serial.begin(19200);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  sw00 = 0;
  powerOn = false;
}

void loop()
{
  if (stringComplete) { //data har kommet
    if (inputString == "sw i01") {
      Serial.println("Command OK");
      sw00 = 1;
    }
    else if (inputString == "sw i02") {
      Serial.println("Command OK");
      sw00 = 2;
    }
    else if (inputString == "sw on") {
      Serial.println("Command OK");
      powerOn = true;
    }
    else if (inputString == "sw off") {
      Serial.println("Command OK");
    }
    else if (inputString == "read") {
      if(powerOn == true){
        Serial.println("Power On");
      }
      else if(powerOn == false){
        Serial.println("Power Off");
      }
      Serial.println("sw i0" + sw00);
      Serial.println("some useless information");
      Serial.println("more useless information");
      Serial.println("end of config");
    }
    else {Serial.println("Command incorrect");
    powerOn = false;
  }
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    if (inChar != '\n') { inputString += inChar; }
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
