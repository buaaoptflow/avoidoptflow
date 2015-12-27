QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = avoidOptFlow_qt
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    avoidOptflow.cpp \
    motioncolor.cpp \
    navigation.cpp \
    optcvmatutil.cpp \
    optdrawflow.cpp \
    optfeatureutil.cpp \
    opticalflow.cpp \
    optmatutil.cpp \
    optutil.cpp \
    stdafx.cpp \
    test.cpp \
    video.cpp \
    usepython.cpp \
    gui_main.cpp \
    contours.cpp


INCLUDEPATH+=G:\opencv2.4.9\include\opencv\
                    G:\opencv2.4.9\include\opencv2\
                    G:\opencv2.4.9\include\

INCLUDEPATH += C:\Python27\include
DEPENDPATH += C:\Python27\include

LIBS+=G:\opencv2.4.9\lib\libopencv_calib3d249.dll.a\
        G:\opencv2.4.9\lib\libopencv_contrib249.dll.a\
        G:\opencv2.4.9\lib\libopencv_core249.dll.a\
        G:\opencv2.4.9\lib\libopencv_features2d249.dll.a\
        G:\opencv2.4.9\lib\libopencv_flann249.dll.a\
        G:\opencv2.4.9\lib\libopencv_gpu249.dll.a\
        G:\opencv2.4.9\lib\libopencv_highgui249.dll.a\
        G:\opencv2.4.9\lib\libopencv_imgproc249.dll.a\
        G:\opencv2.4.9\lib\libopencv_legacy249.dll.a\
        G:\opencv2.4.9\lib\libopencv_ml249.dll.a\
        G:\opencv2.4.9\lib\libopencv_objdetect249.dll.a\
        G:\opencv2.4.9\lib\libopencv_video249.dll.a\
        C:\Python27\libs\libpython27.a

HEADERS += \
    imgutil.h \
    motioncolor.h \
    navigation.h \
    optcvmatutil.h \
    optdrawflow.h \
    optfeatureutil.h \
    opticalflow.h \
    optmatutil.h \
    optutil.h \
    stdafx.h \
    targetver.h \
    test.h \
    video.h \
    usepython.h \
    gui_main.h \
    contours.h

FORMS += \
    dialog.ui


