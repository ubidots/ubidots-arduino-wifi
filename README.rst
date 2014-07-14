Ubidots-Arduino
===============

Ubidots-Arduino is a Arduino library for interacting with Ubidots_ through its API. The library also contains the code for connection to your WiFi



A Quick example
----------------
A quick exaple of how to use the library, with the serial terminal of Arduino.


.. code-block:: cpp

#include <WiFi.h>
#include <Ubidots.h>

char ssid[] = "Atom House Medellin"; //  your network SSID (name) 
char pass[] = "atommed2014";    // your network password (use for WPA, or use as key for WEP)
String api="5ca9b10038e49e0492c6794f9043f0918ddcbd26"; //your API Key number
String idvari = "53baaf3c76254244e1c8e408"; // The number of the Ubidots variable
String ctext=", \"context\":{\"color\":\"blue\",\"status\":\"active\"}}";
Ubidots ubiclient(api);// with that you call the api with the prefix ubiclient

void setup()
{
   boolean response; //You need boolean variable to save value from WifiCon fuction, (True, False)
   int status = WL_IDLE_STATUS;  //   we need to difine first a WL_IDLE_STATUS for the red
   Serial.begin(9600); // 9600 baud for serial transmision
   response=ubiclient.WifiCon(ssid,pass,status,api); // this fuction is for connect to your wifi red
   Serial.println(response); //print in the Serial Monitor
}
void loop()
{
  int incomingByte = 100;// this variable is for send or receive value in ubidots
  String readvalue="";
   if (Serial.available()>0)
  {
    incomingByte=Serial.read()-48; // how incomingByte is integer, you need change this value to ascii number then you need rest 48
    if (incomingByte>0)
    {
       if (ubiclient.save_value(idvari,String(incomingByte))) //this fucion is for post on ubidots, and return True or False depending on if the communication is right
       {
         Serial.println("Ok");// print OK in the monitor when the value is changed in ubidots
       }
    }
    if (incomingByte==0)
    {
     readvalue=ubiclient.get_value(idvari); // this fuction is for get value from ubidots
     Serial.println(readvalue);// print the value in the serial monitor
    }
  }
}





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

Initialize a Ubidots client. This is most likely the first Ubidots library function you will call.

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

Get value from Ubidots. Returns String with the last value of the variable.

WifiCon()
`````````
.. code-block:: c

    response=ubiclient.WifiCon(ssid,pass,status,api);

==============  ===========  =================================================
Type            Argument     Description
==============  ===========  =================================================
String          ssid         The SSID of your WiFi red
String          pass         The pass of your WiFi red
String          status       is the value of the initialization status of WiFi
String          api          Your API Key number
==============  ===========  =================================================

This Function is to connect to your WiFi network, after connect creates a token using the API key. Returns a boolean (true or false) depending on whether the token is obtained or not. 
