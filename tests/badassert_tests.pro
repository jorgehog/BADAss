CONFIG -= app_bundle
CONFIG -= qt

TEMPLATE = app

SOURCES += \
    badass_tests.cpp

LIBS += -lunittest++

QMAKE_CXXFLAGS += -std=c++11

OTHER_FILES +=

HEADERS += \
    ../src/badass_meat.h \
    ../badass.h
