/* #include <Arduino.h>
#include "Wire.h"
#include "ArduinoNunchuk.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>


const char* ssid ="Robot_01";
const char* pass ="letmeaccessyourdata";

unsigned int localPort = 2000; // local port to listen for UDP packets
 
IPAddress ServerIP(192,168,4,1);
IPAddress ClientIP(192,168,4,2);
 
// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;
 
char packetBuffer[9];   //Where we get the UDP data

ArduinoNunchuk nunchuk = ArduinoNunchuk();


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  WiFi.begin(ssid, pass);   //Connect to access point
  //WiFi.config(ClientIP, ServerIP, (255,255,255,0));
  Serial.println("");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("IP Roboter: ");
  Serial.println(WiFi.gatewayIP());
    
  //Start UDP
  Serial.println("Starting UDP");
  Serial.println(udp.begin(localPort));
  Serial.print("Local port: ");
  Serial.println(udp.localPort());
  nunchuk.init();
}

void loop() {
  delay(100);
  // put your main code here, to run repeatedly:
  nunchuk.update();
  Serial.print(nunchuk.analogX); // X links rechts
  Serial.print("    ");
  Serial.println(nunchuk.analogY);



  //byte sensorBuffer [2] = { lowByte(nunchuk.analogX),  lowByte(nunchuk.analogY)};
  //Serial.print(sensorBuffer[0]);
  //Serial.print(' ');
  //Serial.println(sensorBuffer[1]);
  udp.beginPacket(WiFi.gatewayIP(), 2000);
  if(nunchuk.analogX >150)
  {
    if (nunchuk.analogY > 150) 
    {
      udp.print('4');
    }
    else if(nunchuk.analogY < 100)
    {
      udp.print('6');
    }
    else
    {
      udp.print('5');
    }    
  }
  else if(nunchuk.analogX < 100)
  {
    if (nunchuk.analogY > 150) 
    {
      udp.print('2');
    }
    else if(nunchuk.analogY < 100)
    {
      udp.print('8');
    }
    else
    {
      udp.print('1');
    } 
  }
  else
  {
    if (nunchuk.analogY > 150) 
    {
      udp.print('3');
    }
    else if(nunchuk.analogY < 100)
    {
      udp.print('7');
    }
    else
    {
      udp.print('0');
    } 
  }

  //udp.write(sensorBuffer,2);
  //udp.print(sensorBuffer[0]);
  //udp.print(' ');
  //udp.print(sensorBuffer[1]);
  udp.endPacket();
  udp.flush();
  yield();

}
*/

#include "Wire.h"
#include "ArduinoNunchuk.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "Arduino.h" 
const char* ssid ="Robot_01";
const char* pass ="letmeaccessyourdata";

void initNetwork();

unsigned int localPort = 2000; // local port to listen for UDP packets

IPAddress ServerIP(192,168,4,255);
//IPAddress ServerIP(192,168,43,1);
//IPAddress ClientIP(192,168,4,2);
 
// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;
 
char packetBuffer[1];   //Where we get the UDP data
ArduinoNunchuk nunchuk = ArduinoNunchuk();

void setup()
{
  Serial.begin(115200);
    Serial.println();
 
  initNetwork();
  
  
  //Start UDP
  Serial.println("Starting UDP");
  udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());

   nunchuk.init();//D1 = SDA D2 = SCL
}

void loop()
{
  nunchuk.update();

 // Serial.print(nunchuk.analogX, DEC);
 // Serial.print(' ');
 // Serial.print(nunchuk.analogY, DEC);
 // Serial.print(' ');
 // Serial.print(nunchuk.accelX, DEC);
 // Serial.print(' ');
 // Serial.print(nunchuk.accelY, DEC);
 // Serial.print(' ');
 // Serial.print(nunchuk.accelZ, DEC);
 // Serial.print(' ');
 // Serial.print(nunchuk.zButton, DEC);
 // Serial.print(' ');
 // Serial.println(nunchuk.cButton, DEC);

char sensorBuffer;
if ((nunchuk.analogX == 127) && (nunchuk.analogY == 128)){sensorBuffer = '0' ;}
if ((nunchuk.analogX == 127) && (nunchuk.analogY > 150)){sensorBuffer = '1' ;}
if ((nunchuk.analogX > 150) && (nunchuk.analogY == 128)){sensorBuffer = '2' ;}
if ((nunchuk.analogX == 127) && (nunchuk.analogY < 100)){sensorBuffer = '3' ;}
if ((nunchuk.analogX < 100) && (nunchuk.analogY == 128)){sensorBuffer = '4' ;}
if ((nunchuk.analogX > 180) && (nunchuk.analogY  > 180)){sensorBuffer = '5' ;}
if ((nunchuk.analogX > 180) && (nunchuk.analogY  < 70)){sensorBuffer = '6' ;}
if ((nunchuk.analogX < 70) && (nunchuk.analogY  < 70)){sensorBuffer = '7' ;}
if ((nunchuk.analogX < 70) && (nunchuk.analogY  > 180)){sensorBuffer = '8' ;}
if ((nunchuk.cButton) ){sensorBuffer = 'c' ;}
if ((nunchuk.zButton)){sensorBuffer = 'z' ;}
  

Serial.println(sensorBuffer);
 udp.beginPacket(ServerIP, 2000);
 udp.write(sensorBuffer);
 udp.endPacket();
 //udp.flush();

delay(50);
}

void initNetwork()
{
  WiFi.softAP(ssid, pass);
  Serial.println(udp.begin(localPort));
  Serial.println(WiFi.localIP());
}