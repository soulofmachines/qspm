#ifndef XORG_HPP
#define XORG_HPP

#include <X11/Xmd.h> //define CARD16

unsigned int xorgIdle();
bool dpms();
void dpmsSet(CARD16 timeoutStandby, CARD16 timeoutSuspend, CARD16 timeoutOff);
void dpmsGet(CARD16 &timeoutStandby, CARD16 &timeoutSuspend, CARD16 &timeoutOff);

#endif //XORG_HPP
