void EEPROMInit(){
  EEPROM.get(0, data);
  if(data.num < 4294967290){//for first time
    num =  data.num;
    numC = data.numC;
    TimeDur = data.TimeDur;
    MaxSpeed = data.MaxSpeed;
    MaxAcceleration = data.MaxAcceleration;
    SpeedFormat = data.SpeedFormat;
  }
}

void BackUP(){
//  for (byte i = 0; i < 15; i++) FiveMin[i] = FiveMin[i + 1];
//  FiveMin[15] = vel*100;
  data.num = num;
  data.numC = numC;
  data.TimeDur = TimeDur + millis();
  data.MaxSpeed = MaxSpeed;
  data.MaxAcceleration = MaxAcceleration;
  data.SpeedFormat = SpeedFormat;
  EEPROM.put(0, data);
}

void BackReset(){
  data.num = 4294967292;
  EEPROM.put(0, data);
}
