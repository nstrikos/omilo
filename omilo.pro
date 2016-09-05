#-------------------------------------------------
#
# Project created by QtCreator 2011-02-02T11:06:36
#
#-------------------------------------------------

QT       += core gui network widgets multimedia multimediawidgets

TARGET = omilo-exec
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
    textprocess.cpp \
    fontsettingsdialog.cpp \
    soundfilesmerger.cpp \
    emptyfilesremover.cpp \
    tempfilesremover.cpp \
    settingswriter.cpp \
    exportprogressdialog.cpp \
    displaymessagedialog.cpp \
    customfestivaldialog.cpp \
    customfestivalvoice.cpp \
    maryserverforwindows.cpp \
    hotkeythreadforwindows.cpp \

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
    textprocess.h \
    fontsettingsdialog.h \
    soundfilesmerger.h \
    emptyfilesremover.h \
    tempfilesremover.h \
    settingswriter.h \
    exportprogressdialog.h \
    displaymessagedialog.h \
    customfestivaldialog.h \
    customfestivalvoice.h \
    maryserverforwindows.h \
    hotkeythreadforwindows.h \

FORMS    += mainwindow.ui \
    installvoicesdialog.ui \
    progressdialog.ui \
    editorvoiceoptionsdialog.ui \
    flitesettingsdialog.ui \
    fontsettingsdialog.ui \
    exportprogressdialog.ui \
    displaymessagedialog.ui \
    customfestivaldialog.ui

CONFIG += mobility
MOBILITY = 

RESOURCES = omilo.qrc \
    translations.qrc

TRANSLATIONS = omilo_el_GR.ts \
               omilo_de.ts

DISTFILES += \
    images/toggle-toolbars.png

unix: {
    SOURCES += hotkeythread.cpp
    SOURCES -= maryserverforwindows.cpp
    SOURCES -= hotkeythreadforwindows.cpp
    HEADERS += hotkeythread.h
    HEADERS -= maryserverforwindows.h
    HEADERS -= hotkeythreadforwindows.h
    LIBS += -lX11
}
