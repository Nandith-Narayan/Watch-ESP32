#include <TFT_eSPI.h>
#include <ESP32Time.h>
#include "colors.h"
#include "render.h"
#include "apple.h"

#define BACKLIGHT_PIN 21
#define BACKLIGHT_PWN_CHNL 0
#define BUTTON_1 33


TFT_eSPI tft = TFT_eSPI();

double seconds = 30.0;
int minutes = 30;
int hours = 8;
String am_or_pm="AM";

hw_timer_t *sleepTimer = NULL;
uint8_t brightness = 255;

double percent = 100.0;
TFT_eSprite background = TFT_eSprite(&tft);

ESP32Time rtc(0);


void setup(){
  Serial.begin(115200);
  rtc.setTime(0, 55, 22, 28, 10, 2022);
  tft.init();
  tft.setRotation(0);

  background.setColorDepth(4);
  background.createPalette(cmap);
  background.createSprite(240, 240);
  background.fillSprite(BLACK);

  ledcAttachPin(BACKLIGHT_PIN, 0);
  ledcSetup(BACKLIGHT_PWN_CHNL, 1000, 8);
  ledcWrite(BACKLIGHT_PWN_CHNL, brightness);
  setCpuFrequencyMhz(240);
  
  sleepTimer = timerBegin(0, 80, true);
  timerAttachInterrupt(sleepTimer, &enableSleep, true);
  timerAlarmWrite(sleepTimer, 60*1000000, true);
  timerAlarmEnable(sleepTimer);
  
  touchAttachInterrupt(BUTTON_1, &wakeUp, 50);
  esp_sleep_enable_touchpad_wakeup();

}
  int apple_index = 0;
  int apple_frame = 0;
  
void loop(){
  //Serial.println(getCpuFrequencyMhz());
  unsigned long bt = micros();

  /*background.fillSprite(BLACK);


  
  drawBatteryLevel();
  drawMinNumbers();
  drawMinTicks();
  drawSecondNumbers();
  drawSecondTicks();

  background.fillRect(160, 100, 40, 40, BLACK);
  background.drawRoundRect(145, 100, 90, 40, 20, WHITE);

  background.drawString(String((int)minutes),180, 122, 4);
  background.setTextColor(WHITE);
  background.drawString(String(hours), 110, 125, 6);
  background.drawString(am_or_pm, 115, 152, 2);*/

  int x = 0;
  int y = 0;
  int offset_y = 30;
  int color = WHITE;
  apple_frame = (apple_frame+1) % FRAME_LEN;
  while(apple_index<apple_frames[apple_frame]){

    int val = apple_data[apple_index++];
    if(x+val >=240){
      background.drawFastHLine(x, y+offset_y, 240-x, color);
      val -= 240-x;
      y++;
      x = 0;
    }
    if(val > 0){
      background.drawFastHLine(x, y+offset_y, val, color);
      x += val;
    }

    if(color == WHITE){
      color = BLACK;
    }else{
      color = WHITE;
    }
  }
  apple_index = apple_frames[apple_frame];
  //background.drawFastHLine(0, 180+30,240,RED);
  background.pushSprite(0,0);
  //delay(50);
  updateTime();
  while((micros()-bt)< 33000){}
}

void updateTime(){
  seconds = rtc.getSecond()*1.0 + rtc.getMillis()/1000.0;
  minutes = rtc.getMinute();
  hours = rtc.getHour(false);
  am_or_pm = rtc.getAmPm();
}

void enableSleep(){
  //ledcWrite(BACKLIGHT_PWN_CHNL, 0);
  unsigned long bt = micros();
  while((micros() - bt) < 5000){}
  //esp_light_sleep_start();
  
}

void wakeUp(){
  timerWrite(sleepTimer, 0);
  ledcWrite(BACKLIGHT_PWN_CHNL, brightness);

}



