void DrawDisplay(){  
  float dist = num*len;
  
  if(mode == 0){
    long AllWay = dist;
    String distance = "";
    if(AllWay < 1000) distance = String(AllWay);
    else distance = String(AllWay / 1000) + "," + String(AllWay % 1000); 
     
    lcd.setCursor(0,0);
    lcd.print(distance + " m");
    
    lcd.setCursor(0,1);
    lcd.print(CheckSpeed(vel));

   Serial.println(CheckSpeed(vel));
  }
  else if(mode == 1){
    lcd.setCursor(0,0);
    lcd.print("Max: " + CheckSpeed(MaxSpeed));

    byte customChar[] = { 0x1C, 0x04, 0x08, 0x1C, 0x00, 0x00, 0x00, 0x00 };
    lcd.createChar(0, customChar);
      
    lcd.setCursor(0,1);
    lcd.print(FToStr(MaxAcceleration) + " m/s");
    lcd.write(0);
    lcd.print("   "); 
     
  }
  else if(mode == 2){
    lcd.setCursor(0,0);
    float Aver = dist / (millis() + TimeDur)*1000;
    lcd.print("Aver: " + CheckSpeed(Aver));
        
    lcd.setCursor(0,1);
    float AverC = numC*len / (millis() + TimeDur)*1000;
    lcd.print("AverC: " + CheckSpeed(AverC));
  }
  else if(mode == 3){
    lcd.setCursor(0,0);
    lcd.print(TimeFormat(millis()));

    lcd.setCursor(0,1);
    lcd.print(TimeFormat(millis() + TimeDur));
  }  
  else if(mode == 4){
    int disti = vel*10;
    int mxspeed = MaxSpeed * 10;
    drawPlot(0, 1, 16, 2, 0, mxspeed, disti);
  }
}
