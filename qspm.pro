QT       += core dbus

TARGET = qspm

TEMPLATE = app

SOURCES += main.cpp acpi.cpp dbus.cpp xorg.cpp

HEADERS  += acpi.hpp dbus.hpp xorg.hpp

LIBS += -lX11 -lXss -lXext
