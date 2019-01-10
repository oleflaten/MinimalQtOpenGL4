QT          += core gui widgets

TEMPLATE    = app
CONFIG      += c++11

TARGET      = 3D-programming

SOURCES += main.cpp \
    renderwindow.cpp \
    shader.cpp \
    mainwindow.cpp \
    glwidget.cpp

HEADERS += \
    renderwindow.h \
    shader.h \
    mainwindow.h \
    glwidget.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    plainfragment.frag \
    plainvertex.vert
