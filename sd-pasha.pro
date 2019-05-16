TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -llapack -lblas -larmadillo

SOURCES += main.cpp \
    custom.cpp \
    integrator.cpp \
    model.cpp \
    normalgaussgenerator.cpp \
    TQuaternion.cpp \
    measure_rework.cpp \
    trajectory_processing.cpp \
    tmatrix.cpp \
    tvector.cpp \
    mnk.cpp

HEADERS += \
    custom.h \
    integrator.h \
    model.h \
    measure.h \
    normalgaussgenerator.h \
    TQuaternion.h \
    measure_rework.h \
    trajectory_processing.h \
    tmatrix.h \
    tvector.h \
    mnk.h
