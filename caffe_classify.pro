QT += core
QT -= gui

TARGET = caffe_classify
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    classifier.cpp \
    ctrain.cpp \
    cvxtext.cpp

INCLUDEPATH = /usr/include/ \
/usr/include/opencv \
/usr/include/opencv2 \
/home/joyoung/caffe-master/include \
/usr/local/cuda-7.5/include \
/usr/local/include/freetype2 \


LIBS = /usr/lib/x86_64-linux-gnu/libopencv_*.so \
/usr/lib/x86_64-linux-gnu/libboost_*.so \
/usr/lib/x86_64-linux-gnu/libglog.so \
/usr/local/caffe/libcaffe.so \
/usr/local/cuda-7.5/lib64/*.so \
/usr/local/lib/libfreetype.so \


HEADERS += \
    classifier.h \
    ctrain.h \
    cvxtext.h \

