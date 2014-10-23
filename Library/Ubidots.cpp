/*
Ubidots.cpp -El objetivo de la librería es realizar la conexión con la plataforma ubidots, para que los usuarios puedan postear o escribir su información 
en la nube

*/
/*
Ubidots Library with get_value, save_value functions, for  users to be able post or read the information in the Cloud


Created 9 Jun 2014
Modified 9 Jun 2014
by Mateo Vélez

*/

#include "arduino.h"
#include "Ubidots.h"

#include <WiFi.h>
#include <String.h>
//----------------------------------------------------------------------------------------
//--------------------------------------Ready to use the API------------------------------
//----------------------------------------------------------------------------------------
Ubidots::Ubidots(String apikey)
{

  _apikey = apikey;
  _token="";
  _value=" ";
  _mensaje=" ";
  _le="";
  _var=" ";
}
//----------------------------------------------------------------------------------------
//-----------------------------------Get Value from Ubidots!------------------------------
//----------------------------------------------------------------------------------------

/*This functions is for get value from Ubidots, you need send the id variable that you want
to ask to the server.

The function discriminates the value of the variable using a state machine, this in order 
to avoid many changes in each new version of the API.

*/
String Ubidots::get_value(String idvariable)
{
	
	int flag=0;
  _value="";
  resetTimer=0;
  char TOKEN[9]={'"','v','a','l','u','e','"',':',' '};
	char _server[]="things.ubidots.com";
	while (_value=="")
	{
    
		if (_client.connect(_server, 80)) {
		
			_client.println("GET /api/v1.6/variables/"+idvariable+" HTTP/1.1");
			_client.println("X-Auth-Token: "+_token);
			_client.println("Host: things.ubidots.com");
			_client.println();    
			
      while (!_client.available());
			while (_client.available()) {
        _c = _client.read();

     
			  //this switch is for select the value, is manually cuz if the api change in the future the
        //code will not change	
         if((TOKEN[flag]==_c)&&(flag<9))
        {
          flag++;
        }
         else if(((_c>47)&&(_c<58)||(_c=='.')||(_c=='}'))&&(flag>8))
        {
          

          if(_c=='}')
          {

            break;
          }
          else
          {
            _value+=_c;
          flag++;
          }
        }
        else
        {
          flag=0;
        }  
				
       }
       break;		   
		}
  }	  
	 _client.flush(); // Clean the buffer
   _client.stop(); // Stop the communication
   return _value;
  
}

//----------------------------------------------------------------------------------------
//------------------------------------POST Value to Ubidots!------------------------------
//----------------------------------------------------------------------------------------

/*This function is to post the variable value to Ubidots, you need send the id of the variable
and the value that you want post on the server.


The function returns  true or false depending on whether the posting is completed or not
*/
boolean Ubidots::save_value(String idvariable,String valor)
{
	String _mensaje PROGMEM=" ";
  int num=0;
  _var="{\"value\":"+ valor + "}";
  num=_var.length();
  _le=String(num);
	String _value="";
	int flag2=0;
  char Http[9]={'H','T','T','P','/','1','.','1',' '};
  resetTimer=0;
	char _server[]="things.ubidots.com";
	//valorstr(valor,ctext);
	while (_value!="201")
	{	
		if (_client.connect(_server, 80)) {
			_client.println("POST /api/v1.6/variables/"+idvariable+"/values HTTP/1.1");
      _client.println("Content-Type: application/json");
      _client.println("Content-Length: "+_le);
      _client.println("X-Auth-Token: "+_token);
      _client.println("Host: things.ubidots.com");
      _client.println();
      _client.println(_var); // \"context\":{" + ctext + "}}
      _client.println();                  
			while (!_client.available());
      while (_client.available()) {
        _c = _client.read();

       // Serial.print(_c);
//------------------------------------------------------------------------
//-------------------"HTTP/1.1" is for get the number-------------- 
//------------------------------------------------------------------------

/*
check character by character with the string, if is different falg2 restarts

*/
  if((Http[flag2]==_c)&&(flag2<9))
  {
    flag2++;
  }
  else if((flag2>8)&&(flag2<12))
  {
    _value+=_c;
    flag2++; 
  }
  else
  {
    flag2=0;
  } 
//------------------------------------------------------------------------
//------------------------------------------------------------------------
              }
            }
    _client.flush();
    _client.stop();
    
    if (_value=="500")
    {
    return false;
    break;
    }
    } 
  if (_value=="201" || _value=="200")
  {
  return true;
  }
  else
  {
  return false;
  }

}

