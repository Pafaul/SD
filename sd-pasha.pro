TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    custom.cpp \
    integrator.cpp \
    LA.cpp \
    measure.cpp \
    model.cpp \
    normalgaussgenerator.cpp \
    TQuaternion.cpp \
    measure_rework.cpp

HEADERS += \
    custom.h \
    integrator.h \
    LA.h \
    measure.h \
    model.h \
    normalgaussgenerator.h \
    TQuaternion.h \
    measure_rework.h
