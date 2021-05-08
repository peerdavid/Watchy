#ifndef WATCHY_LINUX_H
#define WATCHY_LINUX_H

#include "Watchy_Base.h"
#include "icons.h"
#include "fonts.h"

class WatchyLinux : public WatchyBase {

    public:
        WatchyLinux();
        void drawWatchFace();
        void handleButtonPress();
};

#endif
