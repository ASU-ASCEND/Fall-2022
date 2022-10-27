
//Uncommented all serial.print

//******************Libraries**********************************************
#include <Arduino.h>
#include <Adafruit_Sensor.h>    //Library for many Adafruit sensor libraries. REQUIRED.
#include <Wire.h>               //Library for I2C serial protocol. REQUIRED.
#include <SPI.h>                //Library for microSD card module.
#include <SD.h>                 //Library for microSD card module.
#include <Adafruit_MPU6050.h>   //Library for MPU6050 sensor  
#include <Adafruit_MPRLS.h>     //Library for MPRLS sensor
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>

//*******************Defines**********************************************
#define LEDpin 2
#define MAXBUF_REQUIREMENT 48 // The used commands uses up to 48 bytes. On some Arduino's the default buffer space is not large enough

#if (defined(I2C_BUFFER_LENGTH) &&                 \
     (I2C_BUFFER_LENGTH >= MAXBUF_REQUIREMENT)) || \
    (defined(BUFFER_LENGTH) && BUFFER_LENGTH >= MAXBUF_REQUIREMENT)
#define USE_PRODUCT_INFO
#endif

#define RESET_PIN  -1  // set to any GPIO pin # to hard-reset on begin()
#define EOC_PIN    -1  // set to any GPIO pin to read end-of-conversion by pin
#define TCAADDR 0x70   //multiplexer

//*****************Objects************************************************
SFE_UBLOX_GNSS myGNSS;
Adafruit_MPRLS mpr = Adafruit_MPRLS(RESET_PIN, EOC_PIN);
Adafruit_MPU6050 mpu;

//*****************Variables**********************************************
char logFileName[16]; //Character strings for writing data to memory 
String header = "StratoDevils_Component_Code"; // Variable for file name
String dataString = ""; //holds the entire data string for each read cycles
float sensorVal;
boolean groundMode = 0;


void tcaselect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}






void setup() {
//********************General Initialization*********************************

  pinMode(LEDpin, OUTPUT); 
  digitalWrite(LEDpin, HIGH); delay(100); digitalWrite(LEDpin, LOW); delay(100); digitalWrite(LEDpin, HIGH); delay(100); digitalWrite(LEDpin, LOW);  //Pulse LED to signal that payload has power.
  delay(4000);
  Serial.begin(9600);           //Begin the Serial communication and set it to the proper "channel" (baud rate) to enable the serial monitor.
  Wire.begin();                 //Enables I2C communication

//********************GNSS INITIALIZATION*********************************
while(myGNSS.begin() == false) //Connect to the u-blox module using Wire port
  {
    //serial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
  }
  //serial.println(F("GNSS connected"));
  myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
  myGNSS.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); //Save (only) the communications port settings to flash and BBR

//*****************TCA9548A Multiplexer INITIALIZATION********************
  /* Initialise the 1st sensor */
  tcaselect(2);
  /* Initialise the 2nd sensor */
  tcaselect(6);

//*****************MPU6050 INITIALIZATION*********************************
if (!mpu.begin()) {
    //serial.println(F("Failed to find MPU6050 chip"));
  }
  else {
    //serial.println(F("MPU6050 Found!"));

    mpu.setAccelerometerRange(MPU6050_RANGE_16_G);       //This is a switch-case statement that can change the property of the sensor
    //serial.print("Accelerometer range set to: ");
    switch (mpu.getAccelerometerRange()) {
      case MPU6050_RANGE_2_G:
        //serial.println(F("+-2G"));
        break;
      case MPU6050_RANGE_4_G:
        //serial.println(F("+-4G"));
        break;
      case MPU6050_RANGE_8_G:
        //serial.println(F("+-8G"));
        break;
      case MPU6050_RANGE_16_G:
        //serial.println(F("+-16G"));
        break;
    }

    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    //serial.print("Gyro range set to: ");
    switch (mpu.getGyroRange()) {
      case MPU6050_RANGE_250_DEG:
        //serial.println("+- 250 deg/s");
        break;
      case MPU6050_RANGE_500_DEG:
        //serial.println("+- 500 deg/s");
        break;
      case MPU6050_RANGE_1000_DEG:
        //serial.println("+- 1000 deg/s");
        break;
      case MPU6050_RANGE_2000_DEG:
        //serial.println("+- 2000 deg/s");
        break;
    }

    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    //serial.print("Filter bandwidth set to: ");
    switch (mpu.getFilterBandwidth()) {
      case MPU6050_BAND_260_HZ:
        //serial.println(F("260 Hz"));
        break;
      case MPU6050_BAND_184_HZ:
        //serial.println(F("184 Hz"));
        break;
      case MPU6050_BAND_94_HZ:
        //serial.println(F("94 Hz"));
        break;
      case MPU6050_BAND_44_HZ:
        //serial.println(F("44 Hz"));
        break;
      case MPU6050_BAND_21_HZ:
        //serial.println(F("21 Hz"));
        break;
      case MPU6050_BAND_10_HZ:
        //serial.println(F("10 Hz"));
        break;
      case MPU6050_BAND_5_HZ:
        //serial.println(F("5 Hz"));
        break;
    }
  }

