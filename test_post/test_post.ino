/*
 * Demonstrate using an http server and an HTML form to control an LED.
 * The http server runs on the ESP8266.
 *
 * Connect to "http://esp8266WebForm.local" or "http://<IP address>"
 * to bring up an HTML form to control the LED connected GPIO#0. This works
 * for the Adafruit ESP8266 HUZZAH but the LED may be on a different pin on
 * other breakout boards.
 *
 * Imperatives to turn the LED on/off using a non-browser http client.
 * For example, using wget.
 * $ wget http://esp8266webform.local/ledon
 * $ wget http://esp8266webform.local/ledoff
*/
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Wire.h>        
#include "RTClib.h"
RTC_Millis rtc;

IPAddress ip(192, 168, 43,100);  
IPAddress gateway(192, 168, 43, 1); //set gateway
IPAddress subnet(255, 255, 255, 0);//set subnet
// Fill in your WiFi router SSID and password
const char* ssid = "Honor";
const char* password = "qwerty123";
const char* login_pass="hello";
MDNSResponder mdns;

ESP8266WebServer server(80);
boolean IST_status=0;
int dur=4;
int alert=16;
String time_set="Not Found";
String s,text;
String responce="Welcome";
int timeset[2];
int local[200][3];
int set_size;
String FORM_HTML =
"<!DOCTYPE html>"
"<head><title>Bell Control</title>"  
"<style>"
"@import url(https://fonts.googleapis.com/css?family=Source+Sans+Pro:200,300);"
"* { box-sizing: border-box; margin: 0; padding: 0; font-weight: 300;}"
"body { font-family: 'Source Sans Pro', sans-serif; color: white; font-weight: 300;}"
".wrapper { background: #50a3a2; background: linear-gradient(to bottom right, #50a3a2 0%, #53e3a6 100%); position: absolute; width: 100%; height: 100%; overflow: hidden;}"
".contain { padding-left: 5%; padding-top: 2%;}"
"form button { -webkit-appearance: none; -moz-appearance: none; appearance: none; outline: 0; background-color: white; border: 0; padding: 10px 15px; color: #53e3a6;"
 " border-radius: 3px; width: 250px; cursor: pointer; font-size: 18px; transition-duration: 0.25s;}"
"form button:hover { background-color: #f5f7f9;}"
"</style>"
"</head>"
"<body>"

  "<div class=\"wrapper\"><div class=\"contain\"> <h1>Admin Control Panel</h1> "
  "<h3 id=\"ist\">Current Time:</h3><br>"
        "<form class=\"form\" id=\"myForm\" action=\"/\" method=\"POST\">"
       " Time <input type=\"time\" name=\"time\">"
   "Day <select name= \"date\">"
   "<option value=\"7\">Everyday</option><option value=\"1\">Monday</option><option value=\"2\">Tuesday</option>"
    "<option value=\"3\">wednesday</option><option value=\"4\">Thursday</option><option value=\"5\">Friday</option><option value=\"6\">Saturday</option>"
   " <option value=\"0\">Sunday</option></select><br><br>"
       " Duration <input type=\"text\" name=\"dur\" value=\"4\"  style=\"text-align:center;\" placeholder=\"4\" maxlength=\"2\" size=\"1\"> sec<br><br>"
       " Manual <input type=\"checkbox\" name=\"M\" value= \"1\" checked><br><br>"
       " Update <input type=\"radio\" name=\"U\" value= \"1\" checked>"
       " Delete <input type=\"radio\" name=\"U\" value= \"0\">"
       "<input type=\"hidden\" name=\"hour\" id=\"hour\">"
       "<input type=\"hidden\" name=\"minute\" id=\"minute\">"
       "<input type=\"hidden\" name=\"second\" id=\"second\">"
       "<input type=\"hidden\" name=\"day\" id=\"day\">"
       "<br><br>"
       " <button onclick=\"sub()\" id=\"login-button\">Submit</button>"
    "</form><br><br>"
    "Notification : <p id=\"notification\"><br><br>"
    "<h1>Current Time Set</h1><h3 id=\"data\">Not Found<h3><br>"
    "</div></div>"
