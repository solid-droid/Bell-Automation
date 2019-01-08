void store(int add)
{
 int val=add*3+1;
  String c;int cday;
  c=server.arg("date");
  cday=c.toInt();
   EEPROM.write(val, cday);
   EEPROM.commit();
   EEPROM.write(val+1, timeset[0]);
   EEPROM.commit();
   EEPROM.write(val+2, timeset[1]);
   EEPROM.commit();
}

int readd(int add)
{
  return(EEPROM.read(add));
 
}

void disp_time_set()
{
  int dtemp;
  responce="Submit successful";
  if(readd(2)>0)
   {
   time_set="Available : ";
   set_size=readd(2);
   for(int i=1;i<set_size+1;++i)
  { time_set+=" ||";
    dtemp=readd(i*3+1);
    switch(dtemp)
    {

     case 7:time_set+="Everyday :";break;
     case 1:time_set+="Monday :";break; 
     case 2:time_set+="Tuesday :";break; 
     case 3:time_set+="Wednesday :";break; 
     case 4:time_set+="Thursday :";break; 
     case 5:time_set+="Friday :";break; 
     case 6:time_set+="Saturday :";break;
     case 0:time_set+="Sunday :";break;   


    }
    time_set+="(";
    time_set+=readd(i*3+2);
    time_set+=":";
    time_set+=readd(i*3+3);
    time_set+=")";
    time_set+="||";
    time_set+=" , ";
  }
  }
   else
   time_set="Not Found"; 
 }

 void adj(int add)
{
   set_size=readd(2);
   if(set_size>0)
   {
   int temp;
   for(int i=add;i<set_size+1;++i)
  {
    int val=i*3+1;
    int val1=(i+1)*3+1;
    temp=readd(val1);
    EEPROM.write(val, temp);
    EEPROM.commit();
    delay(1);
    temp=readd(val1+1);
    EEPROM.write(val+1, temp);
    EEPROM.commit();
    delay(1);
    temp=readd(val1+2);
    EEPROM.write(val+2, temp);
    EEPROM.commit();
    delay(1);
   }
   EEPROM.write(2, set_size-1);
    EEPROM.commit();
   }
  delay(1);
}
void current_day()
{ String c;int cday;
  c=server.arg("day");
  cday=c.toInt();
EEPROM.write(3, cday);
EEPROM.commit();
delay(1);
}

