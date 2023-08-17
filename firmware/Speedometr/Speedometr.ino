//#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <EncButton.h>
#include <GyverTimer.h>
GTimer_ms myTimer(500);               // создать миллисекундный таймер

EncButton<EB_TICK, 13> enc; 
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
int mean;

void setup()
{
  Serial.begin(9600);
  pinMode(A0, INPUT);
  lcd.init();                
  // Print a message to the LCD.
  lcd.backlight();
  
  lcd.setCursor(0,0);
  lcd.print("Calibration...");
  
  for(int i = 0; i < 10; i++){
    mean += analogRead(A0);
    delay(100);  
  }
  mean /= 10;
  
  lcd.clear();
  check(); 
}

int num = 0;
int mode = 0;
unsigned long lastturn;
float len = 2.125;
float dist, vel;

void(* resetFunc) (void) = 0; 

void check(){
  if(mode == 0){
    int disti = dist;
    char bufferi[10];
    dtostrf(vel, 4, 1, bufferi);
     
    lcd.setCursor(1,0);
    lcd.print(String(disti) + " m");
    
    lcd.setCursor(0,1);
    lcd.print(String(bufferi) + " m/s   ");  
  }
  
}
void loop()
{
  enc.tick();
  if (enc.click()) mode = (mode + 1) % 5;
  if(enc.clicks == 6) resetFunc();

  int val = abs(mean-analogRead(A0));

  if(val > 2){
    if(millis() - lastturn > 70){
      dist+=len;
      vel = len / (millis() - lastturn) * 1000;
     
      num=num+1;
      check();
    }
    lastturn = millis();
  }
  
  if (myTimer.isReady()) check();
}
