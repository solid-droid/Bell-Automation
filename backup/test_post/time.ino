void settime(boolean k)
{
    int hrs,mts,td;
   if(k==LOW)
    { hrs=readd(0);
      mts=readd(1);
      td=readd(3);
      rtc.adjust(DateTime(2018, 7, td+1, hrs,mts,0)); 
    }
    else
    rtc.adjust(DateTime(2018, 7, 1, 0,0, 0)); 
}

void transfer_dat()
{
  int temp;
  set_size=readd(2);
  if(set_size>0);
  {
    for(int i=1; i<set_size+1; ++i)
     {
      int val=i*3+1;
      temp=readd(val);
      local[i][0]=temp;
      temp=readd(val+1);
      local[i][1]=temp;
      temp=readd(val+2);
      local[i][2]=temp;
     
     }
  }
  
}

boolean check_dat(int hr,int mts,int dn)
{ 
  boolean flag=LOW;
  
  for(int i=1;i<set_size+1;++i)
 if(dn==local[i][0]||local[i][0]==7)
 {
   { 
    if(hr==local[i][1])
    if(mts==local[i][2])
    {
      flag=HIGH;
      return(HIGH);
      break;
    }}
  if(flag==LOW)
  return(LOW);
 }     
}


