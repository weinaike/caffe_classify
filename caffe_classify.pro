QT += core
QT -= gui

TARGET = caffe_classify
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    classifier.cpp \
    ctrain.cpp
INCLUDEPATH=/usr/include/\
/usr/include/opencv\
/usr/include/opencv2\
/home/wnk/caffe-master/include/\


LIBS=/usr/lib/x86_64-linux-gnu/libopencv_*.so\
/usr/lib/x86_64-linux-gnu/libboost_*.so\
/usr/local/lib/libglog.so.0\
/home/wnk/caffe-master/build/lib/libcaffe.so\

HEADERS += \
    classifier.h \
    ctrain.h

