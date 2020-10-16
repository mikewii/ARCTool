QT -= gui

LIBS += -lz -lstdc++fs

INCLUDEPATH += $$PWD/include

CONFIG += c++1z console
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    include/ARC.hpp \
    include/mod.h \
    include/types.h \
    include/utils.h \
    include/zpipe.h

SOURCES += \
    src/ARC.cpp \
    src/main.cpp \
    src/utils.cpp \
    src/zpipe.cpp

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
