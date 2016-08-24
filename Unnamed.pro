QT       += core gui opengl gui-private xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Unnamed
TEMPLATE = app

SOURCES = src/main.cpp

DISTFILES += \
    bin/shaders/basic/triangle.vert \
    bin/shaders/basic/triangle.frag \
    bin/shaders/basic/cube.vert

RESOURCES +=

include(src/Core/Core.pri)
include(src/UI/UI.pri)
include(src/WoW/WoW.pri)
