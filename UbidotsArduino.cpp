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

#include "UbidotsArduino.h"
/**
 * Constructor.
 */
Ubidots::Ubidots(char* token) {
    _token = token;
    maxValues = 5;
    currentValue = 0;
    val = (Value *)malloc(maxValues*sizeof(Value));
}
/** 
 * This function is to get value from the Ubidots API
 * @arg id the id where you will get the data
 * @return num the data that you get from the Ubidots API
 */
float Ubidots::getValue(char* id) {
  float num;
  String raw;
  char reply[500];
  int i = 0;
  uint8_t bodyPosinit;
  uint8_t bodyPosend;
  _client.connect(SERVER, PORT);
  if (_client.connected()) {
        Serial.println(F("Geting your variable"));
        _client.print(F("GET /api/v1.6/variables/"));
        _client.print(id);
        _client.println(F("/values?page_size=1 HTTP/1.1"));
        _client.println(F("Host: things.ubidots.com"));
        _client.println(F("User-Agent: Arduino-Ethernet/1.0"));
        _client.print(F("X-Auth-Token: "));
        _client.println(_token);
        _client.println(F("Connection: close"));
        _client.println();
    } else {
        Serial.println(F("Connection failed"));
        currentValue = 0;
        return NULL;
    }
    int timeout = 0;
    while (!_client.available() && timeout < 5000) {
      timeout++;
      delay(1);
    }
    while (_client.available()) {
        reply[i] = _client.read();
        i++;
        if (i >= 499) {
          i = 0;
          break;
        }
        //Serial.write(c);
    }
    _client.stop();
    Serial.println(reply);
    char* pch = strstr(reply,"\"value\":");
    raw = String(pch);
    bodyPosinit = 9 + raw.indexOf("\"value\":");
    bodyPosend = raw.indexOf(", \"timestamp\"");
    raw.substring(bodyPosinit, bodyPosend).toCharArray(reply, 10);
    num = atof(reply);
    return num;
}
/**
 * Add a value of variable to save
 * @arg variable_id variable id to save in a struct
 * @arg value variable value to save in a struct
 */
void Ubidots::add(char *variable_id, float value) {
  (val+currentValue)->id = variable_id;
  (val+currentValue)->value_id = value;
  currentValue++;
  if (currentValue > maxValues) {
      Serial.println(F("You are sending more than 5 consecutives variables, you just could send 5 variables. Then other variables will be deleted!"));
      currentValue = maxValues;
  }
}
/**
 * Send all data of all variables that you saved
 * @reutrn true upon success, false upon error.
 */
bool Ubidots::sendAll() {
    int i;
    String all;
    String str;
    char b[3];
    all = "[";
    for (i = 0; i < currentValue; ) {
        str = String(((val+i)->value_id), 2);
        all += "{\"variable\": \"";
        all += String((val + i)->id);
        all += "\", \"value\":";
        all += str;
        all += "}";
        i++;
        if (i < currentValue) {
            all += ", ";
        }
    }
    all += "]";
    i = all.length();
    Serial.println(all);
    if (_client.connect(SERVER, PORT)) {
        Serial.println(F("Posting your variables"));
        _client.println(F("POST /api/v1.6/collections/values/?force=true HTTP/1.1"));
        _client.println(F("Host: things.ubidots.com"));
        _client.println(F("User-Agent: Arduino-Ethernet/1.0"));
        _client.print(F("X-Auth-Token: "));
        _client.println(_token);
        _client.println(F("Connection: close"));
        _client.println(F("Content-Type: application/json"));
        _client.print(F("Content-Length: "));
        _client.println(String(i));
        _client.println();
        _client.println(all);
        _client.println();
    } else {
        Serial.println(F("Connection failed"));
        currentValue = 0;
        return false;
    }
    int timeout = 0;
    while (!_client.available() && timeout < 5000) {
        timeout++;
        delay(1);
    }
    while (_client.available()) {
        char c = _client.read();
        Serial.write(c);
    }
    currentValue = 0;
    _client.stop();
    return true;
}
