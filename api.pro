QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = library
TEMPLATE = app

SOURCES += main.cpp \
           mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

# Add the following lines for libcurl
INCLUDEPATH += C:/Coding/curl/include
LIBS += -LC:/Coding/curl/lib -lcurl
