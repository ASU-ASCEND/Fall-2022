#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
 
#include<MS5607.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55);
MS5607 P_Sens; 


void setup(void) 
{
  Serial.begin(9600);
  Serial.println("Orientation Sensor Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  if(!P_Sens.begin()){
    Serial.println("Error in Communicating with sensor, check your connections!");
  }else{
    Serial.println("MS5607 initialization successful!");
  }
  
  delay(1000);
    
  bno.setExtCrystalUse(true);
}

float H_val;

void loop(void) 
{
  /* Get a new sensor event */ 
  sensors_event_t event; 
  bno.getEvent(&event);
  
  /* Display the floating point data */
  Serial.print("X: ");
  Serial.print(event.orientation.x, 4);
  Serial.print("\tY: ");
  Serial.print(event.orientation.y, 4);
  Serial.print("\tZ: ");
  Serial.print(event.orientation.z, 4);
  Serial.println("");
  
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
