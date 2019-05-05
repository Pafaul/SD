TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    custom.cpp \
    integrator.cpp \
    measure.cpp \
    model.cpp \
    normalgaussgenerator.cpp \
    TQuaternion.cpp \
    measure_rework.cpp \
    trajectory_processing.cpp \
    tmatrix.cpp \
    tvector.cpp

HEADERS += \
    custom.h \
    integrator.h \
    measure.h \
    model.h \
    normalgaussgenerator.h \
    TQuaternion.h \
    measure_rework.h \
    trajectory_processing.h \
    tmatrix.h \
    tvector.h
