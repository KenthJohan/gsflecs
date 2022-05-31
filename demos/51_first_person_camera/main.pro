TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DEFINES = ""
DEFINES += __USE_MINGW_ANSI_STDIO=1
DEFINES += GS_GUI_IMPL

QMAKE_CFLAGS = ""
QMAKE_CFLAGS += -Wno-unused-function
QMAKE_CFLAGS += -Wno-unused-parameter
QMAKE_CFLAGS += -Wno-missing-braces
QMAKE_CFLAGS += -Wno-missing-field-initializers
QMAKE_CFLAGS += -std=gnu99 -w

INCLUDEPATH += ../../third_party
INCLUDEPATH += ../../

SOURCES += source/main.c
SOURCES += ../../flecs.c
SOURCES += ../../eg_gunslinger.c
SOURCES += ../../eg_quantity.c
SOURCES += ../../eg_geometry.c
SOURCES += ../../eg_camera.c



#LIBS += -lmingw32 -lSDL2main -lSDL2 -lws2_32
LIBS += -lws2_32
LIBS += -lopengl32
LIBS += -lkernel32
LIBS += -luser32
LIBS += -lshell32
LIBS += -lgdi32
LIBS += -lAdvapi32
LIBS += -lWinmm
LIBS += -lm