"</body>"
"<script>"
    "var day=\"none\";"
    "var d = new Date();"
    "var n = d.getDay();"
    "switch(n)"
    "{ case 0: day=\"Sunday\";break;"
     "  case 1: day=\"Monday\";break;"
     "  case 2: day=\"Tuesday\";break;"
     "  case 3: day=\"Wednesday\";break;"
     "  case 4: day=\"Thursday\";break;"
     "  case 5: day=\"Friday\";break;"
     "  case 6: day=\"Saturday\";break;}"  
    "document.getElementById(\"day\").value =n;" 
    "var currentTime = new Date();"
    "var currentOffset = currentTime.getTimezoneOffset();"
    "var ISTOffset = 330;"
    "var ISTTime = new Date(currentTime.getTime() + (ISTOffset + currentOffset)*60000);"
    "var hoursIST = ISTTime.getHours();"
    "var minutesIST = ISTTime.getMinutes();"
    "var secondIST = ISTTime.getSeconds();"
    "document.getElementById(\"hour\").value =hoursIST;"
    "document.getElementById(\"minute\").value =minutesIST;"
    "document.getElementById(\"second\").value =secondIST;"
    "function sub()"
    "{"
    "hoursIST = ISTTime.getHours();"
    "minutesIST = ISTTime.getMinutes();"
    "secondIST = ISTTime.getSeconds();"
    "document.getElementById(\"hour\").value =hoursIST;"
    "document.getElementById(\"minute\").value =minutesIST;"
    "document.getElementById(\"second\").value =secondIST;"
    "document.getElementById(\"myForm\").submit(); "
    "}"
 "document.getElementById(\"ist\").innerHTML =\"Current Time \"+\"(\"+hoursIST+\" : \"+minutesIST+\" : \"+secondIST+\")\"+\" \"+day+\" --Submit current time & day\";"
"</script>"
"<script>"
  "var x = setInterval(function() {loadData(\"data.txt\",updateData)}, 1000);"
  "function loadData(url, callback){"
"var xhttp = new XMLHttpRequest();"
"xhttp.onreadystatechange = function(){"
 "if(this.readyState == 4 && this.status == 200){"
 "callback.apply(xhttp);"
 "}};"
"xhttp.open(\"GET\", url, true);"
"xhttp.send();}"
"function updateData(){"
 "document.getElementById(\"data\").innerHTML = this.responseText;}"
"</script>"
"<script>"
  "var x1 = setInterval(function() {loadData1(\"notification.txt\",updateData1)}, 1000);"
  "function loadData1(url, callback){"
"var xhttp1 = new XMLHttpRequest();"
"xhttp1.onreadystatechange = function(){"
 "if(this.readyState == 4 && this.status == 200){"
 "callback.apply(xhttp1);"
 "}};"
"xhttp1.open(\"GET\", url, true);"
"xhttp1.send();}"
"function updateData1(){"
 "document.getElementById(\"notification\").innerHTML = this.responseText;}"
"</script>"
"</html>";


const char INDEX_HTML[] =

"<!DOCTYPE html>"
"<html lang=\"en\" >"

"<head>"
"<title>Bell Control</title>"
"<style>"
"@import url(https://fonts.googleapis.com/css?family=Source+Sans+Pro:200,300);* {  box-sizing: border-box;  margin: 0;  padding: 0;  font-weight: 300;}"
"body {  font-family: 'Source Sans Pro', sans-serif;  color: white;  font-weight: 300;}"
".wrapper {  background: #50a3a2;  background: linear-gradient(to bottom right, #50a3a2 0%, #53e3a6 100%);  position: absolute;  top: 50%;  left: 0;  width: 100%;  height: 400px;  margin-top: -200px;overflow: hidden;}"
".wrapper.form-success .container h1 {  -webkit-transform: translateY(85px); transform: translateY(85px);}"
".container {  max-width: 600px; margin: 0 auto; padding: 80px 0; height: 400px; text-align: center;}"
".container h1 { font-size: 40px; transition-duration: 1s; transition-timing-function: ease-in-put; font-weight: 200;}"
"form {  padding: 20px 0; position: relative; z-index: 2;}"
"form input {-webkit-appearance: none; -moz-appearance: none; appearance: none; outline: 0; border: 1px solid rgba(255, 255, 255, 0.4); background-color: rgba(255, 255, 255, 0.2);"
 " width: 250px; border-radius: 3px; padding: 10px 15px; margin: 0 auto 10px auto; display: block; text-align: center; font-size: 18px; color: white; transition-duration: 0.25s;"
  "font-weight: 300;}"
