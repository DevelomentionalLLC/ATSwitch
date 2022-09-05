


#include "ATSwitch.hpp"
#include "Battery.hpp"
public:

ATSwitch::void printVoltages(){
float solpanv = sensePPVVoltage();
Serial.println("panels:\t");Serial.println(solpanv);
float b =  senseBattVoltage();
Serial.println("Battery Voltage:\t");Serial.println(b);

}

ATSwitch::sensePPVoltage(){


}
ATSwitch::senseBattVoltage(){




}