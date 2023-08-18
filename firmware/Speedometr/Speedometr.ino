#include <LiquidCrystal_I2C.h>
#include <EncButton.h>
#include <GyverTimer.h>

EncButton<EB_TICK, 13> enc; 
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int mean;
int plot_array[20];
byte mode = 0;
unsigned long num = 0;
unsigned long lastturn;
const float len = 2.125;
float dist, vel, MaxSpeed, MaxAcceleration;

GTimer_ms myTimer(1000);               // создать миллисекундный таймер

void(* resetFunc) (void) = 0; 

void setup()
{
  Serial.begin(9600);
  pinMode(A0, INPUT);
  
  lcd.init();                
  lcd.backlight();
  initPlot();
  
  lcd.setCursor(0,0);
  lcd.print("Calibration...");
  
  for(byte i = 1; i < 11; i++) mean += analogRead(A0);
  mean /= 10;
  
  lcd.clear();
  
  check(); 
}

void check(){
  if(mode == 0){
    long AllWay = dist;
    String distance = "";
    if(AllWay < 1000) distance = String(AllWay);
    else distance = String(AllWay / 1000) + "," + String(AllWay % 1000); 
     
    lcd.setCursor(0,0);
    lcd.print(distance + " m");
    
    lcd.setCursor(0,1);
    lcd.print(FToStr(vel) + " m/s  " + FToStr(vel*3.6) + " km/h    ");  
  }
  else if(mode == 1){
    lcd.setCursor(0,1);
    lcd.print(FToStr(MaxSpeed) + " m/s  " + FToStr(MaxSpeed*3.6) + " km/h    ");  
    
    lcd.setCursor(1,1);
    lcd.print(FToStr(MaxAcceleration) + " m/s  " + FToStr(MaxAcceleration*3.6) + " km/h    ");  
  }
  else if(mode == 2){
    int disti = vel*10;
    int mxspeed = MaxSpeed * 10;
    drawPlot(0, 1, 16, 2, 0, mxspeed, disti);
  }
  
}

void loop()
{
  enc.tick();
  if (enc.click()) mode = (mode + 1) % 3;
  if(enc.clicks == 6) resetFunc();

  int val = abs(mean-analogRead(A0));

  if(val > 2){
    int delta = millis() - lastturn;
    if(delta > 70){
      dist+=len;
      float PrevVel = vel;
      vel = len / (delta) * 1000;
      
      MaxSpeed = max(MaxSpeed, vel);
      MaxAcceleration = max(MaxSpeed, (vel - PrevVel) / delta);
      num=num+1;
      
      check();
    }
    
    lastturn = millis();
  }
  
  if (myTimer.isReady()) check();
}


void drawPlot(byte pos, byte row, byte width, byte height, int min_val, int max_val, int fill_val) {
  initPlot();
  for (byte i = 0; i < width; i++) {
    plot_array[i] = plot_array[i + 1];
  }
  fill_val = constrain(fill_val, min_val, max_val);
  plot_array[width] = fill_val;
  for (byte i = 0; i < width; i++) {                  // каждый столбец параметров
    byte infill, fract;
    // найти количество целых блоков с учётом минимума и максимума для отображения на графике
    infill = floor((float)(plot_array[i] - min_val) / (max_val - min_val) * height * 10);
    fract = (infill % 10) * 8 / 10;                   // найти количество оставшихся полосок
    infill = infill / 10;
    for (byte n = 0; n < height; n++) {     // для всех строк графика
      if (n < infill && infill > 0) {       // пока мы ниже уровня
        lcd.setCursor(i, (row - n));        // заполняем полными ячейками
        lcd.write(0);
      }
      if (n >= infill) {                    // если достигли уровня
        lcd.setCursor(i, (row - n));
        if (fract > 0) lcd.write(fract);          // заполняем дробные ячейки
        else lcd.write(16);                       // если дробные == 0, заливаем пустой
        for (byte k = n + 1; k < height; k++) {   // всё что сверху заливаем пустыми
          lcd.setCursor(i, (row - k));
          lcd.write(16);
        }
        break;
      }
    }
  }
}

void initPlot() {
  byte row8[8] = {0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
  byte row7[8] = {0b00000,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
  byte row6[8] = {0b00000,  0b00000,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
  byte row5[8] = {0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
  byte row4[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111,  0b11111};
  byte row3[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111};
  byte row2[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111};
  byte row1[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111};
  lcd.createChar(0, row8);
  lcd.createChar(1, row1);
  lcd.createChar(2, row2);
  lcd.createChar(3, row3);
  lcd.createChar(4, row4);
  lcd.createChar(5, row5);
  lcd.createChar(6, row6);
  lcd.createChar(7, row7);
}
String FToStr(float num){
  char bufferi[10];
  
  if(num < 10) dtostrf(vel, 3, 1, bufferi);
  else dtostrf(vel, 4, 1, bufferi);

  return String(bufferi);
}

int floatToInt(float val){return val;}
