TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    custom.cpp \
    integrator.cpp \
    LA.cpp \
    model.cpp \
    TQuaternion.cpp \
    normalgaussgenerator.cpp \
    measure.cpp

HEADERS += \
    custom.h \
    integrator.h \
    LA.h \
    model.h \
    test.h \
    TQuaternion.h \
    normalgaussgenerator.h \
    measure.h
