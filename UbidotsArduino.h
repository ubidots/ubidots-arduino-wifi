/*
Ubidots.cpp -El objetivo de la librería es realizar la conexión con la plataforma ubidots, para que los usuarios puedan postear o escribir su información 
en la nube


*/
#ifndef _UbidotsArduino_H_
#define _UbidotsArduino_H_
#include <SPI.h>
#include <WiFi.h>


#define SERVER "things.ubidots.com"
#define PORT 80

typedef struct Value {
  char  *id;
  float value_id;
} Value;

class Ubidots {
   public:
      Ubidots(char* token);
      bool sendAll();
      float getValue(char* id);
      void add(char *variable_id, double value);
   
   private:
      char* _token;
      uint8_t maxValues;
      uint8_t currentValue;
      Value * val;
      WiFiClient _client;
   
};

#endif
