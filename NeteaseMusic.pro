QT       += core gui network multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NeteaseMusic
TEMPLATE = app

SOURCES += \
    httprequest.cpp \
    logindialog.cpp \
        main.cpp \
        mainwindow.cpp \
    titlebar.cpp \
    controlbar.cpp \
    navwidget.cpp \
    lyricwidget.cpp \
    userinfowidget.cpp

HEADERS += \
    httprequest.h \
    logindialog.h \
        mainwindow.h \
    titlebar.h \
    controlbar.h \
    navwidget.h \
    lyricwidget.h \
    userinfowidget.h

RESOURCES += \
    res.qrc

RC_ICONS = icon\icon.ico
#RC_FILE = res.rc