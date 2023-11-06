void(* resetFunc) (void) = 0; 

void ButtonTick(){
  enc.tick();

  if (enc.held()) {lcd.clear();mode = 0;}
  
  if(!enc.hasClicks()) return;
  int clickCount = enc.clicks;
  lastturn = millis();
  lcd.setBacklight(HIGH);
  if(delta > SleepTime) return;
  
  if(enc.clicks == 1){  
    lcd.clear();
    mode = (mode + 1) % NumMode;
  }

  if(enc.clicks == 2){
    lcd.clear(); 
    mode = (mode + NumMode - 1) % NumMode;
  } 
  
  if(enc.clicks == 3) SpeedFormat = !SpeedFormat;
  
  if(enc.clicks == 4) {
    MaxAcceleration = 0;
    MaxSpeed = 0;
  }
  if(enc.clicks >= 6) {
    BackReset();
    resetFunc();
  }
}
