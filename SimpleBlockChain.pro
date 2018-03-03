TEMPLATE = app
CONFIG += console c++11
QT += core
CONFIG -= app_bundle
#CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp \
    block.cpp \
    transaction.cpp \
    blockchain.cpp

HEADERS += \
    block.h \
    transaction.h \
    blockchain.h
