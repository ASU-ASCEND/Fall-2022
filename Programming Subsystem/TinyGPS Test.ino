#include <Arduino.h>
#include <Adafruit_Sensor.h>    //Library for many Adafruit sensor libraries. REQUIRED.
//#include <Wire.h>               //Library for I2C serial protocol. REQUIRED.
#include <SoftwareSerial.h>
#include <TinyGPS.h>

#define RXPIN 30
#define TXPIN 31
#define LEDpin 2

SoftwareSerial nss(RXPIN, TXPIN);
TinyGPS gps;
long lat, lon;
unsigned long fix_age; 

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



void setup() {
  // put your setup code here, to run once:
  pinMode(LEDpin, OUTPUT); 
  digitalWrite(LEDpin, HIGH); delay(100); digitalWrite(LEDpin, LOW); delay(100); digitalWrite(LEDpin, HIGH); delay(100); digitalWrite(LEDpin, LOW);  //Pulse LED to signal that payload has power.
  delay(4000);
  Serial.begin(9600);           //Begin the Serial communication and set it to the proper "channel" (baud rate) to enable the serial monitor.
}

void loop() {
  // put your main code here, to run repeatedly:
  while (nss.available())
  {
    int c = nss.read();
    if (gps.encode(c))
    {
      // process new gps info here
      gps.get_position(&lat, &lon, &fix_age);
      Serial.print("lat:");
      Serial.print(lat);
      Serial.print("lon:");
      Serial.print(lon);
      Serial.print("/n");
      
      delay(1000);
    }
  }
}
