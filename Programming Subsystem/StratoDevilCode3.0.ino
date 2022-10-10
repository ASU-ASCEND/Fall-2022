/*****************************************
Title: StratoDevilsCodeV3.0
Name(s): Feras Jayyusi, Cassandra Ward, Sagan Richardson, Wilson Luu, Muhammed Topiwala, Burhanuddin Barafwala, Feras Al-Qassas, Joseph Cohen 
Last updated 10/9/2022 11:33 PM by Feras Jayyusi

 P L E A S E   R E A D ! ! ! !

Description: This program is for the Fall2022 ASCEND Payload at ASU. Much of the code is taken from StratoDevilCodeV2.1.0 with permission from David Lopez, 
therefore, this program should be considered an updated version of it. The payload utilizes two Arduinos of different models, "Arduino Nano" and "Arduino Nano Every".
This program is intended to be uploaded to the "Arduino Nano" and NOT the "Arduino Nano Every". (FJ 10/9)

NOTE FOR DEVELOPERS - Please comment on every line of code you write describing its function. Whenever you make a comment to the code, please add your initials at the end. 
Example: //This is a comment HI! :D (FJ 10/9)
Please also remember to update the section titled "Last updated" whenever you finish coding. Thank you!!! (FJ 10/9)
******************************************/

//**************Debug**********************
//Area to activate debug mode. Will be implemented later (FJ 10/9)

//*************Libraries*********************
#include <Wire.h> //Library used for I2C communication. (FJ 10/9)
#include <SD.h>   //SD Card Library. (FJ 10/9)

//**********SD CARD DECLARATIONS ****************
String file_name = "Data.csv"; // File name must follow 8.3 rule. <= 8 characters b4 period. only 3 characters for extension. "12345678.123" <-- Max characters. https://www.arduino.cc/en/Reference/SDCardNotes (FJ 10/9)
File ascendDataObj;            // Create a File object, that allows for reading and writing data. (FJ 10/9)

//**********BATTERY DECLARATIONS*****

//int battPin = A6; //Not sure if this is the pin we will be using for the battery (FJ 10/9) 
float voltage; //float value for voltage reading (FJ 10/9)

void setup() {
  //****************************Initial Start-up**************************************************************
  Serial.begin(115200); //What is the baud rate being used? (FJ 10/9)
  Wire.begin();         //Initialize Wire Library for I2C Communication (FJ 10/9)
  delay(3000);          //What is the delay used for? (FJ 10/9)
}

void loop() {

}
