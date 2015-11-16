/*
 Analog Read example for Ubidots 
 
 This sketch connects to Ubidots (http://www.ubidots.com) using an Arduino Wifi shield.
 
 This example is written for a network using WPA encryption. You only need your SSID and PASS.
 
 This example uses Ubidots official library for Arduino WiFi Shield, for the Ubidots API v1.6.
 
 With this example you can read an analog sensor with the arduino, and post it
 to the Ubidots platform with the function save_value 
 
 Components:
 * Arduino Wifi shield
 * Serial Monitor in your Arduino IDE
 
 Created 9 Jun 2014
 Modified 9 Jun 2014
 by Mateo Vélez
 
 This code is in the public domain.
 
 */
#include <WiFi.h>
#include <Ubidots.h>
char ssid[] = "Atom House Medellin";                                                 //your network SSID (name) 
char pass[] = "atommed2014";                                                         //your network password (use for WPA, or use as key for WEP)
String api = "xxxxxxx6794f9043f0918ddcbd26";                                         //your API Key 
String idvari = "53baaf3c76254244e1c8e408";                                          //the number of the Ubidots variable
int sensorValue = 0;                                                                 //variable to store the value coming from the sensor
int sensorPin = A0;                                                                  //select your analog pin
Ubidots ubiclient(api);                                                              //call the api with the function ubiclient


void setup()
{
   boolean response;                                                                 
   int status = WL_IDLE_STATUS;                                                      //we need to define first a WL_IDLE_STATUS for the network
   Serial.begin(9600);                                                               //9600 baud for serial transmision
   response = ubiclient.WifiCon(ssid, pass, status, api);                               //this function is to connect to your wifi network
   Serial.println(response);                                                         //print response to the Serial Monitor
}
void loop()
{
 
       sensorValue = analogRead(sensorPin); 
       if (ubiclient.save_value(idvari, String(sensorValue)))                         //this function is to post to ubidots and return True or False depending on the connection status
       {
         Serial.println("The sensor value " + String(sensorValue) + " was sent to Ubidots");                //print the sensor value     
       }
     
}
