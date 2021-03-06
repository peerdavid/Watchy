#ifndef WATCHY_BTTF_H
#define WATCHY_BTTF_H

#include "Watchy_Base.h"
#include "icons.h"
#include "fonts.h"
#include "Images/bttf.h"

class WatchyBTTF : public WatchyBase {

    public:
        WatchyBTTF();
        void drawWatchFace();
        void drawTime();
        void drawDate();
        void drawSteps();
        void drawBattery();
        void handleButtonPress();

    private:
        void printCentered(uint16_t x, uint16_t y, String text);
        void drawCircle(int16_t x0, int16_t y0, int16_t r,
                        uint16_t color, uint8_t width);

};

#endif
