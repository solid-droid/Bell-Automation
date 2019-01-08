
void writedat(int add,String dat){
   for(int i=0;i<dat.length();i++)
  {
    EEPROM.write(add+i,dat[i]);
  }
  EEPROM.commit();  
}
String readdat(int add,int add_dat)
{
    dat.remove(0); 
  for(int i=1;i<EEPROM.read(add);i++) 
  {
    dat = dat + char(EEPROM.read(add_dat+i)); 
  }  
 return(dat);
}

