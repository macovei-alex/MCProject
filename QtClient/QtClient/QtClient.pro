QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    canvaspaint.cpp \
    main.cpp \
    mainwindow.cpp \
    MyLine.cpp \
    ImageReceiverThread.cpp \
    GameStateReceiverThread.cpp

HEADERS += \
    canvaspaint.h \
    macro.h \
    mainwindow.h \
    ui_canvaspaint.h \
    ui_mainwindow.h \
    DrawingState.h \
    MyLine.h \
    ImageReceiverThread.h \
    GameStateReceiverThread.h

FORMS += \
    canvaspaint.ui \
    mainwindow.ui
	
INCLUDEPATH += \
	../../Common \
	../../ClientServices

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
