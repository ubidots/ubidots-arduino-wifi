# [DEPRECATED] Ubidots Arduino WiFi

**WARNING: **The [Arduino Wi-Fi shield](https://store.arduino.cc/usa/arduino-wifi-shield) was retired from commercial sales markets in 2015; for this reason the library is **"DEPRECATED"**

# HTTP requests to Ubidots

To send **HTTP requests** to [Ubidots](https://ubidots.com/) using the Arduino WiFi Shield please follow the below steps:
1. Open the Arduino IDE. Go to **File > Examples > WiFi > WiFiWebClient**.
2. Assign your WiFi crendetials were is indicated.
3. Upload the code into the Arduino UNO. Before upload the code make sure to select the right COM Port from **Tools > Port > Port Assigned**.
4. Once the code is uploaded, open the **Serial Monitor** to verify the response of the sample request.
5. If everything is working properly, modify the sample code handling the request to Ubidots. Please, reference to the [REST API Reference](https://ubidots.com/docs/api/) to build the HTTP request.

**NOTE**: For any additional assistance, please reference to [Ubidots Community](http://community.ubidots.com/).
