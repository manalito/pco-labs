QT += widgets

HEADERS       = mandelbrotwidget.h \
                renderthread.h \
    computethread.h
SOURCES       = main.cpp \
                mandelbrotwidget.cpp \
                renderthread.cpp \
    computethread.cpp

unix:!mac:!vxworks:!integrity:LIBS += -lm

# install
target.path = $$[QT_INSTALL_EXAMPLES]/threads/mandelbrot
INSTALLS += target
