QT       += core dbus

TARGET = qspm

TEMPLATE = app

SOURCES += main.cpp dbus.cpp xorg.cpp

HEADERS  += dbus.hpp xorg.hpp

LIBS += -lX11 -lXss -lXext
