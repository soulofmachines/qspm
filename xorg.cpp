#include <X11/extensions/scrnsaver.h>
#include <X11/extensions/dpms.h>
#include "xorg.hpp"

Display* dpy = XOpenDisplay(NULL);
XScreenSaverInfo *ssi = XScreenSaverAllocInfo();

unsigned int xorgIdle() {
    XScreenSaverQueryInfo(dpy, DefaultRootWindow(dpy), ssi);
    return ssi->idle/1000;
}

bool dpms() {
    if (DPMSCapable(dpy)) {
        return true;
    } else {
        return false;
    }
}

void dpmsSet(CARD16 timeoutStandby, CARD16 timeoutSuspend, CARD16 timeoutOff) {
    if (DPMSCapable(dpy)) {
        DPMSEnable(dpy);
        DPMSSetTimeouts(dpy, timeoutStandby, timeoutSuspend, timeoutOff);
        XFlush(dpy);
    }
}

void dpmsGet(CARD16 &timeoutStandby, CARD16 &timeoutSuspend, CARD16 &timeoutOff) {
    if (DPMSCapable(dpy)) {
        DPMSGetTimeouts(dpy, &timeoutStandby, &timeoutSuspend, &timeoutOff);
    } else {
        timeoutStandby = 0;
        timeoutSuspend = 0;
        timeoutOff = 0;
    }
}
