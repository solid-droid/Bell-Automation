#include <ESP8266WiFi.h>
#include <EEPROM.h>
char inSerial[15];
char* ssid;
char* password;
String dat;
int x=0; 
int ledPin = 1; 
boolean new_net=0;
String request;
boolean value = LOW;
 
WiFiServer server(80);
WiFiClient client;

void setup() {
  Serial.begin(115200);
   pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  delay(1000);
 digitalWrite(ledPin, LOW);
  delay(1000);
  digitalWrite(ledPin, HIGH);
Serial.println("Control Prompt");
control_prompt();

    
}


 
void loop() {

  
 client_req();

 
 
  if (request.indexOf("/LED=ON") != -1) 
  {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  } 
  if (request.indexOf("/LED=OFF") != -1)
  {
    digitalWrite(ledPin, LOW);
    value = LOW;
  }
 


home_pg();

  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
}






