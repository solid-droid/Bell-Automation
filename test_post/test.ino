void check_time()
{
    DateTime now = rtc.now();
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
}

void check_mem()
{
  for(int i=0;i<set_size+1;++i)
  {for(int j=0;j<3;++j)
  Serial.print(local[i][j]);
  Serial.println();
  }
}

