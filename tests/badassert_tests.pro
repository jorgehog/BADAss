CONFIG -= app_bundle
CONFIG -= qt

TEMPLATE = app

SOURCES += badassert_tests.cpp

LIBS += -lunittest++

QMAKE_CXXFLAGS += -std=c++11

OTHER_FILES += ../badassert.h

HEADERS += \
    ../src/badassert_meat.h
