# Ubidots Arduino WiFi 

Ubidots-Arduino is an Arduino library for interacting with Ubidots through its API. 
The library also contains the code to connect to your WiFi network.

Note: This code has been tested ONLY with the official Arduino WiFi Shield: http://arduino.cc/en/Main/ArduinoWiFiShield
We can't guarantee that it will work with different WiFi modules.

## Requiremets

* [Arduino UNO](https://www.arduino.cc/en/Main/ArduinoBoardUno)
* [Arduino WiFi Shield](https://www.arduino.cc/en/Main/ArduinoWiFiShield)
* [Arduino IDE 1.6.5 or higher](https://www.arduino.cc/en/Main/Software)
* [UbidotsArduino library](https://github.com/ubidots/ubidots-arduino-wifi/archive/master.zip)
* []()
## Setup


1. Download the UbidotsArduino library [here](https://github.com/ubidots/ubidots-arduino-wifi/archive/master.zip)
2. Go to the Arduino IDE, click on **Sketch -> Include Library -> Add .ZIP Library**
3. Select the .ZIP file of Ubidots_FONA and then "Accept" or "Choose"
4. Do the same to add the Adafruit_FONA library.
5. Close the Arduino IDE and open it again.

<aside class="alert">
Use the Arduino 1.6.5 version or newer with the Arduino YÚN.
</aside>
<aside class="warning">
NOTE: this product is currently retired by Arduino and the documentation will not be kept up-to-date 
</aside>
    
## Send one value to Ubidots

To send a value to Ubidots, go to **Sketch -> Examples -> UbidotsArduino library** and select the "saveValue" example. 
Put your Ubidots token and variable ID where indicated, as well as the WiFi settings.
Upload the code, open the Serial monitor to check the results. If no response is seen, try unplugging your Arduino and then plugging it again. Make sure the baud rate of the Serial monitor is set to the same one specified in your code.

```c++

#include <SPI.h>
#include <WiFi.h>

#include <UbidotsArduino.h>
#define ID  "Your_variable_ID_here"  // Put here your Ubidots variable ID
#define TOKEN  "Your_token_here"  // Put here your Ubidots TOKEN


char ssid[] = "yourNetwork"; //  your network SSID (name)
char pass[] = "secretPassword";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

Ubidots client(TOKEN);

void setup(){
    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    // check for the presence of the shield:
    if (WiFi.status() == WL_NO_SHIELD) {
        Serial.println("WiFi shield not present");
        // don't continue:
        while (true);
    }

    String fv = WiFi.firmwareVersion();
    if (fv != "1.1.0") {
        Serial.println("Please upgrade the firmware");
    }

    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, pass);

        // wait 10 seconds for connection:
        delay(10000);
    }
        
}
void loop(){
        float value = analogRead(A0);
        client.add(ID, value);
        client.sendAll();
}
```


## Get one value from Ubidots

To get the last value of a variable from Ubidots, go to **Sketch -> Examples -> UbidotsArduino library** and select the "getValue" example. 
Put your Ubidots token and variable ID where indicated, as well as the WiFi settings.
Upload the code, open the Serial monitor to check the results. If no response is seen, try unplugging your Arduino and then plugging it again. Make sure the baud rate of the Serial monitor is set to the same one specified in your code.

```c++

#include <SPI.h>
#include <WiFi.h>

#include <UbidotsArduino.h>
#define ID  "Your_variable_ID_here"  // Put here your Ubidots variable ID
#define TOKEN  "Your_token_here"  // Put here your Ubidots TOKEN


char ssid[] = "yourNetwork"; //  your network SSID (name)
char pass[] = "secretPassword";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

Ubidots client(TOKEN);

void setup(){
    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    // check for the presence of the shield:
    if (WiFi.status() == WL_NO_SHIELD) {
        Serial.println("WiFi shield not present");
        // don't continue:
        while (true);
    }

    String fv = WiFi.firmwareVersion();
    if (fv != "1.1.0") {
        Serial.println("Please upgrade the firmware");
    }

    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, pass);

        // wait 10 seconds for connection:
        delay(10000);
    }
        
}
void loop(){
    float *responseArray;
    float value;
    responseArray = client.getValue(ID);
    // The library return a verify float in the first position, 0 when
    // it doesn't get the value and 1 when it get the value
    // in the second position is the value 
    if (responseArray[0] == 1) {
        value = responseArray[1];
    }
}
```
