void DrawDisplay(){  
  float dist = num*len;
  RefreshPloat(Secondly); 
  
  if(mode == 0){
    long AllWay = dist;
    String distance = "";
    String CurTime = TimeFormat(millis() + TimeDur, 0);
    
    if(AllWay < 1000) distance = String(AllWay) + " m";
    else distance = String(AllWay / 1000) + "," + String(AllWay % 1000) + " m"; 

    int dif = max(16 - CurTime.length() - distance.length(), 0);
    
    while(dif--) distance+=" "; 
    distance += CurTime;
     
    lcd.setCursor(0,0);
    lcd.print(distance);
    
    lcd.setCursor(0,1);
    lcd.print(CheckSpeed(vel));

   Serial.println(CheckSpeed(vel));
  }
  else if(mode == 1) drawPlot(0, 1, 16, 2, (int*)Secondly);
  
  else if(mode == 2){
    lcd.setCursor(0,0);
    lcd.print("Max: " + CheckSpeed(MaxSpeed));
      
    lcd.setCursor(0,1);
    lcd.print(FToStr(MaxAcceleration) + " m/s^2");
    lcd.print("   "); 
  }
  
  else if(mode == 3){
    lcd.setCursor(0,0);
    float Aver = dist / (millis() + TimeDur)*1000;
    lcd.print("Aver: " + CheckSpeed(Aver));
        
    lcd.setCursor(0,1);
    float AverC = numC*len / (millis() + TimeDur)*1000;
    lcd.print("AverC: " + CheckSpeed(AverC));
  }
  
  else if(mode == 4){
    lcd.setCursor(0,0);
    lcd.print(TimeFormat(millis(), 1));

    lcd.setCursor(0,1);
    lcd.print(TimeFormat(millis() + TimeDur, 1));
  }  
  else if(mode == 5) drawPlot(0, 1, 16, 2, (int*)OneMin);
  else if(mode == 6) drawPlot(0, 1, 16, 2, (int*)ThreeMin);
  else if(mode == 7) drawPlot(0, 1, 16, 2, (int*)FifteenMin);
}
