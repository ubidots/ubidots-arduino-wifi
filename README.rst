Ubidots-Arduino
===============

Ubidots-Arduino is an Arduino library for interacting with Ubidots through its API. 
The library also contains the code to connect to your WiFi network.

Note: This code has been tested ONLY with the official Arduino WiFi Shield: http://arduino.cc/en/Main/ArduinoWiFiShield
We can't guarantee that it will work with different WiFi modules.

A Quick example
----------------
Here's a quick example of how to use the library, with the serial terminal of Arduino:


.. code-block:: cpp

   #include <WiFi.h>
   #include <Ubidots.h>

    char ssid[] = "Atom House Medellin";                    //  your network SSID (name) 
    char pass[] = "atommed2014";                            // your network password (use for WPA, or use as key for WEP)
    String api="5ca9b10038e49e0492c6794f9043f0918ddcbd26";  //your API Key number
    String idvari = "53baaf3c76254244e1c8e408";             // The number of the Ubidots variable
    String ctext=", \"context\":{\"color\":\"blue\",\"status\":\"active\"}}";
    Ubidots ubiclient(api);                                 // with that you call the api with the prefix ubiclient

    void setup()
    {
     boolean response;                                   // You need boolean variable to save value from WifiCon function (True or False)
     int status = WL_IDLE_STATUS;                        // we need to define first a WL_IDLE_STATUS for the network
     Serial.begin(9600);                                 // 9600 baud for serial transmision
     response=ubiclient.WifiCon(ssid,pass,status,api);   // this function is to connect to your wifi network
     Serial.println(response);                           //print in the Serial Monitor
    }
    void loop()
    {
    int incomingByte = 100;                                  // this variable is to send or receive a value in ubidots
    String readvalue="";
     if (Serial.available()>0)
    {
      incomingByte = Serial.read() - 48;                     // Because incomingByte is integer, you need to change this value to an ascii number and then substract 48
      if (incomingByte>0)
      {
         if (ubiclient.save_value(idvari,String(incomingByte))) //this function is to post the value to Ubidots, and return True or False if the communication is successful
         {
           Serial.println("Ok");                            // print OK in the monitor when the value is changed in ubidots
         }
      }
      if (incomingByte==0)
      {
       readvalue=ubiclient.get_value(idvari);               // this function is to get a value from ubidots
       Serial.println(readvalue);                           // print the value in the serial monitor
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