//*****************MPRLS INITIALIZATION*********************************
if (! mpr.begin()) {
    //serial.println(F("Failed to communicate with MPRLS sensor, check wiring?"));
  } 
  else {
    //serial.println(F("Found MPRLS sensor"));
  }

//*****************SD CARD INITIALIZATION*********************************
  pinMode(10, OUTPUT);
  SPI.begin();
  SD.begin(10);
  SDCardInit();
  //serial.println();

  /* HOW TO SAVE A TEXT FILE
    1. Open the file. (NOTE: Only one file can be open at a time, so you have to close this one before opening another.)
      myFile = SD.open("test.txt", FILE_WRITE);

    2. If the file opened okay, write to it:
      if (myFile) {
    //serial.print("Writing to test.txt...");
    myFile.println("This is a test file :)");
    myFile.println("testing 1, 2, 3.");
  */

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LEDpin, HIGH);
  dataString = "";

//********************GNSS*********************************
if(true){
  if(myGNSS.isConnected()){
    long latitude = myGNSS.getLatitude();
    //serial.print(F("Lat: "));
    //serial.print(latitude/10000000.0,6);
    //serial.print(F(" deg"));

    long longitude = myGNSS.getLongitude();
    //serial.print(F(" Long: "));
    //serial.print(longitude/10000000.0,6);
    //serial.print(F(" deg"));

    long altitude = myGNSS.getAltitude();
    //serial.print(F(" Alt: "));
    //serial.print(altitude);
    //serial.print(F(" mm"));
    //serial.println();
  }
  else{
    //serial.print(F("no signal")); 
  }
}
else{
  //serial.print(F("no connection"));
  }

//*****************TCA9548A Multiplexer********************
  /* Get a new sensor event */ 
  sensors_event_t event; 
  tcaselect(2);
  tcaselect(6);

//****************MPRLS********************
  float pressure_hPa = mpr.readPressure();

  //serial.print("Pressure (hPa): "); //serial.println(sensorVal=pressure_hPa);
    dataString += String(sensorVal, 2);
    dataString += ", ";

  //serial.print("Pressure (PSI): "); //serial.println(sensorVal=(pressure_hPa / 68.947572932));
    dataString += String(sensorVal, 2);
    dataString += ", ";

//*****************MPU6050********************
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  //serial.print("Acceleration X: "); 
    //serial.print(a.acceleration.x); 
    //serial.print(", Y: ");
    //serial.print(a.acceleration.y);
    //serial.print(", Z: ");
    //serial.print(a.acceleration.z); 
    //serial.println(" m/s^2");

  //serial.print("Rotation X: "); 
    //serial.print(g.gyro.x);
    //serial.print(", Y: ");
    //serial.print(g.gyro.y);
    //serial.print(", Z: ");
    //serial.print(g.gyro.z);
    //serial.println(" rad/s");

  //serial.print("Temperature: "); 
    //serial.print(temp.temperature);
    //serial.println(" degC");

//****************Battery Voltage********************
  float batteryV = analogRead(A6);
  //serial.print("Battery Voltage: ");//serial.print(batteryV, 3); //serial.println("V");
//****************WRITE TO SD CARD*******************
  dataString += String(sensorVal);
   writeDataToSD();
   //serial.println();
   digitalWrite(LEDpin, LOW);
   delay(2000);
} //End of loop

//#############################################################################################################
//--->FUNCTION is called when the system starts or after a power reset to enable recording data to the SD card.
boolean SDCardInit() 
{
  //serial.println();  
  //serial.println("Initializing SD card...");

  // see if the card is present and can be initialized:
 /* if (!SD.begin(8))
  {
    //serial.println("Card failed, or not present");
    //delay(100);
    // Can't do anything more.  Try another time....
    return 1;
  }
  */
  // The Card is present so find an unused file name
  //serial.println("searching for an unused file name... ");
  // Start with LOG0.CSV and count up until an unused file name is found.
  for (int i = 0; i < 1000; i++)
  {
    sprintf(logFileName, "LOG%d.CSV", i);
    if (!SD.exists(logFileName))
    {
      break;
    }
  }

  //serial.print("The Log filename is:  ");
    //serial.println(logFileName);

  File dataFile = SD.open(logFileName, FILE_WRITE);

  // Write the header including sensor names to the newly opened file
  dataString = header;

  if (dataFile)
  {
    dataFile.println(dataString);
    dataFile.close();
    //serial.println("SD Card initialized and data written.");
    return (1);                          // Able to write to SD card
  }
  else
  {
    //serial.println("SD card present but unable to write to file");
    sprintf(logFileName, "LOG0.CSV");           // Clear out the file name to force the program to find a unused file name
    return (0);                           // Unable to write to SD card
  }
}

boolean writeDataToSD() 
{
  File dataFile = SD.open(logFileName, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile)
  {
    dataFile.println(dataString);
    dataFile.close();
    return (1);                                             // Return a 1 since the write was successful LED Blink
  }

  // if the file isn't open, notify that there was an error and re-initialize  the card
  else 
  {
    //if (groundMode) serial.println();
    //if (groundMode) serial.print("error writing to file: ");
    //if (groundMode) serial.println(logFileName);
    dataFile.close();
    delay(100);
    //if (groundMode) serial.println("Re - Initializing SD card...");
    return (SDCardInit());          //Return a 0 if successful to disable LED Blink
  }
}
