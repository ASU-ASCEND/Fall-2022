/***************************************************************************
  Title: StratoDevils_Components_Code_V3.1 
  Name(s): Anyell Mata, Jesse Ontiveros, Berkeley Adair, Lizzie Garayzar, Feras Jayyusi, Cassandra Ward, Wilson Luu, Sagan Richardson, Feras Al-Qassas, 
  Date: 10-14-22
  Description: 
  V3.0 
  This code was constructed mainly using basic example code and StratoDevil's previous
  launch code. The sensors used are: TMP36 (Temperature), , radino32_sx1272 (Radio), mpu6050 (Accelerometer & Gyro), MPRLS (Pressure), 
  amazon spi microsd card (mircoSD Card), SEN54(VOC), 

  V3.3 Feras Jayyusi, Cassandra Ward, Wilson Luu, Sagan Richardson, Anyell Mata
  Removed TMP36, Radino32, and SEN54(VOC)
  Added SD Card Functionality
  Added GPS, GNSS library was too big to create a sketch, tinyGPS library utilized instead
  
 ***************************************************************************/

#include <Arduino.h>
#include <Adafruit_Sensor.h>    //Library for many Adafruit sensor libraries. REQUIRED.
#include <Wire.h>               //Library for I2C serial protocol. REQUIRED.
#include <SPI.h>                //Library for microSD card module.
#include <SD.h>                 //Library for microSD card module.
#include <Adafruit_MPU6050.h>   //Library for MPU6050 sensor  
#include <Adafruit_MPRLS.h>     //Library for MPRLS sensor
#include <SoftwareSerial.h>
#include <TinyGPS.h>


#define LEDpin 2
#define RXPIN 1
#define TXPIN 0
#define TCAADDR 0x70 //multiplexer
// The used commands use up to 48 bytes. On some Arduino's the default buffer
// space is not large enough
#define MAXBUF_REQUIREMENT 48
#if (defined(I2C_BUFFER_LENGTH) &&                 \
     (I2C_BUFFER_LENGTH >= MAXBUF_REQUIREMENT)) || \
    (defined(BUFFER_LENGTH) && BUFFER_LENGTH >= MAXBUF_REQUIREMENT)
#define USE_PRODUCT_INFO
#endif
// You dont *need* a reset and EOC pin for most uses, so we set to -1 and don't connect
#define RESET_PIN  -1  // set to any GPIO pin # to hard-reset on begin()
#define EOC_PIN    -1  // set to any GPIO pin to read end-of-conversion by pin
//************Variables*************************************
Adafruit_MPRLS mpr = Adafruit_MPRLS(RESET_PIN, EOC_PIN);
Adafruit_MPU6050 mpu;           //This basically shortens 'Adafruit_MPU6050' to 'mpu' so that you don't have to repeatedly use the longer name.
SoftwareSerial ss(RXPIN, TXPIN);
TinyGPS gps;
long lat, lon;
unsigned long fix_age; 
unsigned long currentTime = 0;
float sensorVal;
boolean groundMode = 0;
// Variable for file name
char logFileName[16];
//Character strings for writing data to memory //
String header = "Time,Pressure hpa,Pressure psi,Acc X m/s^2,Acc Y m/s^2,Acc Z m/s^2,Volt 3V,Lat,Lon";
String dataString = ""; //holds the entire data string for each read cycles


void setup() {
  pinMode(LEDpin, OUTPUT); 
  digitalWrite(LEDpin, HIGH); delay(100); digitalWrite(LEDpin, LOW); delay(100); digitalWrite(LEDpin, HIGH); delay(100); digitalWrite(LEDpin, LOW);  //Pulse LED to signal that payload has power.
  delay(4000);
  Serial.begin(9600);           //Begin the Serial communication and set it to the proper "channel" (baud rate) to enable the serial monitor.
  Wire.begin();                 //Enables I2C communication
  ss.begin(9600);               //GPS Serial Initialization
  Serial.println("Simple TinyGPS library v. "); Serial.println(TinyGPS::library_version()); //Tiny GPS Version Number

//*****************TCA9548A Multiplexer********************
  /* Initialise the 1st sensor */
  tcaselect(2);
  
  /* Initialise the 2nd sensor */
  tcaselect(6);

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
  }
  else {
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);       //This is a switch-case statement that can change the property of the sensor
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
    case MPU6050_RANGE_2_G:
      Serial.println("+-2G");
      break;
    case MPU6050_RANGE_4_G:
      Serial.println("+-4G");
      break;
    case MPU6050_RANGE_8_G:
      Serial.println("+-8G");
      break;
    case MPU6050_RANGE_16_G:
      Serial.println("+-16G");
      break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
    case MPU6050_RANGE_250_DEG:
      Serial.println("+- 250 deg/s");
      break;
    case MPU6050_RANGE_500_DEG:
      Serial.println("+- 500 deg/s");
      break;
    case MPU6050_RANGE_1000_DEG:
      Serial.println("+- 1000 deg/s");
      break;
    case MPU6050_RANGE_2000_DEG:
      Serial.println("+- 2000 deg/s");
      break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
    case MPU6050_BAND_260_HZ:
      Serial.println("260 Hz");
      break;
    case MPU6050_BAND_184_HZ:
      Serial.println("184 Hz");
      break;
    case MPU6050_BAND_94_HZ:
      Serial.println("94 Hz");
      break;
    case MPU6050_BAND_44_HZ:
      Serial.println("44 Hz");
      break;
    case MPU6050_BAND_21_HZ:
      Serial.println("21 Hz");
      break;
    case MPU6050_BAND_10_HZ:
      Serial.println("10 Hz");
      break;
    case MPU6050_BAND_5_HZ:
      Serial.println("5 Hz");
      break;
  }
  }

  if (! mpr.begin()) {
    Serial.println("Failed to communicate with MPRLS sensor, check wiring?");
  } else {
  Serial.println("Found MPRLS sensor");
  }

