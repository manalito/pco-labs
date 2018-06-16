QT       += core websockets
QT       -= gui

TARGET = fileserver
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp \
    fileserver.cpp \
    filereader.cpp \
    responsedispatcherthread.cpp \
    requestdispatcherthread.cpp \
    requestprocessor.cpp \
    hoaremonitor.cpp \
    readerwritercache.cpp \
    readerwriterlock.cpp

HEADERS += \
    fileserver.h \
    filereader.h \
    response.h \
    abstractbuffer.h \
    request.h \
    responsedispatcherthread.h \
    requestdispatcherthread.h \
    buffern.h \
    requestprocessor.h \
    hoaremonitor.h \
    option.h \
    readerwritercache.h \
    readerwriterlock.h

EXAMPLE_FILES += fileclient.html

