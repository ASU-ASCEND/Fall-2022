#include <Wire.h>
#include <Adafruit_Sensor.h>
/* IMU Library */
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
/* Altimeter Library*/
#include<MS5607.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55);
MS5607 P_Sens; 

/* Two "independant" timed events */
const long eventTime_IMU = 100; //in ms
const long eventTime_Altimeter = 1000; //in ms

/* When did they start the race? */
unsigned long previousTime_1 = 0;
unsigned long previousTime_2 = 0;

void setup(void) 
{
  Serial.begin(9600);
  Serial.println("Orientation Sensor Test"); 
  
  /* Initialise the IMU sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  /* Initalise the Altimeter */
  if(!P_Sens.begin()){
    Serial.println("Error in Communicating with sensor, check your connections!");
  }else{
    Serial.println("MS5607 initialization successful!");
    Serial.println("");
  }
  
  delay(1000);
    
  bno.setExtCrystalUse(true);
}

/* Initialising Height value for the altimeter */
float H_val;

void loop(void) 
{
  /* Get a new sensor event */ 
  sensors_event_t event; 
  bno.getEvent(&event);
  
  /* Updates frequently */
  unsigned long currentTime = millis();
  if(currentTime - previousTime_1 >= eventTime_IMU)
  {
    /* Display the floating point data */
    Serial.print("X: ");
    Serial.print(event.orientation.x, 4);
    Serial.print("\tY: ");
    Serial.print(event.orientation.y, 4);
    Serial.print("\tZ: ");
    Serial.print(event.orientation.z, 4);
    Serial.println("");

    /* Update the timing for the next event*/
    previousTime_1 = currentTime;
  }
    
  if ( currentTime - previousTime_2 >= eventTime_Altimeter) 
  {
  /* Reading raw data */
  if(P_Sens.readDigitalValue()){
    H_val = P_Sens.getAltitude();
  }else{
    Serial.println("Error in reading digital value in sensor!");
  }
  /* Display altimeter data */
  Serial.print("Altitude    :  ");
  Serial.print(H_val);
  Serial.println(" meter");
     /* Update the timing for the next event*/
    previousTime_2 = currentTime;
  }
  
}
