void plotTick(){
  if(OneMinPlot.isReady()) RefreshPloat(FifteenMin);
  if(FifteenMinPlot.isReady()) RefreshPloat(OneMin);
}

void drawPlot(byte pos, byte row, byte width, byte height, int *plot_array) {

  int max_val = -32000;
  int min_val = 0;
  for (byte i = 0; i < width; i++) max_val = max(max_val, plot_array[i]);

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
