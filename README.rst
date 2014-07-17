Ubidots-Arduino
===============

Ubidots-Arduino is an Arduino library for interacting with Ubidots through its API. 
The library also contains the code to connect to your WiFi network.

Note: This code has been tested ONLY with the official Arduino WiFi Shield: http://arduino.cc/en/Main/ArduinoWiFiShield
We can't guarantee that it will work with different WiFi modules.

Install the Library
-------------------

1. Download the library as a ZIP file here: https://github.com/ubidots/ubidots-arduino/blob/master/Library/Ubidots.zip?raw=true

2. Open the Arduino IDE and go to "Sketch" --> "Import Library" --> "Add Library".

3. Select the downloaded ZIP file

4. Restart the Arduino IDE, you should then see Ubidots examples under "File" --> "Examples" --> "Ubidots"


A Quick example
----------------
Here's a quick example of how to use the library, with the serial terminal of Arduino:


.. code-block:: cpp

   /*
      Wifi Ubidots serial Client
 
      This sketch connect to Ubidots (http://www.ubidots.com)
      using an Arduino Wifi shield.
 
      This example is written for a network using WPA encryption. You only need your SSID and PASS.
 
      This example has new library for Arduino WiFi Shield, for the 1.6 API version.
      With the get_value fuction you can get the value of the variable, and with save_value you
      can post on Ubidots in real time.
 
      Circuit:
      * Arduino Wifi shield
      * Serial Monitor of Arduino 
 
      Created 9 Jun 2014
      Modified 17 Jun 2014
      by Mateo Vélez
 
      This code is in the public domain.
 
    */
    #include <WiFi.h>
    #include <Ubidots.h>
    char ssid[] = "Atom House Medellin";                                                 //your network SSID (name) 
    char pass[] = "atommed2014";                                                         //your network password (use for WPA, or use as key for WEP)
    String api = "5ca9b10038e49e0492c6794f9043f0918ddcbd26";                             //your API Key number
    String idvari = "53baaf3c76254244e1c8e408";                                          //the number of the Ubidots variable
    String ctext = ", \"context\":{\"color\":\"blue\",\"status\":\"active\"}}";
    Ubidots ubiclient(api);                                                              //with that you call the api with the prefix ubiclient

    void setup()
    {
       boolean response;                                                                 //you need boolean variable to save value from WifiCon fuction, (True, False)
       int status = WL_IDLE_STATUS;                                                      //we need to difine first a WL_IDLE_STATUS for the red
       Serial.begin(9600);                                                               //9600 baud for serial transmision
       response = ubiclient.WifiCon(ssid,pass,status,api);                               //this fuction is for connect to your wifi red
       Serial.println(response);                                                         //print in the Serial Monitor
    }
    void loop()
    {
      int incomingByte = 100;                                                            //this variable is for send or receive value in ubidots
      String readvalue = "";
       if (Serial.available()>0)
      {
        Serial.println("Enter 0 to get the data, or any number between 1 and 9 to post it to Ubidots");
        incomingByte = Serial.read()-48;                                                 //how incomingByte is integer, you need change this value to ascii number then you need rest 48
        if (incomingByte>0)
        {
           if (ubiclient.save_value(idvari,String(incomingByte),ctext))                  //this fucion is for post on ubidots, and return True or False depending on if the communication is right
           {
             Serial.println("Ok");                                                       //print OK in the monitor when the value is changed in ubidots
           }
        }
        if (incomingByte == 0)
        {
         readvalue = ubiclient.get_value(idvari);                                        //this fuction is for get value from ubidots
         Serial.println(readvalue);                                                      //print the value in the serial monitor
        }
      }
    }


Please pay close attention to the format of ctext, 

.. code-block:: cpp

   String ctext=",  \"context\":{\"color\":\"blue\",\"status\":\"active\"}}";

It is required to send "}",  or the function will not work , the format of the content text is  ", \"context\":{}}" inside the brackets you can add the information to the feature you want, as long as you follow the format of the example.

API Reference
-------------

Ubidots ubiclient():
````````````````````
.. code-block:: cpp

    Ubidots ubiclient(api);

=======  ========  =================================
Type     Argument  Description
=======  ========  =================================
String   api       Your API key for the Ubidots API
=======  ========  =================================

Initialize a Ubidots client. This is most likely to be the first Ubidots library function to call.

save_value()
````````````````````
.. code-block:: cpp

    boolean=ubiclient.save_value(idvari,String(incomingByte),ctext)
=======  ============  =================================
Type     Argument      Description
=======  ============  =================================
String   idvari        ID of the variable to save
String   incomingByte  The value of the sensor
String   ctext         Content text of the value
=======  ============  =================================

Save a value to Ubidots. Returns true upon success. Returns false upon error.

get_value()
```````````
.. code-block:: cpp

    readvalue=ubiclient.get_value(idvari);

==================  ===========  =============================================
Type                Argument     Description
==================  ===========  =============================================
String              idvari       ID of the variable that you want make request 
==================  ===========  =============================================

Get value from Ubidots. Returns a String containing the last value of the variable.

WifiCon()
`````````
.. code-block:: c

    response=ubiclient.WifiCon(ssid,pass,status,api);

==============  ===========  =================================================
Type            Argument     Description
==============  ===========  =================================================
String          ssid         The SSID of your WiFi network
String          pass         The pass of your WiFi network
String          status       is the value of the initialization status of WiFi
String          api          Your API Key number
==============  ===========  =================================================

This Function is to connect to your WiFi network, after connection it creates a token using the API key. 
Returns a boolean (true or false) depending on whether the token is obtained or not. 
