void SpeedometerTick(){
  int val = analogRead(A0);
  
  if(val >= CalibrationValue){
    lcd.setBacklight(HIGH);
    lastturn = millis();
    
    if(f) return;
    if(delta < 70) return; //For stability 

    float PrevVel = vel;
    vel = len / (delta) * 1000;
    Acceleration = (vel - PrevVel) / delta * 1000;
    f = 1;

    if(abs(Acceleration) > 45) {vel = PrevVel; return;} //Unless you're an astronaut
    
    num++;

    if(delta < RestTime) numC++;
  }
  else 
  {
    if(delta > RestTime) {//it's time to sleep
      lcd.setBacklight((delta <= SleepTime) ? HIGH: LOW);
      
      vel = 0;
    }
    else lcd.setBacklight(HIGH);
    f=0;  
  }
}

//void ChangeBright(bool Status){
//  if(Status) lcd.setBacklight(HIGH);
//  else{
//    
//  }
//  lcd.setBacklight(Status ? HIGH: LOW);
//}

String FToStr(float num){
  char bufferi[10];
  
  if(num < 10) dtostrf(num, 3, 1, bufferi);
  else dtostrf(num, 4, 1, bufferi);

  return String(bufferi);
}

String CheckSpeed(float str){
  if(SpeedFormat) return FToStr(str) + " m/s     ";
  else return FToStr(str*3.6) + " km/h     ";
}

String TimeFormat(unsigned long Time, bool ModeF){
    int seconds = (Time / 1000) % 60;
    int minutes = (Time / 60000) % 60;
    int hours = Time / 3600000;

    String ans = "";

    if(!ModeF)
    {
      if(hours){
        if (hours < 10) ans += "0";
        ans += String(hours) + ":";
        if (minutes < 10) ans += "0";
        ans += String(minutes);        
      }
      else{
        if (minutes < 10) ans += "0";
        ans += String(minutes) + ":";
        if (seconds < 10) ans += "0";
        ans+=String(seconds);        
      }
      return ans;
    }
    
    if (hours < 10) ans += "0";
    ans += String(hours) + ":";
    if (minutes < 10) ans += "0";
    ans += String(minutes) + ":";
    if (seconds < 10) ans += "0";
    ans+=String(seconds);

    return ans;
}
