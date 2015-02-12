#-------------------------------------------------
#
# Project created by QtCreator 2011-02-02T11:06:36
#
#-------------------------------------------------

QT       += core gui network widgets multimedia multimediawidgets

TARGET = omilo
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    installvoicesdialog.cpp \
    progressdialog.cpp \
    speechengineinfo.cpp \
    speechvoice.cpp \
    speechengine.cpp \
    festivalflitevoice.cpp \
    festivalvoice.cpp \
    flitevoice.cpp \
    downloadmanager.cpp \
    maryvoice.cpp \
    startupthread.cpp \
    editorvoiceoptionsdialog.cpp \
    playercontrols.cpp \
    playlistmodel.cpp \
    flitesettingsdialog.cpp \
    hotkeythread.cpp

HEADERS  += mainwindow.h \
    installvoicesdialog.h \
    progressdialog.h \
    speechengineinfo.h \
    speechvoice.h \
    speechengine.h \
    definitions.h \
    festivalflitevoice.h \
    festivalvoice.h \
    flitevoice.h \
    downloadmanager.h \
    maryvoice.h \
    startupthread.h \
    editorvoiceoptionsdialog.h \
    playercontrols.h \
    playlistmodel.h \
    flitesettingsdialog.h \
    hotkeythread.h

FORMS    += mainwindow.ui \
    installvoicesdialog.ui \
    progressdialog.ui \
    installorcamoduledialog.ui \
    editorvoiceoptionsdialog.ui \
    flitesettingsdialog.ui

CONFIG += mobility
MOBILITY = 

RESOURCES = omilo.qrc \
    translations.qrc

TRANSLATIONS = omilo_el_GR.ts \
               omilo_de.ts

DISTFILES += \
    images/toggle-toolbars.png

LIBS += -lX11
