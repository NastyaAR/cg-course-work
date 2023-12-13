QT       += core gui opengl testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    baseobject.cpp \
    camera.cpp \
    errmsg.cpp \
    glwidget.cpp \
    light.cpp \
    main.cpp \
    mainwindow.cpp \
    testcamera.cpp \
    testlight.cpp

HEADERS += \
    baseobject.h \
    camera.h \
    errmsg.h \
    glwidget.h \
    light.h \
    mainwindow.h \
    mydialog.h \
    structures.h \
    testcamera.h \
    testlight.h


FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    shaders.qrc
