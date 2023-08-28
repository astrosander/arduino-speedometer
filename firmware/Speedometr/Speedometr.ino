#include <LiquidCrystal_I2C.h>
#include <EncButton.h>
#include <GyverTimer.h>
#include <EEPROM.h>

EncButton<EB_TICK, 13> enc; 
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

GTimer_ms myTimer(1000);
GTimer_ms BackUp(10000);

int mean;
int plot_array[20];
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

void loop()
{
  enc.tick();
  if (enc.click()) {
    lcd.clear();
    mode = (mode + 1) % 5;
  }
  if(enc.clicks == 2) mode = 0;
  if(enc.clicks == 6) {BackReset();resetFunc();}
  if (enc.held()) SpeedFormat = !SpeedFormat;

  int val = abs(mean-analogRead(A0));

  if(val > 2){
    int delta = millis() - lastturn;
    if(delta > 70){
      float PrevVel = vel;
      vel = len / (delta) * 1000;

      MaxSpeed = max(MaxSpeed, vel);
      MaxAcceleration = max(MaxAcceleration, (vel - PrevVel) / delta * 1000);
      num++;

      if(delta < 2000)numC++;
    }
    
    lastturn = millis();
  }
  
  if (myTimer.isReady()) DrawDisplay();
  if (BackUp.isReady()) BackUP();
}