//GPS Initialization
  bool newData = false;

//SD card initialization
  pinMode(10, OUTPUT);
  SPI.begin();
  SD.begin(10);
  SDCardInit();
  Serial.println();
}


void loop() {
  digitalWrite(LEDpin, HIGH);
  dataString = "";
//*****************Current Time****************************

currentTime = millis() / 1000; 
round(currentTime);
Serial.print("Seconds since launch: ");
Serial.println(currentTime);
dataString += String(currentTime);
dataString += ", ";

//*****************TCA9548A Multiplexer********************
  /* Get a new sensor event */ 
  sensors_event_t event; 
  tcaselect(2);
  tcaselect(6);

//****************MPRLS********************
  float pressure_hPa = mpr.readPressure();
  Serial.print("Pressure (hPa): "); Serial.println(sensorVal=pressure_hPa);
    dataString += String(sensorVal, 2);
    dataString += ", ";
  Serial.print("Pressure (PSI): "); Serial.println(sensorVal=(pressure_hPa / 68.947572932));
    dataString += String(sensorVal, 2);
    dataString += ", ";

//*****************MPU6050********************
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  Serial.print("Acceleration X: "); Serial.print(a.acceleration.x); Serial.print(", Y: "); Serial.print(a.acceleration.y); Serial.print(", Z: "); Serial.print(a.acceleration.z); Serial.println(" m/s^2");
  dataString += (String(a.acceleration.x, 2)); 
dataString += ", ";
  dataString += (String(a.acceleration.y, 2));
dataString += ", ";
  dataString += (String(a.acceleration.z, 2));
dataString += ", ";
  //Serial.print("Rotation X: "); Serial.print(g.gyro.x); Serial.print(", Y: "); Serial.print(g.gyro.y); Serial.print(", Z: "); Serial.print(g.gyro.z); Serial.println(" rad/s");
  //Serial.print("Temperature: "); Serial.print(temp.temperature); Serial.println(" degC");
/*
 //****************TMP36's********************
  float inTmpV = analogRead(A0) * (5.0 / 1024.0); // For memory purposes it might be best to just get the analog reading and convert it into Excel later
  float extTmpV = analogRead(A1) * (5.0 / 1024.0);
  float inTmpC = (inTmpV - 0.5) * 100;
  float extTmpC = (extTmpV - 0.5) * 100;
  Serial.print("Internal Temperature: ");Serial.print(inTmpC, 3); Serial.println(" C");
  Serial.print("External Temperature: ");Serial.print(extTmpC, 3); Serial.println(" C");
*/
/*
 //****************RTD********************
  float rtdV1 = analogRead(A0);
  float rtdV2 = analogRead(A1);
  Serial.print("RTD Temperature1: ");Serial.print(rtdV1, 3); Serial.println(" V");
  Serial.print("RTD Temperature2: ");Serial.print(rtdV2, 3); Serial.println(" V");
*/
 //****************Battery Voltage********************
  float batteryV = analogRead(A6);
  Serial.print("Battery Voltage: ");Serial.print(batteryV, 3); Serial.println("V");
  dataString += String(batteryV);
  dataString += ", ";
//***************GPS COORDINATES**********************
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData)
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    Serial.print("LAT=");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    dataString += String(flat, 6);
    dataString += ", ";
    Serial.print(" LON=");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    dataString += String(flon, 6);
    dataString += ", ";
    Serial.print(" SAT=");
    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.print(" PREC=");
    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
  }
  
  gps.stats(&chars, &sentences, &failed);
  Serial.print(" CHARS=");
  Serial.print(chars);
  Serial.print(" SENTENCES=");
  Serial.print(sentences);
  Serial.print(" CSUM ERR=");
  Serial.println(failed);
  
  if (chars == 0)
    Serial.println("** No characters received from GPS: check wiring **");
//**************Create CSV File**************************
  //This will be the buffer we use to write to the SD
  dataString += String(digitalRead(47));
  writeDataToSD();
  Serial.println();

  digitalWrite(LEDpin, LOW);
  delay(1000);

}//End of loop


//#############################################################################################################
//--->FUNCTION is called when the system starts or after a power reset to enable recording data to the SD card.
boolean SDCardInit() 
{
  Serial.println();  
  Serial.println("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(8))
  {
    Serial.println("Card failed, or not present");
    //delay(100);
    // Can't do anything more.  Try another time....
    return 1;
  }
  
  // The Card is present so find an unused file name
  Serial.println("searching for an unused file name... ");
  // Start with LOG0.CSV and count up until an unused file name is found.
  for (int i = 0; i < 1000; i++)
  {
    sprintf(logFileName, "LOG%d.CSV", i);
    if (!SD.exists(logFileName))
    {
      break;
    }
  }

  Serial.print("The Log filename is:  ");
    Serial.println(logFileName);

  File dataFile = SD.open(logFileName, FILE_WRITE);

  // Write the header including sensor names to the newly opened file
  dataString = header;

  if (dataFile)
  {
    dataFile.println(dataString);
    dataFile.close();
    Serial.println("SD Card initialized and data written.");
    return (1);                          // Able to write to SD card
  }
  else
  {
    Serial.println("SD card present but unable to write to file");
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
    if (groundMode) Serial.println();
    if (groundMode) Serial.print("error writing to file: ");
    if (groundMode) Serial.println(logFileName);
    dataFile.close();
    delay(100);
    if (groundMode) Serial.println("Re - Initializing SD card...");
    return (SDCardInit());          //Return a 0 if successful to disable LED Blink
  }
}

void tcaselect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}