QT += core gui widgets

CONFIG += c++17

# OpenSSL include and library paths
INCLUDEPATH += "C:/OpenSSL-Win64/include" \
               "C:/Qt/Tools/OpenSSL-Win64/include"

LIBS += -LC:/Qt/Tools/OpenSSL-Win64/lib/VC/x64/MD \
        -lssl -lcrypto -lws2_32 -lcrypt32 -pthread

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    pannableWidget.cpp \
    RoamMateReq.cpp

HEADERS += \
    mainwindow.h \
    pannableWidget.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

QMAKE_CXXFLAGS += -v

