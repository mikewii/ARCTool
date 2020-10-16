QT -= gui

LIBS += -lz -lstdc++fs

CONFIG += c++1z console
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    ARC.hpp \
    types.h \
    utils.h \
    zpipe.h

SOURCES += \
    ARC.cpp \
    main.cpp \
    utils.cpp \
    zpipe.cpp

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
