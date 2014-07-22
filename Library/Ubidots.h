/*
Ubidots.cpp -El objetivo de la librería es realizar la conexión con la plataforma ubidots, para que los usuarios puedan postear o escribir su información 
en la nube


*/
#ifndef Ubidots_h
#define Ubidots_h
#include "arduino.h"

#include <WiFi.h>
#include <String.h>

class Ubidots {
  public:
   Ubidots(String apikey);
   String get_value(String idvariable);
   boolean save_value(String idvariable,String valor);
   boolean save_value(String idvariable,String valor,String ctext);
   boolean ubitoken();
   boolean WifiCon(char *ssid,char *pass,int status, String api);  
 
  private: 
   void printWifiStatus();
   String _apikey; 	
   String _token;
   String _mensaje PROGMEM;
   char _c;
   int resetTimer;
   boolean response;
   String _le;
   String _var;
   String _valor2;
   String _value;
   WiFiClient _client;     
};

#endif
