QT += 3dcore 3drender 3dinput 3dextras
QT += widgets core

SOURCES += src/main.cpp \
    src/block/PoseSimulation.cpp \
    src/block/TractorModel.cpp \
    src/block/TrailerModel.cpp \
    src/block/guidancebase.cpp \
    src/gui/CameraToolbar.cpp \
    src/gui/GuidanceToolbar.cpp \
    src/gui/LengthBlockModel.cpp \
    src/gui/SettingsDialog.cpp \
    src/gui/SimulatorToolbar.cpp \
    src/gui/VectorBlockModel.cpp
    
HEADERS += \
    src/block/CameraController.h \
    src/block/DebugSink.h \
    src/block/GuidanceBase.h \
    src/block/LengthObject.h \
    src/block/PoseCache.h \
    src/block/PoseSimulation.h \
    src/block/TractorModel.h \
    src/block/TrailerModel.h \
    src/block/VectorObject.h \
    src/gui/CameraToolbar.h \
    src/gui/GuidanceToolbar.h \
    src/gui/LengthBlockModel.h \
    src/gui/SettingsDialog.h \
    src/gui/SimulatorToolbar.h \
    src/gui/VectorBlockModel.h \
    src/kinematic/FixedKinematic.h \
    src/kinematic/TrailerKinematic.h

#QMAKE_CXXFLAGS += -pg -g

FORMS += \
    src/gui/CameraToolbar.ui \
    src/gui/GuidanceToolbar.ui \
    src/gui/SettingsDialog.ui \
    src/gui/SimulatorToolbar.ui \
    src/gui/lengthwidget.ui \
    src/gui/vectorwidget.ui

RESOURCES += \
    res/ressources.qrc

#in your project main .pro qmake configuration file
#include($$PWD/lib/QuickQanava/src/quickqanava.pri)

include($$PWD/src/qnodeseditor/qnodeeditor.pri)
