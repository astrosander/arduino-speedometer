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
