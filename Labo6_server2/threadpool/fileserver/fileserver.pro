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
    requesthandler.cpp \
    requestdispatcherthread.cpp \
    responsedispatcherthread.cpp \
    threadpool.cpp

HEADERS += \
    fileserver.h \
    filereader.h \
    response.h \
    abstractbuffer.h \
    request.h \
    requesthandler.h \
    requestdispatcherthread.h \
    responsedispatcherthread.h \
    threadpool.h \
    buffern.h \
    runnable.h

EXAMPLE_FILES += fileclient.html

