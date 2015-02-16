#-------------------------------------------------
#
# Project created by QtCreator 2015-01-13T22:04:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = filebrowser
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    directoryscanner.cpp \
    directorychooser.cpp

HEADERS  += mainwindow.h \
    directoryscanner.h \
    directorychooser.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../filecleaner/release/ -lfilecleaner
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../filecleaner/debug/ -lfilecleaner
else:unix: LIBS += -L$$PWD/../filecleaner/ -lfilecleaner

INCLUDEPATH += $$PWD/../filecleaner
DEPENDPATH += $$PWD/../filecleaner
