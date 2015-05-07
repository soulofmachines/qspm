#include <QCoreApplication>
#include <QProcess>
#include <QDebug>
#include <unistd.h>
#include "dbus.hpp"
#include "xorg.hpp"

QProcess* lockProcess = new QProcess();

//settings
QString lockCommand = "gnome-screensaver-comman -l";
int lowPercentage = 10, screenTimeout = 300, idleTimeout = 900;

//triggers
bool doLowPercentage = true, doIdle = true, doDPMSon = true, doDPMSoff = true, notifyBattery = true, notifyDPMS = true;

void doDPMS(int timeout) {
    qDebug() << "dpms" << timeout;
    if (dpms()) {
        dpmsSet(0, 0, timeout);
    } else {
        if (notifyDPMS) {
            notifyDPMS = false;
            notify(QCoreApplication::applicationName(),
                   "DPMS not found");
        }
    }
}

bool doLock() {
    qDebug() << "lock screen";
    if (!lockCommand.isEmpty()) {
        if (lockProcess->startDetached(lockCommand)) {
            return true;
        } else {
            notify(QCoreApplication::applicationName(),
                   "Lock screen failed");
            return false;
        }
    } else {
        return true;
    }
}

bool doSuspend() {
    qDebug() << "suspend";
    if (canSuspend() && allowedSuspend()) {
        suspend();
        return true;
    } else {
        notify(QCoreApplication::applicationName(),
               "Can't suspend");
        return false;
    }
}

bool doHibernate() {
    qDebug() << "hibernate";
    if (canHibernate() && allowedHibernate()) {
        hibernate();
        return true;
    } else {
        notify(QCoreApplication::applicationName(),
               "Can't hibernate");
        return false;
    }
}

int main()
{
    QCoreApplication::setApplicationName("Qt Simple Power Manager");

    int percentage = 0, idle = 0;

    qDebug() << "battery on:        " << batteryOn();
    qDebug() << "battery percentage:" << batteryPercentage();
    qDebug() << "can suspend:       " << canSuspend();
    qDebug() << "can hibernate:     " << canHibernate();
    qDebug() << "allowed suspend:   " << allowedSuspend();
    qDebug() << "allowed hibernate: " << allowedHibernate();
    qDebug() << "allowed stop:      " << allowedStop();
    qDebug() << "allowed restart:   " << allowedRestart();

    while (true) {
        if (batteryOn()) {
            percentage = batteryPercentage();
            idle = xorgIdle();
            if (idle < idleTimeout) {
                if (!doIdle) {
                    doIdle = true;
                }
            }
            if (doDPMSon) {
                doDPMSon = false;
                doDPMSoff = true;
                doDPMS(screenTimeout);
            }
            if (percentage >= 0) {
                if (doLowPercentage) {
                    if (percentage <= lowPercentage) {
                        doLowPercentage = false;
                        if (doLock()) {
                            if (!doHibernate()) {
                                doSuspend();
                            }
                        }
                    }
                }
                if (doIdle) {
                    if (idle >= idleTimeout) {
                        doIdle = false;
                        if (doLock()) {
                            doSuspend();
                        }
                    }
                }
            } else {
                if (notifyBattery) {
                    notifyBattery = false;
                    notify(QCoreApplication::applicationName(),
                           "Battery not found");
                }
            }
        } else {
            if (!doLowPercentage) {
                doLowPercentage = true;
            }
            if (doDPMSoff) {
                doDPMSoff = false;
                doDPMSon = true;
                doDPMS(0);
            }
        }
        sleep(30);
    }

    return 0;
}
