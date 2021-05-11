#include "Watchy_Analog.h"


// For more fonts look here:
// https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts
//#define FONT_LARGE       Bohemian_Typewriter22pt7b
//#define FONT_MEDUM       Bohemian_Typewriter18pt7b
#define FONT            WallingtonRegular12pt7b

WatchyAnalog::WatchyAnalog(){

}


void WatchyAnalog::handleButtonPress(){
    WatchyBase::handleButtonPress();

    uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();
    if(IS_DOUBLE_TAP){
        dark_mode = dark_mode ? false : true;
        RTC.read(currentTime);
        showWatchFace(false);
        return;
    }
}


void WatchyAnalog::drawWatchFace(){
    WatchyBase::drawWatchFace();
    if(watchFaceDisabled()){
        return;
    }

    display.drawBitmap(0, 0, analog, 200, 200, FOREGROUND_COLOR);
    drawDate();
    drawSteps();
    drawBattery();
    drawAlarm();
    drawTime();

    //drawHelperGrid();
}


void WatchyAnalog::printCentered(uint16_t x, uint16_t y, String text){
    int16_t  x1, y1;
    uint16_t w, h;

    display.getTextBounds(text, 40, 100, &x1, &y1, &w, &h);
    display.setCursor(x-w/2, y+h/2);
    display.println(text);
}


void WatchyAnalog::drawTime(){
    int theHour = currentTime.Hour;
    int theMinute = currentTime.Minute;

    // minute hand
    drawBitmapRotate(100,100, minute_hand_inv, theMinute * 6, GxEPD_WHITE);
    drawBitmapRotate(100,100, minute_hand_inv, theMinute * 6, GREY);
    drawBitmapRotate(100,100, minute_hand, theMinute * 6, GxEPD_BLACK);

    // hour hand
    int hourAngle = ((theHour%12)*60 + theMinute) * 360 / 720;
    drawBitmapRotate(100,100, hour_hand_inv, hourAngle, GxEPD_WHITE);
    drawBitmapRotate(100,100, hour_hand_inv, hourAngle, GREY);
    drawBitmapRotate(100,100, hour_hand, hourAngle, GxEPD_BLACK);

    display.fillCircle(100,100, 13, BACKGROUND_COLOR);
    display.fillCircle(100,100, 9, FOREGROUND_COLOR);
    display.fillCircle(100,100, 5, BACKGROUND_COLOR);
}


void WatchyAnalog::drawDate(){
    display.setFont(&FONT);
    display.setTextColor(FOREGROUND_COLOR);

    String dayOfWeek = dayShortStr(currentTime.Wday);
    String dayStr = String(currentTime.Day);
    dayStr = currentTime.Day < 10 ? "0" + dayStr : dayStr;
    printCentered(145, 85, dayOfWeek);
    printCentered(145, 110, dayStr);
}

void WatchyAnalog::drawAlarm(){
    display.setFont(&FONT);
    display.setTextColor(FOREGROUND_COLOR);

    if (alarm_timer >= 0){
        printCentered(100, 45, "T-" + String(alarm_timer) + " min.");
    } else{
        printCentered(100, 40, "Watchy");
    }
}


void WatchyAnalog::drawBattery(){
    display.setFont(&FONT);
    display.setTextColor(FOREGROUND_COLOR);

    int8_t bat = getBattery();
    bat = bat >= 100 ? 99 : bat;
    String batStr = String(bat);
    batStr = bat < 10 ? "0" + batStr : batStr;
    printCentered(60, 100, batStr + "%");
}


void WatchyAnalog::drawSteps(){
    display.setFont(&FONT);
    display.setTextColor(FOREGROUND_COLOR);

    bool rtc_alarm = wakeup_reason == ESP_SLEEP_WAKEUP_EXT0;

    // Whenever we have a new hour, we can restart our step counting.
    // But only if its an rtc alarm - ignore button press etc.
    if(rtc_alarm && currentTime.Minute == 0 && currentTime.Hour == 0){
        sensor.resetStepCounter();
    }

    uint32_t steps = sensor.getCounter();
    printCentered(100, 150, String(steps));
}