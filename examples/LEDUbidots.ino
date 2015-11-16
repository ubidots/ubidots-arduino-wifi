/*
  Example with LED for Ubidots
 
 This sketch connect to Ubidots (http://www.ubidots.com)
 using an Arduino Wifi shield.
 
 This example is written for a network using WPA encryption. You only need your SSID and PASS.
 
 This example implements the Ubidots library for the Arduino WiFi Shield, for the 1.6 API version.
 With this example you can turn an LED on/off with a Switch from the Ubidots dashboard
 
 Arduino pins:
 A0  --> LED +
 GND --> LED -
 
 Created 9 Jun 2014
 Modified 9 Jun 2014
 by Mateo Vélez
 
 This code is in the public domain.
 
 */
#include <WiFi.h>
#include <Ubidots.h>
char ssid[] = "Atom House Medellin";                                    //your network SSID (name) 
char pass[] = "atommed2014";                                            //your network password (use for WPA, or use as key for WEP)
String api = "xxxxxxxxe0492c6794f9043f0918ddcbd26";                //your API Key number
String idvari = "53beeca07625420bde83ce29";                             //the number of the Ubidots variable

Ubidots ubiclient(api);                                                 //with that you call the api with the prefix ubiclient
String readvalue = "";
boolean response;
int valor_sensor = 0;
int status = WL_IDLE_STATUS;
void setup()
{
   Serial.begin(9600);                                                   //9600 baud for serial transmission
                                                                         //You need boolean variable to save value from WifiCon function, (True, False)
                                                                         //we need to define first a WL_IDLE_STATUS for the network   
   response = ubiclient.WifiCon(ssid, pass, status, api);                //this function is to connect to your wifi network
 
   
}
void loop()
{
  
             pinMode(A0, OUTPUT); 
             readvalue = ubiclient.get_value(idvari);
             Serial.println(readvalue);
             if(readvalue == "1.0")
             {
                
               digitalWrite(A0, HIGH);
             }
             else
             {
               digitalWrite(A0, LOW);
             }
        
}
