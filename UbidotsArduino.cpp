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
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "UbidotsArduino.h"

/**
 * Constructor.
 */
Ubidots::Ubidots(char* token, char* server) {
    _token = token;
    _server = server;
    currentValue = 0;
    val = (Value *)malloc(MAX_VALUES*sizeof(Value));
}
/** 
 * This function is to get value from the Ubidots API
 * @arg id the id where you will get the data
 * @return true uppon succes
 */

/** 
 * This function is to get variable information from the Ubidots API
 * @arg dot is pointer to struct ubi_value
 * @return dot->value, the float value of the dot you get from the Ubidots API
 * The struct dot is updated, providing access to:
 * dot->value       := (float) value of datadot
 * dot->count       := number of datadots 
 * dot->timestamp   := timestamp in SECONDS
 * dot->valid       := reading valid 
 * Example Code for Arduino:
  ubi_value ubi_struct = {"575e754d76254211xxxxxxxx", 0.0, 0L, false};  // Ubidots struct for "Switch" 
  float value;

  value=client.getValueInfo(&ubi_struct); // This is the important call

  if (ubi_struct.valid){  // Ubidot Value is valid
      Serial.print(" Ubi_struct: "+String(ubi_struct.value,DEC)+" Count:"+String(ubi_struct.count,DEC)+" timestamp[s]:"+String(ubi_struct.timestamp,DEC));
   } else {
     Serial.println("UBIDOTS: ERROR getDot(Dot_Switch) ");
   }
*
**/
void Ubidots::getValueInfo(ubi_value* dot) {
    String raw;
    char reply[500];
    int i = 0;
    int timeout = 3000;
    char* id = dot->id;
    uint8_t bodyPosinit;
    uint8_t bodyPosend;
    _client.connect(SERVER, PORT);
    if (_client.connected()) {
        Serial.println(F("Getting your dot"));
        // Make a HTTP request:
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
        dot->valid = false;
        return;
    }
    while (!_client.available()) { // Allow Timeout in endless loop
        delay(100);
        timeout -= 100;
        if (timeout < 0) {
            Serial.println(F("ERROR: Wating for client timed out"));
            break;
        }
    }
    while (_client.available()) {
        reply[i] = _client.read();
        i++;
        if(i >= 499) {
          i = 0;
          break;
        }
        //Serial.write(c);
    }
    _client.stop();
//  Serial.println(reply);
    char* pch;
    char* rest;
    dot->valid = true;
    pch = strstr(reply,"\"value\":");
    raw = String(pch);
    bodyPosinit = 9 + raw.indexOf("\"value\":");
    bodyPosend = raw.indexOf(", \"timestamp\"");
    raw.substring(bodyPosinit, bodyPosend);
    dot->value = raw.toFloat();

    bodyPosinit = 9 + raw.indexOf("\"count\": ");
    bodyPosend = raw.indexOf(", \"next\"");
    raw.substring(bodyPosinit, bodyPosend);
    dot->count = atof(raw.c_str());

    bodyPosinit = 13 + raw.indexOf("\"timestamp\": ");
    bodyPosend = raw.indexOf(", \"context\"");
    raw.substring(bodyPosinit, bodyPosend);
    dot->count = atof(raw.c_str());
    return;
}


float * Ubidots::getValue(char* id) {
    String raw;
    float arrayResponse[2];
    arrayResponse[0] = 0;
    arrayResponse[1] = 0;
    char reply[500];
    int i = 0;
    uint8_t bodyPosinit;
    uint8_t bodyPosend;
    _client.connect(_server, PORT);
    if (_client.connected()) {
        Serial.println(F("Geting your variable"));
        // Make a HTTP request:
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
        return arrayResponse;
    }
    int timeout = 0;
    while (!_client.available() && timeout < 5000) {
        delay(1);
        timeout++;
    }
    while (_client.available()) {
        reply[i] = _client.read();
        i++;
        if (i >= 499) {
            i = 0;
            break;
        }
    }
    _client.stop();
    arrayResponse[0] = 1;
    Serial.println(reply);
    char* pch = strstr(reply, "\"value\":");
    raw = String(pch);
    bodyPosinit = 9 + raw.indexOf("\"value\":");
    bodyPosend = raw.indexOf(", \"timestamp\"");
    raw.substring(bodyPosinit, bodyPosend).toCharArray(reply, 10);
    arrayResponse[1] = atof(reply);
    return arrayResponse;
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
    if (currentValue > MAX_VALUES) {
        Serial.println(F("You are sending more than 5 consecutives variables, you just could send 5 variables. Then other variables will be deleted!"));
        currentValue = MAX_VALUES;
    }
}
/**
 * Send all data of all variables that you saved
 * @reutrn true upon success, false upon error.
 */
bool Ubidots::sendAll() {
    String payload;
    String httpHeaders;
    String str;
    uint8_t size = 0;
    httpHeaders = "POST /api/v1.6/collections/values/?force=true HTTP/1.1\r\n";
    httpHeaders += "Host: things.ubidots.com\r\n";
    httpHeaders += "User-Agent: Arduino-WiFi/" + VERSION +"\r\n";
    httpHeaders += "X-Auth-Token: " + _token + "\r\n";
    httpHeaders += "Connection: close\r\n";
    httpHeaders += "Content-Type: application/json\r\n";
    payload = "[";
    for (int i = 0; i < currentValue; ) {
        str = String(((val+i)->value_id), 2);
        payload += "{\"variable\": \"{" + (val + i)->id +"}\", \"value\":"+ str +"}";
        i++;
        if (i < currentValue) {
            payload += ", ";
        }
    }
    payload += "]\r\n";
    size = payload.length();
    httpHeaders += "Content-Length: "+ String(size) +"\r\n\r\n";
#ifdef META_DEBUG
    Serial.println("The full HTTP is: ");
    Serial.print(httpHeaders);
    Serial.println(payload);
    httpHeaders += payload;
#endif

    if (_client.connect(_server, PORT)) {
        Serial.println(F("The TCP socket is opened"));
        _client.println(httpHeaders)
        _client.flush();
    }
    int timeout = 0;
    delay(100);
    while (!_client.available() && timeout < 5000) {
        delay(1);
        timeout++;
    }
    while (_client.available()) {
        char c = _client.read();
        Serial.write(c);
    }
    currentValue = 0;
    _client.stop();
    return true;
}
