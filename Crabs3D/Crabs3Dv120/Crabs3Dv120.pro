#-------------------------------------------------
#
# Project created by QtCreator 2017-11-28T19:25:18
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Crabs3Dv120
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        main_window.cpp \
    read_c3d.cpp \
    glwidget.cpp \
    kmeans.cpp \
    cluster_options.cpp \
    crabs_editor.cpp \
    model.cpp \
    model_create_dialog.cpp \
    unit_dialog.cpp \
    set_bones.cpp

HEADERS += \
        main_window.h \
    read_c3d.h \
    glwidget.h \
    kmeans.h \
    cluster_options.h \
    crabs_editor.h \
    model.h \
    model_create_dialog.h \
    unit_dialog.h \
    set_bones.h

FORMS += \
        main_window.ui \
    cluster_options.ui \
    crabs_editor.ui \
    model_create_dialog.ui \
    unit_dialog.ui \
    set_bones.ui

RESOURCES += \
    resource.qrc

LIBS += -lGL -lGLEW -lglut -lGLU