"form input:hover { background-color: rgba(255, 255, 255, 0.4);}"
"form input:focus { background-color: white; width: 300px; color: #53e3a6;}"
"form button { -webkit-appearance: none; -moz-appearance: none; appearance: none; outline: 0; background-color: white; border: 0; padding: 10px 15px; color: #53e3a6;  border-radius: 3px;"
  "width: 250px;cursor: pointer; font-size: 18px; transition-duration: 0.25s;}"
"form button:hover { background-color: #f5f7f9;}"
".bg-bubbles { position: absolute; top: 0; left: 0; width: 100%; height: 100%; z-index: 1;}"
".bg-bubbles li { position: absolute; list-style: none; display: block; width: 40px; height: 40px; background-color: rgba(255, 255, 255, 0.15); bottom: -160px; -webkit-animation: square 25s infinite;"
  "animation: square 25s infinite; transition-timing-function: linear;}"
".bg-bubbles li:nth-child(1) { left: 10%;}"
".bg-bubbles li:nth-child(2) { left: 20%; width: 80px; height: 80px; -webkit-animation-delay: 2s; animation-delay: 2s; -webkit-animation-duration: 17s; animation-duration: 17s;}"
".bg-bubbles li:nth-child(3) { left: 25%; -webkit-animation-delay: 4s; animation-delay: 4s;}"
".bg-bubbles li:nth-child(4) { left: 40%; width: 60px; height: 60px; -webkit-animation-duration: 22s; animation-duration: 22s; background-color: rgba(255, 255, 255, 0.25);}"
".bg-bubbles li:nth-child(5) { left: 70%;}"
".bg-bubbles li:nth-child(6) { left: 80%; width: 120px; height: 120px; -webkit-animation-delay: 3s; animation-delay: 3s; background-color: rgba(255, 255, 255, 0.2);}"
".bg-bubbles li:nth-child(7) { left: 32%; width: 160px; height: 160px; -webkit-animation-delay: 7s;    animation-delay: 7s;}"
".bg-bubbles li:nth-child(8) {"
  "left: 55%;"
  "width: 20px;"
  "height: 20px;"
  "-webkit-animation-delay: 15s;"
          "animation-delay: 15s;"
  "-webkit-animation-duration: 40s;"
          "animation-duration: 40s;}"
".bg-bubbles li:nth-child(9) {"
  "left: 25%;"
  "width: 10px;"
  "height: 10px;"
  "-webkit-animation-delay: 2s;"
          "animation-delay: 2s;"
  "-webkit-animation-duration: 40s;"
          "animation-duration: 40s;"
 " background-color: rgba(255, 255, 255, 0.3);}"
".bg-bubbles li:nth-child(10) {"
  "left: 90%;"
 " width: 160px;"
 " height: 160px;"
 " -webkit-animation-delay: 11s;"
         " animation-delay: 11s;}"
"@-webkit-keyframes square {"
  "0% {-webkit-transform: translateY(0);"
           " transform: translateY(0);}"
  "100% { -webkit-transform: translateY(-700px) rotate(600deg);"
           " transform: translateY(-700px) rotate(600deg); }}"
"@keyframes square {"
 " 0% { -webkit-transform: translateY(0);"
         "  transform: translateY(0);}"

 " 100% { -webkit-transform: translateY(-700px) rotate(600deg);"
          " transform: translateY(-700px) rotate(600deg);}}"

"</style>"
"</head>"
"<body>"
 "<font color=\"black\"><h2>Bell Automation</h2>"
 "<h3>Model Engineering College</h3></font>"
 " <div class=\"wrapper\">"
 " <div class=\"container\">"
  "  <h1>Welcome</h1>"
   "<FORM class=\"form\" action=\"/\" method=\"post\">" 
      "<input type=\"text\" placeholder=\"Username\" name=\"name\">"
     " <input type=\"password\" placeholder=\"Password\" name=\"pass\">"
     " <button type=\"submit\">Login</button>"
    "</form>"
