#include <QtDBus>
#include "dbus.hpp"

QDBusInterface consoleKit("org.freedesktop.ConsoleKit",
                          "/org/freedesktop/ConsoleKit/Manager",
                          "org.freedesktop.ConsoleKit.Manager",
                          QDBusConnection::systemBus());
QDBusInterface uPower("org.freedesktop.UPower",
                      "/org/freedesktop/UPower",
                      "org.freedesktop.UPower",
                      QDBusConnection::systemBus());
QDBusInterface notification("org.freedesktop.Notifications",
                            "/org/freedesktop/Notifications",
                            "org.freedesktop.Notifications",
                            QDBusConnection::sessionBus());
QDBusInterface battery("org.freedesktop.UPower",
                       "/org/freedesktop/UPower/devices/battery_BAT0",
                       "org.freedesktop.UPower.Device",
                       QDBusConnection::systemBus());

bool canSuspend() {
    return uPower.property("CanSuspend").toBool();
}

bool canHibernate() {
    return uPower.property("CanHibernate").toBool();
}

bool allowedStop() {
    return consoleKit.call("CanStop").arguments().at(0).toBool();
}

bool allowedRestart() {
    return consoleKit.call("CanRestart").arguments().at(0).toBool();
}

bool allowedSuspend() {
    return uPower.call("SuspendAllowed").arguments().at(0).toBool();
}

bool allowedHibernate() {
    return uPower.call("HibernateAllowed").arguments().at(0).toBool();
}

bool batteryOn() {
    return uPower.property("OnBattery").toBool();
}

int batteryPercentage() {
    bool ok = true;
    int perc = battery.property("Percentage").toInt(&ok);
    if (ok) {
        return perc;
    } else {
        return -1;
    }
}

void notify(QString title, QString text) {
    QList<QVariant> arg;
    arg << (QCoreApplication::applicationName()) //appname
        << ((unsigned int) 0) //id
        << QString() //icon
        << title //summary
        << text //body
        << QStringList() //actions
        << QVariantMap() //hints
        << (int) 5000; //timeout
    notification.callWithArgumentList(QDBus::AutoDetect, "Notify", arg);
}

void stop() {
    consoleKit.call("Stop");
}

void restart() {
    consoleKit.call("Stop");
}

void suspend() {
    uPower.call("Suspend");
}

void hibernate() {
    uPower.call("Hibernate");
}
