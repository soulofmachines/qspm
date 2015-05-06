#ifndef DBUS_HPP
#define DBUS_HPP

#include <QString>

bool canSuspend();
bool canHibernate();

bool allowedStop();
bool allowedRestart();
bool allowedSuspend();
bool allowedHibernate();

bool batteryOn();
int batteryPercentage();

void notify(QString title, QString text);

void stop();
void restart();
void suspend();
void hibernate();

#endif //DBUS_HPP
