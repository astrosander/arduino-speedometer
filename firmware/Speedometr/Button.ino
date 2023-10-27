void ButtonTick(){
  enc.tick();

  if (enc.held()) SpeedFormat = !SpeedFormat;
  
  if(!enc.hasClicks()) return;
  int clickCount = enc.clicks;

  if(enc.clicks == 1){
    lastturn = millis();
    lcd.setBacklight(HIGH);
    if(delta > 30000) return;
    lcd.clear();
    mode = (mode + 1) % NumMode;
  }

  
  if(enc.clicks == 2) mode = 0;
  if(enc.clicks == 5) MaxSpeed=0;
  if(enc.clicks == 10) {BackReset();resetFunc();}
}
