void connect_wifi()
{
    // Connect to WiFi network
  Serial.println();
  Serial.println();
  dat=readdat(1,3);
  dat.toCharArray(ssid,EEPROM.read(1)+1);
   delay(500);
  dat=readdat(2,20);
  dat.toCharArray(password,EEPROM.read(2)+1);
  delay(500);
  Serial.print("Connecting to ");
  Serial.println(ssid);
   
  WiFi.begin(ssid, password);
   
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
   
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}


void read_data(int x)
{
  int i=0;
                                        
    if (Serial.available() > 0) {             
       while (Serial.available() > 0) {
         inSerial[i]=Serial.read(); 
         i++;      
       }
       inSerial[i]='\0';
      
      if(x==1)
      ssid_data(inSerial);
      else if(x==2)
      pass_data(inSerial);
      else if(x==0)
        new_net=1;
 
     }
}

void ssid_data(char inStr[])
{   
  dat=inStr;
  EEPROM.write(1,dat.length());
  writedat(3,dat);
  
}  
void pass_data(char inStr[])
{   
  dat=inStr;
  EEPROM.write(2,dat.length());
  writedat(20,dat);
  
}  


void control_prompt()
{
     Serial.println("Enter 1 to connect to a new WIFI network in 5 seconds");
   for(int i=0;i<5000;++i)
   {
    read_data(x);
    if(new_net==1)
    break;
    if(i==0||i==1000||i==2000||i==3000||i==4000)
    Serial.print('.');
    delay(1);
   }
  if(new_net==1)
  {
   Serial.println("Enter SSID:");
    x=1;
   read_data(x);
    Serial.println("Enter Password:");
    x=2;
   read_data(x);
   delay(50);
   new_net=0;
   connect_wifi();
  }
  else
  connect_wifi(); 
}

