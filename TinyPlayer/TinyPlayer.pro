# -------------------------------------------------
# Project created by QtCreator 2009-02-20T10:47:14
# -------------------------------------------------
TARGET = TinyPlayer
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    VLCException.cpp \
    VLCInstance.cpp \
    VLCMediaPlayer.cpp \
    VLCMedia.cpp
HEADERS += mainwindow.h \
    VLCMedia.h \
    VLCInstance.h \
    VLCMediaPlayer.h \
    VLCException.h
FORMS += mainwindow.ui
INCLUDEPATH += /usr/local/include/vlc \
    /usr/local/include/vlc/plugins
LIBS = -L/usr/local/ \
    -lvlc
