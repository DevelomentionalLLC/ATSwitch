
float ppv = 0;
float batv = 0;
const int ledPin =  LED_BUILTIN;

int ppvSensePiin = A1;
int batterySensePin = A2;
int shuntSensePin = A3;
int inverterLowCapacityPin =- 23;
int inverterPin =- 24;
int ppvConnect = 25;
int battConnect = 26;
int gridConnectPin = 27;
bool batteryLow = true;
bool gridOff = true;
bool ppvNominal = true;

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete



void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
 pinMode(ledPin, OUTPUT);

 pinMode(ppvConnect, OUTPUT);
 pinMode(battConnect, OUTPUT);
 pinMode(gridConnectPin, OUTPUT);
 pinMode(inverterPin, OUTPUT);
 pinMode(inverterPin, OUTPUT);

 pinMode(ppvSensePiin, INPUT);
 pinMode(batterySensePin, INPUT);
 pinMode(shuntSensePin, INPUT);



}

void loop() {
  // put your main code here, to run repeatedly:
if (stringComplete) {
    Serial.println(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
batv = senseBattVoltage();
ppv = sensePPVVoltage();


}
// senses ppv voltage
float sensePPVVoltage()
{
 int sensorValue = analogRead(ppvSensePiin);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);

  // print out the value you read:
if (ppv >= 14.4) {
ppvNominal = true;
}else {
ppvNominal = false;
}

  Serial.println(voltage);
  return voltage;;
}
/*
Super bad shit to avoid at all cost:
grid on + inverter on
grid to battery



*/
//sense current through shunt resistor
float senseCurrent()
{
  int shuntValue = 1.2;//1.2 Ohm 1 mv = 1 ampre

 int sensorValue = analogRead(shuntSensePin);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);
  // print out the value you read:
  Serial.println(voltage);

return voltage/shuntValue;
}

// senses batteries voltage
float senseBattVoltage()
{
   int sensorValue = analogRead(batterySensePin);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);
  // print out the value you read:
  Serial.println("battery voltage::");
   Serial.println(voltage);
   return voltage;
}
// connects ppv to batteries
void chargeBatteryConnect ()
{
  
  digitalWrite(battConnect, LOW);
digitalWrite(ppvConnect, LOW);


}
// disconnect ppv from  batteries
void chargeBatteryDisconnect()
{
 digitalWrite(battConnect, HIGH);
digitalWrite(ppvConnect, HIGH);


}
//connects ac side of inverter to full load.
void gridConnect()
{

digitalWrite(gridConnectPin , LOW);

}
//disconnects ac side of inverter from full load.
void gridDisconnect()
{
 
digitalWrite(gridConnectPin , HIGH);

  
}
//turns on dc power to inverter
void inverterOn()
{

digitalWrite(inverterPin , LOW);

}
//turns off the dc power to inverter.
void inverterOff()
{

digitalWrite(inverterPin , HIGH);

}
// puts inverter ac load on but with a small load.
void inverterLowCompacity()
{
 digitalWrite(inverterLowCapacityPin , LOW);
}
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    
    // add it to the inputString:
    inputString += inChar;
    
    Serial.println(inputString);
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}