"<h4>Developed by REBOOT</h4>  "
" </div>"
"<ul class=\"bg-bubbles\">"
"<li></li><li></li><li></li><li></li><li></li><li></li><li></li><li></li><li></li><li></li>"
"</ul>"
"</div>"
"</font>"
"</body>"
"</html>";




const char FORM_HTML_INCORRECT[] =
"<!DOCTYPE HTML>"
"<html>"
"<head>"
"<meta name = \"viewport\" content = \"width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0\">"
"<title>Bell Control</title>"
"<style>"
"\"body { background-color: #808080; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }\""
"</style>"
"</head>"
"<body><center>"
"<h1>BELL AUTOMATION</h1>"
"<h3>MODEL ENGINEERING COLLEGE</h3>"
"</center>"
"<h3>Access Denied</h5>"
"<h3>Contact Incharge</h5>"
"<FORM action=\"/\" method=\"post\">"
"<input type=\"hidden\" name=\"back\" value=\"back\">"
"<INPUT type=\"submit\" value=\"Back\">"
"</P>"
"</FORM>"
"</body>"
"</html>";


// GPIO#0 is for Adafruit ESP8266 HUZZAH board. Your board LED might be on 13.
const int LEDPIN = 5;
void handleRoot()
{
 String dd = server.arg("dur");
 dur=dd.toInt();
  
 if(server.hasArg("hour")&&server.hasArg("minute"))
 {
  current_time();
  current_day();
   disp_time_set();
 
 }

  if(server.hasArg("pass")) {
    handleSubmit_login();
  }
  else if(server.hasArg("back")) {
     server.send(200, "text/html", INDEX_HTML);
  }

 else if (server.hasArg("U"))
{
      if(server.arg("U")=="1")
      {
        s=server.arg("time");
        if(s!="")
         update_set();   
         transfer_dat();
      }
      else if(server.arg("U")=="0")
      {
        s=server.arg("time");
        if(s!="")
        delete_set();
        transfer_dat();
      }
disp_time_set();     
server.send(200, "text/html", FORM_HTML);
manual();
}
 else
  server.send(200, "text/html", INDEX_HTML);    
}

void returnFail(String msg)
{
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(500, "text/plain", msg + "\r\n");
}
void handleSubmit_login()
{
  String admin;
   if (!server.hasArg("pass")) return returnFail("BAD ARGS");
   { admin = server.arg("pass");
     if(admin==login_pass)
     server.send(200, "text/html", FORM_HTML);
     else
     server.send(200, "text/html", FORM_HTML_INCORRECT);
     
    }
  
}

void returnOK()
{
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "OK\r\n");
}


void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}


void setup(void)
{

  rtc.begin(DateTime(2018, 7, 1, 3, 0, 0));
  pinMode(LEDPIN, OUTPUT);
  pinMode(alert, OUTPUT);
  digitalWrite(alert,LOW);
  digitalWrite(LEDPIN,LOW);
  Serial.begin(115200);
  EEPROM.begin(512);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  Serial.println("");
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (mdns.begin("Web Form", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.on("/data.txt", [](){
  server.send(200, "text/html", time_set);
 });
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.on("/notification.txt", [](){
 server.send(200, "text/html", responce);
 });
  server.begin();
  Serial.print("Connect to http://");
  Serial.println(WiFi.localIP());
  transfer_dat();
}
int loc_mts=90;
int res=0;
boolean st=HIGH;
void loop(void)
{
  
  DateTime now = rtc.now();
  server.handleClient();
  if(now.day()>7)
  settime(HIGH);
  if(check_dat(now.hour(),now.minute(),now.day())==HIGH)
  { 
      if(loc_mts==90||loc_mts!=now.minute())
     { 
      ring();
      loc_mts=now.minute();
     }
  }

  if(IST_status==0)
 {
  if(res<200)
   {  
     delay(1);
     res++;
   }  
   else
   {
    res=0;
    st=!st;
   }
 }
 else
 st=HIGH;
 digitalWrite(alert,st);  
}
