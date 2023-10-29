void SpeedometerTick(){
  int val = abs(mean-analogRead(A0));
  
  if(val >= CalibrationDelta){
    lcd.setBacklight(HIGH);
    lastturn = millis();
    
    if(f) return;
    if(delta < 70) return;

    float PrevVel = vel;
    float Acceleration = (vel - PrevVel) / delta * 1000;
    vel = len / (delta) * 1000;
    f = 1;

    if((PrevVel < 0.3 && vel > 4) || Acceleration > 80) {vel = 0; return;}
    
    MaxSpeed = max(MaxSpeed, vel);
    MaxAcceleration = max(MaxAcceleration, Acceleration);
    num++;

    if(delta < RestTime)numC++;
  }
  else 
  {
    if(delta > RestTime) {
      if(delta > SleepTime) lcd.setBacklight(LOW);
      else lcd.setBacklight(HIGH);
      
      vel = 0;
    }
    f=0;  
  }
}

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

String TimeFormat(unsigned long Time){
    int seconds = (Time / 1000) % 60;
    int minutes = (Time / 60000) % 60;
    int hours = Time / 3600000;

    String ans = "";
    
    if (hours < 10) ans += "0";
    ans += String(hours) + ":";
    if (minutes < 10) ans += "0";
    ans += String(minutes) + ":";
    if (seconds < 10) ans += "0";
    ans+=String(seconds);

    return ans;
}
