
/* Bike Computer based on Arduino and Hall Sensor
 * Сomponents: Analog Hall Sensor & Magnet, Arduino UNO/Nano, button, LCD display I2C 16x02
 * Home page: https://github.com/astrosander/arduino-speedometer
 * Made with 💖 by astrosander
 */
 
#define NumMode 8 //number of modes
#define ButPin 13 //pin of the Button
#define CalibrationValue 539 //depends on the location of magnet relatively to Hall Sensor 
#define RestTime 4000 //the time after which the bicycle is considered to have stopped (in milliseconds)
#define SleepTime 30000 //time after which the backlight switches off (in milliseconds)
const float len = 2.125; //length of the wheel


#include <LiquidCrystal_I2C.h>
#include <EncButton.h>
#include <GyverTimer.h>
#include <EEPROM.h>

EncButton<EB_TICK, ButPin> enc; 
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

GTimer_ms myTimer(500);
GTimer_ms BackUp((long)3*3750);
GTimer_ms OneMinPlot((long)1*3750);
GTimer_ms FifteenMinPlot((long)15*3750);

bool SpeedFormat, f, CurrentBright=1;
byte mode = 0;
int Secondly[16], OneMin[16], ThreeMin[16], FifteenMin[16], mean;
unsigned long num, numC, TimeDur, lastturn, delta=0;
float vel, Acceleration, MaxSpeed, MaxAcceleration=-99999;


struct Data {
  unsigned long num = 0;
  unsigned long numC = 0;
  unsigned long TimeDur = 0;
  float MaxSpeed = 0;
  float MaxAcceleration = 0;
  bool SpeedFormat = 0;
};

Data data;

void setup()
{
  Serial.begin(115200);
  pinMode(A0, INPUT);
  enc.setStepTimeout(500);
  
  lcd.init();                
  lcd.backlight();
  initPlot();
  EEPROMInit();
  
  mean = analogRead(A0);
  
  DrawDisplay(); 
}

void loop()
{
  delta = millis() - lastturn;

  ButtonTick();
  SpeedometerTick();
  plotTick();

  //cheecking of timers
  if (myTimer.isReady()) DrawDisplay();
  if (BackUp.isReady() && delta < 4000) BackUP();
}

void RefreshPloat(int *plot_array){
  for (byte i = 0; i < 15; i++) plot_array[i] = plot_array[i + 1];
  plot_array[15] = vel*100;
}
