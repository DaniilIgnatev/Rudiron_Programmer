QT -= gui
QT += serialport

CONFIG += c++11 console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

macx{
    ICON = icon.icns
}
else{
    RC_ICONS = icon.ico
}


SOURCES += \
        argumentsparser.cpp \
        hexflashparser.cpp \
        hexparser.cpp \
        hexramparser.cpp \
        main.cpp \
        programmer.cpp \
        programmerarguments.cpp \
        uart.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    TypeDefs.hpp \
    argumentsparser.h \
    hexflashparser.hpp \
    hexparser.hpp \
    hexramparser.hpp \
    programmer.hpp \
    programmerarguments.h \
    uart.hpp

DISTFILES += \
    icon.icns \
    icon.ico

RESOURCES += \
    qml.qrc
