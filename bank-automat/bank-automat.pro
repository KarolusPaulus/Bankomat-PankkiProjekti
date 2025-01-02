QT       += core gui serialport widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += network

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    env.cpp \
    transaction.cpp \
    profilewindow.cpp \
    transactionHistory.cpp

HEADERS += \
    mainwindow.h \
    env.h \
    transaction.h \
    profilewindow.h \
    transactionHistory.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    myStyle.qss \
    styles/myStyle.qss \
    logo.png
    deco.png
    card.png

RESOURCES += \
    Retest.qrc

win32: LIBS += -L$$PWD/../pincodeDLL/build/debug/ -lpincodeDLL

INCLUDEPATH += $$PWD/../pincodeDLL
DEPENDPATH += $$PWD/../pincodeDLL
