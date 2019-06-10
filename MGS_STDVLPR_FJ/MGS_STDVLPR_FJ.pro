#-------------------------------------------------
#
# Project created by QtCreator 2019-06-09T14:17:41
#
#-------------------------------------------------

QT       -= core gui

TARGET = MGS_STDVLPR_FJ
TEMPLATE = lib

DEFINES += MGS_STDVLPR_FJ_LIBRARY

SOURCES += mgs_stdvlpr_fj.cpp \
    cameraModule/BaseCamera.cpp \
    cameraModule/Camera6467_plate.cpp \
    cameraModule/CameraResult.cpp \
    cameraModule/ResultListManager.cpp \
    tinyxml/tinystr.cpp \
    tinyxml/tinyxml.cpp \
    tinyxml/tinyxmlerror.cpp \
    tinyxml/tinyxmlparser.cpp \
    utilityTool/tool_function.cpp \
    utilityTool/encode.c \
    utilityTool/inifile.c \
    cameraModule/DeviceListManager.cpp

HEADERS += mgs_stdvlpr_fj.h\
        mgs_stdvlpr_fj_global.h \
    CommonDef.h \
    cameraModule/BaseCamera.h \
    cameraModule/Camera6467_plate.h \
    cameraModule/CameraResult.h \
    cameraModule/ResultListManager.h \
    libHvDevice/HVAPI_HANDLE_CONTEXT_EX.h \
    libHvDevice/HvDeviceBaseType.h \
    libHvDevice/HvDeviceCommDef.h \
    libHvDevice/HvDeviceEx.h \
    libHvDevice/swerror.h \
    tinyxml/tinystr.h \
    tinyxml/tinyxml.h \
    utilityTool/encode.h \
    utilityTool/inifile.h \
    utilityTool/tool_function.h \
    cameraModule/DeviceListManager.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    libHvDevice/HvDevice.lib \
    libHvDevice/libHvdevice.so
