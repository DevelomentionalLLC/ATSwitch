
float ppv = 0;
float batv = 0;
const int ledPin =  LED_BUILTIN;

int ppvSensePiin = DDA1;
int batterySensePin = DDA2;
int shuntSensePin = A3;
int inverterLowCapacityPin = DDA0;
int inverterPin = 24;
int ppvConnect = 25;
int battConnect = 26;
int gridConnectPin = 27;
bool batteryLow = true;
bool gridOn = false;
bool ppvNominal = true;
bool printtimeok =true;
bool timelimited = true;
bool debug = false;
bool io = true;
bool isInverterOn ;
bool isBatteryOn;
bool isPPVOn ;
bool isGridOn ;

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
 void showWarning(String message);
 void showReport(String message);
void inverterOn();
float sensePPVVoltage();
//turns off the dc power to inverter.
void inverterOff();
// senses ppv voltage
float sensePPVVoltage();

void serialCommandIssued(String commandName , int extra);
float senseCurrent();
float senseBattVoltage();
// connects ppv to batteries
void chargeBatteryConnect ();
// disconnect ppv from  batteries
void chargeBatteryDisconnect();
void gridConnect();
//disconnects ac side of inverter from full load.
void gridDisconnect();
void runSelfTest();
 void showWarning(String message);

 void showReport(String message);
void printVoltages();
void printActive();
void printDeactive();
void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200);
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
runSelfTest();


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
if (batv <= 2.5) {
chargeBatteryDisconnect();
}
if (batv >2.5) {
chargeBatteryConnect();
}
if (batv > 2.52 ) {
inverterOn();
}else{
  inverterOff();
}

}
// senses ppv voltage
float sensePPVVoltage()
{
 int sensorValue = analogRead(ppvSensePiin);
  map(sensorValue,0,1023,0,5);
  float voltage = sensorValue * (5.0 / 1023.0);
  // print out the value you read:

 


  return voltage;
}
void serialCommandIssued(String commandName , int extra)
{if(commandName == "active"){extra =0;}
if(commandName == "off"){extra =1;}
if(commandName == "volt"){extra =2;}
if(commandName == "grid on"){extra =4;}
if(commandName == "debug"){extra =3;}
if(commandName == "battery"){extra =5;}
if(commandName == "ppv"){extra =6;}
if(commandName == "battery on"){extra =7;}
if(commandName == "battery off"){extra =8;}

digitalWrite(ledPin,HIGH);
delay(100);
digitalWrite(ledPin,LOW);
float b ,p;
//if(commandName == "3"){extra =3;}
switch (extra) {
  case 0:
printActive();
  break;
  case 1:
 gridDisconnect();
 chargeBatteryDisconnect();
 inverterOff();


  break;
   case 2:
 printVoltages();
  break;
   case 3:
if (debug==true){
   debug = false;
  }else{
    debug = true;
  } 
  break;
   case 4:
gridConnect();
  break;
  case 5:
 b =  senseBattVoltage();
Serial.println("Battery Voltage:\t");
Serial.println(b);
delay(100);
  break;
    case 6:
  p =  sensePPVVoltage();
Serial.println("panel Voltage:\t");
Serial.println(p);
delay(100);
  break;
    case 7:
chargeBatteryConnect();
  break;
    case 8:
chargeBatteryDisconnect();
  break;
    case 9:
if (timelimited==true){
   timelimited = false;
  }else{
    timelimited = true;
  } 
  break;
    case 10:
if (timelimited==true){
   timelimited = false;
  }else{
    timelimited = true;
  } 
  break;
  default:

  break;
}
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
   map(sensorValue,0,1023,0,5);
  float voltage = sensorValue * (5.0 / 1023.0) ;
  // print out the value you read:
 

return voltage/shuntValue;
}

// senses batteries voltage
float senseBattVoltage()
{
   int sensorValue = analogRead(batterySensePin);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  map(sensorValue,0,1023,0,5);
  float voltage = sensorValue * (5.0 / 1023.0);
  // print out the value you read:
  
   return voltage;
}
// connects ppv to batteries
void chargeBatteryConnect ()
{  isBatteryOn = true;
isPPVOn =true;
  digitalWrite(battConnect, LOW);
digitalWrite(ppvConnect, LOW);


}
// disconnect ppv from  batteries
void chargeBatteryDisconnect()
{isBatteryOn = false;
isPPVOn = false;
 digitalWrite(battConnect, HIGH);
digitalWrite(ppvConnect, HIGH);


}
//connects ac side of inverter to full load.
void gridConnect()
{
isGridOn = true;
isInverterOn = false;
digitalWrite(gridConnectPin , LOW);
digitalWrite(inverterPin , HIGH);
}
//disconnects ac side of inverter from full load.
void gridDisconnect()
{
 isGridOn = false;
digitalWrite(gridConnectPin , HIGH);

  
}
//turns on dc power to inverter
void inverterOn()
{
 isGridOn = false;
 isInverterOn = true;
digitalWrite(gridConnectPin , HIGH);
digitalWrite(inverterPin , LOW);



}
//turns off the dc power to inverter.
void inverterOff()
{
isInverterOn = false;
digitalWrite(inverterPin , HIGH);

}
// puts inverter ac load on but with a small load.
void inverterLowCompacity()
{
  if (gridOn == false) {
  
  
 digitalWrite(inverterLowCapacityPin , LOW);
 
 }else {
 Serial.println("\t WARNING! : \n \tgrid power Is On.\n\t WARNING!");
 }
}
void runSelfTest()
{
  float bv = senseBattVoltage();
if (bv <= 1.50f){
showWarning("no battery preasent");

}
delay(1000);
float solpanv = sensePPVVoltage();
if (solpanv >= 1.0f)
{

Serial.println("panels:");
Serial.println(solpanv);
delay(400);
Serial.println("OK");

}else {
showWarning("panels not preasent");
}
inverterOn();
digitalWrite(ledPin , HIGH);
delay(1000);
inverterOff();

digitalWrite(ledPin, LOW);
showWarning("Startup Complete");
}
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    
    // add it to the inputString:
    inputString += inChar;
    serialCommandIssued(inputString, 1);
    Serial.println(inputString);
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
 
}
 void showWarning(String message)
{

Serial.println("\t\tWarning!");
Serial.println(message);
Serial.println("\t\tWarning!");
}
  
 
 void showReport(String message)
  {
if (message == "Self Test"){

Serial.println("\tStarting services!");
Serial.println(message);
Serial.println("hang in there");

}else{

Serial.println(message);

}


}

void printActive(){
if(isBatteryOn == true){
  Serial.println("Battery is ON:"); Serial.println(batv);
}else {
 Serial.println("Battery is OFF:");
}
 if(isInverterOn == true){Serial.println("Inverter is ON:");
 }else {
 Serial.println("Inverter is OFF:");
 }
if(isPPVOn == true){ Serial.println("PPV is ON:"); Serial.println(ppv);

}else {
 Serial.println("PPV is OFF:");
}
if(isGridOn == true){ Serial.println("Grid is ON");
}else {
Serial.println("Grid is OFF");
}

}