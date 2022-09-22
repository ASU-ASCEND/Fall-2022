
#include<MS5607.h>

MS5607 P_Sens;

void setup(void){
  Serial.begin(9600);
  if(!P_Sens.begin()){
    Serial.println("Error in Communicating with sensor, check your connections!");
  }else{
    Serial.println("MS5607 initialization successful!");
  }
}
float H_val;
void loop(void){
  if(P_Sens.readDigitalValue()){
    H_val = P_Sens.getAltitude();
  }else{
    Serial.println("Error in reading digital value in sensor!");
  }

  Serial.print("Altitude    :  ");
  Serial.print(H_val);
  Serial.println(" meter");

  delay(1000);
}
