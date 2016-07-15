QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Unnamed
TEMPLATE = app

SOURCES += src/main.cpp\
        src/ui/mainwindow.cpp \
        src/core/application.cpp \
    src/ui/glwindow.cpp \
    src/ui/graphicswindow.cpp \
    src/core/globject.cpp \
    src/core/triangle.cpp

HEADERS  += src/ui/mainwindow.h \
         src/core/application.h \
    src/ui/glwindow.h \
    src/ui/graphicswindow.h \
    src/core/globject.h \
    src/core/triangle.h

FORMS    += src/ui/mainwindow.ui

DISTFILES += \
    bin/shaders/basic/triangle.vert \
    bin/shaders/basic/triangle.frag

RESOURCES +=
