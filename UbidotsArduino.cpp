#include "UbidotsArduino.h"
/**
 * Constructor.
 */
Ubidots::Ubidots(char* token){
    _token = token;
    maxValues = 4;
    currentValue = 0;
    val = (Value *)malloc(maxValues*sizeof(Value));
    status = WL_IDLE_STATUS;
}
/** 
 * This function is to get value from the Ubidots API
 * @arg id the id where you will get the data
 * @return num the data that you get from the Ubidots API
 */
float Ubidots::getValue(char* id){
  float num;
  int i = 0;
  String raw;
  uint8_t bodyPosinit;
  uint8_t bodyPosend;
  char reply[15];
  _client.connect(SERVER, PORT);
  if (_client.connected()){
        Serial.println(F("Geting your variable"));
        // Make a HTTP request:
        _client.print(F("GET /api/v1.6/variables/"));
        _client.print(id);
        _client.println(F("/values?page_size=1 HTTP/1.1"));
        _client.println(F("Host: things.ubidots.com"));
        _client.println(F("User-Agent: Arduino-WiFi/1.0"));
        _client.print(F("X-Auth-Token: "));
        _client.println(_token);
        _client.println(F("Connection: close"));
        _client.println();
    } else {
        Serial.println(F("Connection failed"));  
        currentValue = 0;  
        return NULL;
    }
    while (!_client.available());
    while (_client.available()){
        char c = _client.read();
        Serial.write(c);
    }
    Serial.println();
    num = atof(reply); 
    _client.stop();
    return num;
}
/**
 * Add a value of variable to save
 * @arg variable_id variable id to save in a struct
 * @arg value variable value to save in a struct
 */
void Ubidots::add(char *variable_id, double value){
  (val+currentValue)->id = variable_id;
  (val+currentValue)->value_id = value;
  currentValue++;
  if(currentValue>maxValues){
    Serial.println(F("You are sending more than 5 consecutives variables, you just could send 5 variables. Then other variables will be deleted!"));
    currentValue = maxValues;
  }
}
/**
 * Send all data of all variables that you saved
 * @reutrn true upon success, false upon error.
 */
bool Ubidots::sendAll(){
    int i;
    char vals[10];
    char mess[800];
    char data[250];
    String message;  
    sprintf(data,"[");
    for(i=0; i<currentValue;){
      dtostrf((val + i)->value_id, 10, 3, vals);
      sprintf(data, "%s{\"variable\": \"%s\", \"value\":\"%s\"}", data, (val + i)->id, vals);
      i++;
      if(i<currentValue){
        sprintf(data,"%s, ", data);
      }
    }
    sprintf(data, "%s]", data);
    sprintf(mess,"POST /api/v1.6/collections/values/?force=true HTTP/1.1\nHost: things.ubidots.com\nUser-Agent: User-Agent: Arduino-Ethernet/1.0\nX-Auth-Token: %s\nConnection: close", _token);
    
    sprintf(mess,"%s\nContent-Type: application/json\nContent-Length:  %d\n\n%s\n",mess, strlen(data), data);
      _client.connect(SERVER, PORT);
    if (_client.connected()){
          Serial.println(F("Posting your variables"));
          // Make a HTTP request:
          _client.println(mess);
          
    }else{
          Serial.println(F("Connection failed"));  
          currentValue = 0;  
          return NULL;
    }
    while (!_client.available());
    while (_client.available()){
        char c = _client.read();
        Serial.write(c);
    }
    currentValue = 0;
    return true;    
}
