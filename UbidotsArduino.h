/*
Copyright (c) 2013-2016 Ubidots.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Made by Mateo Velez - Metavix for Ubidots Inc

*/
#ifndef _UbidotsArduino_H_
#define _UbidotsArduino_H_
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <SPI.h>


#if defined(__AVR_ATmega328P__)||defined(__AVR_ATmega32U4__)||defined(__AVR_ATmega2560__)||defined (__AVR_ATtiny85__)
    #include <WiFi.h>
#else
    #include "WiFi101.h"
#endif

#define SERVER "things.ubidots.com"
#define PORT 80
#define MAX_VALUES 5

typedef struct Value {
  char  *id;
  float value_id;
} Value;

// Struct added by Dr. Juergen Leib
typedef struct ubi_value {
  char  *id;
  float value;
  unsigned long count;
  unsigned long timestamp;
  bool  valid;
} ubi_value;

class Ubidots {
 public:
    Ubidots(char* token, char* server = SERVER);
    bool sendAll();
    float * getValue(char* id);
    void getValueInfo(ubi_value* dot);  // Function added by Dr. Juergen Leib
    void add(char *variable_id, float value);

 private:
    char* _token;
    char* _server;
    uint8_t currentValue;
    Value * val;
    WiFiClient _client;
};

#endif  // _UbidotsArduino_H_
