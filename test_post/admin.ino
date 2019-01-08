void manual()
{
    if(server.hasArg("M")) 
      if(server.arg("M")=="1")
      {
       digitalWrite(LEDPIN,HIGH);
       delay(dur*1000);
       digitalWrite(LEDPIN,LOW);
      }
      
}

void update_set()
{
  char buf[6];
  s.toCharArray(buf, 6); 
  set_size=readd(2);
  if(set_size<200)
  {responce="";
  { String temp;
    temp=buf[0];
    temp+=buf[1];
    timeset[0]=temp.toInt();
    temp=buf[3];
    temp+=buf[4];
    timeset[1]=temp.toInt();
  }

   store(set_size+1);
   set_size++;
   EEPROM.write(2, set_size);
   EEPROM.commit();
  }
  else
  responce="Reached storable limit";
  
}
void delete_set()
{
  
 char buf[6];
 s.toCharArray(buf, 6); 
 String c;
 c=server.arg("date");
 String temp;
     temp=buf[0];
    temp+=buf[1];
    set_size=readd(2);
    for(int i=1;i<set_size+1;++i)
{
 if(readd(i*3+1)==c.toInt())
 {
  if(readd(i*3+2)==temp.toInt());
  {
    temp=buf[3];
    temp+=buf[4];
    if(readd(i*3+3)==temp.toInt())
    {
      adj(i);
    }
    break;
  }
 }
}}


void current_time()
{
  char buf[3],buf1[3];
  String temp;
  (server.arg("hour")).toCharArray(buf, 3); 
  temp=buf[0];
  temp+=buf[1];
  EEPROM.write(0, temp.toInt());
  EEPROM.commit();
  delay(1);
  (server.arg("minute")).toCharArray(buf1, 3); 
  temp=buf1[0];
  temp+=buf1[1];
  EEPROM.write(1, temp.toInt());
  EEPROM.commit();
  delay(1);
  settime(LOW);
  IST_status=1;
}

void ring()
{
   Serial.println("ring");
       digitalWrite(LEDPIN,HIGH);
       delay(dur*1000);
       digitalWrite(LEDPIN,LOW);
}

