#include "render.h"
#include "colors.h"
#include <Arduino.h>

extern TFT_eSprite background;
extern double seconds;
extern int minutes;
extern int hours;
extern String am_or_pm;
extern double percent;

void drawBatteryLevel(){
  
  uint color = TFT_GREEN;
  if(percent <= 75.0){
    color = TFT_GREENYELLOW;
  }
  if(percent <= 50.0){
    color = TFT_YELLOW;
  }
  if(percent <= 25.0){
    color = TFT_RED;
  }
  color = WHITE;
  //background.pushImage(0, 0, 240, 240, (uint16_t *)background_pic);
  double voltage = getBatteryVoltage();
  if(voltage > 4.1){
    percent = 100.0;
  }else if(voltage <= 3.0){
    percent = 0.0;
  }else{
    percent = 100.0*(voltage - 3.0)/1.1;
  }

  fillArc(120,120,0,(int)(180.0*percent/100.0), 120, 120, 5, color);
  //background.fillSmoothCircle(120,120,115, TFT_PASET);

  

  background.setTextColor(WHITE);
  background.drawCentreString(String(voltage)+"V", 70+50, 140+25, 2);
}

void drawSecondTicks(){
  for(int i=0;i<60;i++){
    uint color = DARK_GREY;
    double angle = (i+seconds)*0.10472;
    int r1 = 113;
    int r2 = 108;
    if(i%5==0){
      r2 = 103;
      color = LIGHT_GREY;
    }
    int x1 = r1*cos(angle) + 120.0;
    int y1 = r1*sin(angle) + 120.0;
    int x2 = r2*cos(angle) + 120.0;
    int y2 = r2*sin(angle) + 120.0;
    background.drawLine(x1,y1,x2,y2,color);
  }
}

void drawSecondNumbers(){
  for(int i=1; i <= 12; i++){
    double angle = (i*5+seconds)*0.10472;
    int r = 100;
    int x = r*cos(angle)+120;
    int y = r*sin(angle)+120;
    background.setTextDatum(MC_DATUM);
    background.setTextColor(WHITE);
    background.drawString(String(60-(i*5)),x,y,2);
  }
}

void drawMinTicks(){
  for(int i=0;i<60;i++){
    uint color = DARK_GREY;
    double angle = (i+minutes)*0.10472;
    int r1 = 95;
    int r2 = 90;
    if(i%5==0){
      r2 = 85;
      color = RED;
    }
    int x1 = r1*cos(angle) + 120.0;
    int y1 = r1*sin(angle) + 120.0;
    int x2 = r2*cos(angle) + 120.0;
    int y2 = r2*sin(angle) + 120.0;
    background.drawLine(x1,y1,x2,y2,color);
  }
}

void drawMinNumbers(){
  for(int i=1; i <= 12; i++){
    double angle = (i*5+minutes)*0.10472;
    int r = 72;
    int x = r*cos(angle)+120;
    int y = r*sin(angle)+120;
    background.setTextDatum(MC_DATUM);
    background.setTextColor(DARK_GREY);
    background.drawString(String(60-(i*5)),x,y,2);
  }
}

double getBatteryVoltage(){
  return (analogRead(35)/4095.0)*2*3.3*1.1;
}



#define DEG2RAD 0.0174532925

void fillArc(int x, int y, int start_angle, int seg_count, int rx, int ry, int w, unsigned int colour)
{

  byte seg = 2; // Segments are 3 degrees wide = 120 segments for 360 degrees
  byte inc = 2; // Draw segments every 3 degrees, increase to 6 for segmented ring

  // Calculate first pair of coordinates for segment start
  float sx = cos((start_angle - 90) * DEG2RAD);
  float sy = sin((start_angle - 90) * DEG2RAD);
  uint16_t x0 = sx * (rx - w) + x;
  uint16_t y0 = sy * (ry - w) + y;
  uint16_t x1 = sx * rx + x;
  uint16_t y1 = sy * ry + y;

  // Draw colour blocks every inc degrees
  for (int i = start_angle; i < start_angle + seg * seg_count; i += inc) {

    // Calculate pair of coordinates for segment end
    float sx2 = cos((i + seg - 90) * DEG2RAD);
    float sy2 = sin((i + seg - 90) * DEG2RAD);
    int x2 = sx2 * (rx - w) + x;
    int y2 = sy2 * (ry - w) + y;
    int x3 = sx2 * rx + x;
    int y3 = sy2 * ry + y;

    background.fillTriangle(x0, y0, x1, y1, x2, y2, colour);
    background.fillTriangle(x1, y1, x2, y2, x3, y3, colour);

    // Copy segment end to sgement start for next segment
    x0 = x2;
    y0 = y2;
    x1 = x3;
    y1 = y3;
  }
}