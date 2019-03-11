#-------------------------------------------------
#
# Project created by QtCreator 2019-02-24T08:22:01
#
#-------------------------------------------------

QT       -= gui

TARGET = LibDriverOracle
TEMPLATE = lib

DEFINES  += LIBDRIVERORACLE_LIBRARY
CONFIG   += c++11 plugin shared resources_big

#JAVA
INCLUDEPATH += "C:\Program Files\Java\jdk1.8.0_191\include"
INCLUDEPATH += "C:\Program Files\Java\jdk1.8.0_191\include\win32"
INCLUDEPATH += "C:\Program Files\Java\jdk1.8.0_191\include\win32\bridge"

DESTDIR += D:\C++\workespaceqt\build-WebMSVC-MSVC_STATIC-Release

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
    environment.cpp \
    resultset.cpp \
    dbj.cpp \
    statement.cpp

HEADERS += \
        libdriveroracle_global.h \ 
    environment.h \
    resultset.h \
    dbj.h \
    statement.h \
    oracle.hpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    qml.qrc

DISTFILES += \
    java/lib/ojdbc5.jar \
    java/DriverDB.jar
