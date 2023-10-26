void plotTick(){
  if(SecondlyPlot.isReady()){
    for (byte i = 0; i < 15; i++) Secondly[i] = Secondly[i + 1];
    Secondly[15] = vel*100;
  }
  if(FiveMinPlot.isReady()){
    for (byte i = 0; i < 15; i++) FiveMin[i] = FiveMin[i + 1];
     FiveMin[15] = vel*100;
  }
//  if(FifteenMinPlot.isReady()){
//    for (byte i = 0; i < 15; i++) FifteenMin[i] = FifteenMin[i + 1];
//     FifteenMin[15] = vel*100;
//  } 
//  if(HourPlot.isReady()){
//    for (byte i = 0; i < 15; i++) Hour[i] = Hour[i + 1];
//     Hour[15] = vel*100;
//  }   
}


void drawPlot1(byte pos, byte row, byte width, byte height, int *plot_array) {

//  for (byte i = 0; i < width; i++){
//    Serial.print(plot_array[i]);
//    Serial.print("   ");
//  }
//  Serial.println("\n");
  
  int max_val = -32000;
  int min_val = 0;
  for (byte i = 0; i < width; i++) {
//    min_val = min(min_val, plot_array[i]);
    max_val = max(max_val, plot_array[i]);
//    if (plot_array[i] > max_value) max_value = plot_array[i];
//    if (plot_array[i] && plot_array[i] < min_value) min_value = plot_array[i];
  }

  
//  min_val = min_val*0.95*0;
  max_val = max_val*1.05;

  
  for (byte i = 0; i < width; i++) { // each column of parameters
    int fill_val = plot_array[i];
    fill_val = constrain(fill_val, min_val, max_val);
    byte infill, fract;
    // find the number of integer blocks considering the minimum and maximum to be displayed on the chart
    if (plot_array[i] > min_val)
      infill = floor((float)(plot_array[i] - min_val) / (max_val - min_val) * height * 10);
    else infill = 0;
    fract = (float)(infill % 10) * 8 / 10; // find the number of remaining strips
    infill = infill / 10;

    for (byte n = 0; n < height; n++) { // for all lines of the chart
      if (n < infill && infill > 0) { // while we are below the level
        lcd.setCursor(i, (row - n)); // fill with full cells
        lcd.write(0);
      }
      if (n >= infill) { // if we have reached the level
        lcd.setCursor(i, (row - n));
        if (fract > 0) lcd.write(fract); // fill fractional cells
        else lcd.write(16); // if fractional == 0, fill the empty one
        for (byte k = n + 1; k < height; k++) { // fill everything on top with empty ones
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











void drawPlot(byte pos, byte row, byte width, byte height, int min_val, int max_val, int fill_val) {
  int plot_array[20];
  initPlot();
  for (byte i = 0; i < width; i++) {
    plot_array[i] = plot_array[i + 1];
  }
  fill_val = constrain(fill_val, min_val, max_val);
  plot_array[width] = fill_val;
  for (byte i = 0; i < width; i++) {                  // каждый столбец параметров
    byte infill, fract;
    
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
