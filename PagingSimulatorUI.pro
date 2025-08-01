QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    configurationwindow.cpp \
    simulationwindow.cpp

HEADERS += \
    configurationwindow.h \
    simulationwindow.h

FORMS += \
    configurationwindow.ui \
    simulationwindow.ui

INCLUDEPATH += $$PWD/PagingSimulator/src
SOURCES += \
    $$PWD/PagingSimulator/src/des/Event.cpp \
    $$PWD/PagingSimulator/src/des/EventQueue.cpp \
    $$PWD/PagingSimulator/src/core/MemoryAccessEvent.cpp \
    $$PWD/PagingSimulator/src/Simulation.cpp \
    $$PWD/PagingSimulator/src/core/Algorithms/FIFOAlgorithm.cpp \
    $$PWD/PagingSimulator/src/core/Algorithms/SecondChanceAlgorithm.cpp \
    $$PWD/PagingSimulator/src/core/Algorithms/LRUAlgorithm.cpp \
    $$PWD/PagingSimulator/src/core/Algorithms/NRUAlgorithm.cpp \
    $$PWD/PagingSimulator/src/core/Algorithms/NFUAlgorithm.cpp \
    $$PWD/PagingSimulator/src/TraceLoader.cpp \
    $$PWD/PagingSimulator/src/core/Algorithms/NFUNoAgingAlgorithm.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
