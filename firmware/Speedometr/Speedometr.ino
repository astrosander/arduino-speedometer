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
  
  mean = analogRead(A0);

  EEPROM.get(0, data);
  if(data.num < 4294967290){//for first time
    num =  data.num;
    numC = data.numC;
    TimeDur = data.TimeDur;
    MaxSpeed = data.MaxSpeed;
    MaxAcceleration = data.MaxAcceleration;
    SpeedFormat = data.SpeedFormat;
  }
  
  check(); 
}

void check(){
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
  
  if (myTimer.isReady()) check();
  if (BackUp.isReady()) BackUP();
}

void BackUP(){
  data.num = num;
  data.numC = numC;
  data.TimeDur = TimeDur + millis();
  data.MaxSpeed = MaxSpeed;
  data.MaxAcceleration = MaxAcceleration;
  data.SpeedFormat = SpeedFormat;
  EEPROM.put(0, data);
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