boolean Ubidots::save_value(String idvariable,String valor, String ctext)
{
  String _mensaje PROGMEM=" ";
  int num=0;
  _var="{\"value\":"+ valor;
  ctext=", \"context\":" + ctext + "}";
  num=_var.length()  +  ctext.length();
  _le=String(num);
  String _value="";
  int flag2=0;
  char Http[9]={'H','T','T','P','/','1','.','1',' '};
  resetTimer=0;
  char _server[]="things.ubidots.com";
  //valorstr(valor,ctext);
  while (_value!="201")
  { 
    if (_client.connect(_server, 80)) {
      _client.println("POST /api/v1.6/variables/"+idvariable+"/values HTTP/1.1");
      _client.println("Content-Type: application/json");
      _client.println("Content-Length: "+_le);
      _client.println("X-Auth-Token: "+_token);
      _client.println("Host: things.ubidots.com");
      _client.println();
      _client.print(_var); // \"context\":{" + ctext + "}}
      _client.println(ctext);
      _client.println();                  
      while (!_client.available());
      while (_client.available()) {
        _c = _client.read();

       // Serial.print(_c);
//------------------------------------------------------------------------
//-------------------"HTTP/1.1" is for get the number-------------- 
//------------------------------------------------------------------------

/*
check character by character with the string, if is different falg2 restarts

*/
  if((Http[flag2]==_c)&&(flag2<9))
  {
    flag2++;
  }
  else if((flag2>8)&&(flag2<12))
  {
    _value+=_c;
    flag2++; 
  }
  else
  {
    flag2=0;
  } 
//------------------------------------------------------------------------
//------------------------------------------------------------------------
              }
            }
    _client.flush();
    _client.stop();
    
    if (_value=="500")
    {
    return false;
    break;
    }
    } 
  if (_value=="201" || _value=="200")
  {
  return true;
  }
  else
  {
  return false;
  }

}
//----------------------------------------------------------------------------------------
//-------------------------Create a Token for your variable!------------------------------
//----------------------------------------------------------------------------------------

/*This function is to create a token for your connection with the server.


The function returns  true or false depending on whether the connection  is completed or not

*/
boolean Ubidots::ubitoken()
{
	int flag=0,flag2=0;
  char Http[9]={'H','T','T','P','/','1','.','1',' '};
  char TOKEN[9]={'t','o','k','e','n','"',':',' ','"'};
  resetTimer=0;
  _mensaje="";
  _token="";
	_value="";
	char _server[]="things.ubidots.com";
	if (_client.connect(_server, 80)) {
    _client.println("POST /api/v1.6/auth/token HTTP/1.1");
    _client.println("X-Ubidots-ApiKey: "+_apikey);
    _client.println("Host: things.ubidots.com");
    _client.println();
//-----------------------------------------------------------------------------------------
    while (!_client.available());     
    while (_client.available()) {
    _c = _client.read();
    Serial.print(_c);
    
    
//---------------------------------------------------------------------------------------------
//-------------------------- "HTTP/1.1" is for get the number----------------------------
//---------------------------------------------------------------------------------------------
/*
check character by character with the string, if is different falg2 restarts

*/
  if((Http[flag2]==_c)&&(flag2<9))
  {
    flag2++;
  }
  else if((flag2>8)&&(flag2<12))
  {
    _value+=_c;
    flag2++; 
  }
  else
  {
    flag2=0;
  } 
//---------------------------------------------------------------------------------------------
//-----------------------------"token": is for get the token----------------------------
//---------------------------------------------------------------------------------------------
  /*
check character by character with the string, if is different falg restarts

*/
    if((TOKEN[flag]==_c)&&(flag<9))
  {
    flag++;
  }
  else if((_c>47)&&(_c<123)&&(flag>8))
  {
    _token+=_c;
    flag++; 
  }
  else
  {
    flag=0;
  }     
     }
  } 
  _client.flush();
  _client.stop(); 
 
   if (_value=="201" || _value=="200")
  { 
  return true;  
  }
  else
  {
  return false;
  }
 }

//----------------------------------------------------------------------------------------
//------------------------------------Function of save_value-------------------------------
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
//-------------------------------------WiFi data serial Post------------------------------
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
//-------------------------------------WiFi data serial Post------------------------------
//----------------------------------------------------------------------------------------
void Ubidots::printWifiStatus() 
{  
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
 
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
//----------------------------------------------------------------------------------------
//-----------------------------------Connection to your WiFi------------------------------
//----------------------------------------------------------------------------------------
boolean Ubidots::WifiCon(char *ssid,char *pass,int status, String api)
{
  boolean response=0;  
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);
  }   
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);        
    status = WiFi.begin(ssid, pass);  
  } 
  Serial.println("Connected to wifi");
  printWifiStatus();  
  Serial.println("\nStarting connection to server...");
  response=ubitoken();
  //Checkpoint for token request
  while(!response)
  {
    Serial.println("\nConnection failed, restarting connection...");
    response=ubitoken();
  }  

  return response;  
}


