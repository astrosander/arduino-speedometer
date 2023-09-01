#include <LiquidCrystal_I2C.h>
#include <EncButton.h>
#include <GyverTimer.h>
#include <EEPROM.h>

EncButton<EB_TICK, 13> enc; 
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

GTimer_ms myTimer(1000);
GTimer_ms BackUp(10000);
GTimer_ms SecondlyPlot(1000);
GTimer_ms FiveMinPlot((long)1*3750);
//GTimer_ms FifteenMinPlot((long)5*3750);
//GTimer_ms HourPlot((long)15*3750);

int Secondly[16], FiveMin[16], FifteenMin[16], Hour[16];
int mean;
byte mode = 0;
unsigned long num, numC, TimeDur, lastturn;
const float len = 2.125;
float vel, MaxSpeed, MaxAcceleration;
bool SpeedFormat = true;

struct Data {
  unsigned long num = 0;
  unsigned long numC = 0;
  unsigned long TimeDur = 0;
  float MaxSpeed = 0;
  float MaxAcceleration = 0;
  bool SpeedFormat = 0;
};
Data data;

void(* resetFunc) (void) = 0; 

void setup()
{
  Serial.begin(9600);
  pinMode(A0, INPUT);
  
  lcd.init();                
  lcd.backlight();
  initPlot();
  EEPROMInit();
  
  mean = analogRead(A0);
  
  DrawDisplay(); 
}
const byte NumMode = 6;
long lastDelta=0;
bool f = 0;

void loop()
{
  enc.tick();
//  if(enc.clicks == 2) {mode = (mode + NumMode - 2) % NumMode;return;}

  int val = abs(mean-analogRead(A0));

  if(val > 2){
    lcd.setBacklight(HIGH);
    int delta = millis() - lastturn;
    lastturn = millis();
    if(f) return;
    f = 1;
    if(delta < 70) return;

    float PrevVel = vel;
    vel = len / (delta) * 1000;
//
    Serial.print(vel);
    Serial.print("  ");
    Serial.println(delta);
    
    MaxSpeed = max(MaxSpeed, vel);
    MaxAcceleration = max(MaxAcceleration, (vel - PrevVel) / delta * 1000);
    num++;

    if(delta < 2000)numC++;
    
    
  }
  else 
  {
  if(millis() - lastturn > 4000) {vel = 0;lcd.setBacklight(LOW);}
  f=0;  
  }
  
  if(enc.clicks == 2) mode = 0;
  if(enc.clicks == 5) MaxSpeed=0;
  if(enc.clicks == 10) {BackReset();resetFunc();}
  if (enc.click()) {
    lcd.clear();
    mode = (mode + 1) % NumMode;
//    if(mode == 4){lcd.setCursor(0,0); lcd.print("secondly Plot"); delay(500);}
//    else if(mode == 5){lcd.setCursor(0,0); lcd.print("1 minute Plot"); delay(500);}
//    else if(mode == 6){lcd.setCursor(0,0); lcd.print("5 minute Plot"); delay(500);}
//    else if(mode == 7){lcd.setCursor(0,0); lcd.print("15 minute Plot"); delay(500);}
//    lcd.clear();
//    
  }

  if (enc.held()) SpeedFormat = !SpeedFormat;

  if (myTimer.isReady()) DrawDisplay();
  if (BackUp.isReady()) BackUP();
  
  plotTick();
}
