QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    BrokenBrick.cpp \
    Coin.cpp \
    FinishStageWidget.cpp \
    FlagItem.cpp \
    FloatingCoin.cpp \
    FloorBrick.cpp \
    GameStageWidget.cpp \
    InteractiveBrick.cpp \
    Mario.cpp \
    NormalBrick.cpp \
    PrepareStageWidget.cpp \
    SuperMushroom.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    Brick.h \
    BrokenBrick.h \
    Coin.h \
    FinishStageWidget.h \
    FlagItem.h \
    FloatingCoin.h \
    FloorBrick.h \
    GameStageWidget.h \
    InteractiveBrick.h \
    Item.h \
    MainWindow.h \
    Mario.h \
    NormalBrick.h \
    PrepareStageWidget.h \
    SuperMushroom.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    M2_zh_TW.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    data.qrc
