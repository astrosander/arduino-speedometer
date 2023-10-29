void(* resetFunc) (void) = 0; 

void ButtonTick(){
  enc.tick();

  if (enc.held()) mode = 0;
  
  if(!enc.hasClicks()) return;
  int clickCount = enc.clicks;
  lastturn = millis();
  lcd.setBacklight(HIGH);
  
  if(enc.clicks == 1){
    lastturn = millis();
    lcd.setBacklight(HIGH);
    if(delta > SleepTime) return;
    lcd.clear();
    mode = (mode + 1) % NumMode;
  }

  
  if(enc.clicks == 2){lcd.clear(); mode = (mode + NumMode - 1) % NumMode;} 
  if(enc.clicks == 3) SpeedFormat = !SpeedFormat;
  
  if(enc.clicks == 4) MaxSpeed=0;
  if(enc.clicks >= 10) {BackReset();resetFunc();}
}
