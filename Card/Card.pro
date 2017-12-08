#-------------------------------------------------
#
# Project created by QtCreator 2017-11-30T12:43:43
#
#-------------------------------------------------

QT       += widgets
QT       += network
#DESTDIR = bin
TARGET = Card
TEMPLATE = lib

DEFINES += CARD_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        card.cpp \
    monstercard.cpp \
    magiccard.cpp \
    armscard.cpp \
    magicdeck.cpp \
    monsterdeck.cpp \
    armsdeck.cpp

HEADERS += \
        card.h \
        card_global.h \  
    monstercard.h \
    magiccard.h \
    armscard.h \
    magicdeck.h \
    monsterdeck.h \
    armsdeck.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
