# ====================== PagingSimulator UI (qmake) ======================
QT       += core gui widgets
TEMPLATE  = app
TARGET    = PagingSimulatorUI

# C++ / warnings
CONFIG   += c++20 warn_on
DEFINES  += QT_DEPRECATED_WARNINGS NOMINMAX
win32-g++: QMAKE_CXXFLAGS += -Wall -Wextra -Wpedantic
msvc:      QMAKE_CXXFLAGS += /W4 /permissive- /EHsc

# -------- Core submodule path (relative!) --------
CORE_DIR  = $$clean_path($$PWD/PagingSimulator/src)

INCLUDEPATH += $$CORE_DIR
DEPENDPATH  += $$CORE_DIR

# -------- UI sources --------
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

# -------- Core sources (compiled into the UI app) --------
SOURCES += \
    $${CORE_DIR}/des/Event.cpp \
    $${CORE_DIR}/des/EventQueue.cpp \
    $${CORE_DIR}/core/MemoryAccessEvent.cpp \
    $${CORE_DIR}/Simulation.cpp \
    $${CORE_DIR}/TraceLoader.cpp \
    $${CORE_DIR}/core/algorithms/FIFOAlgorithm.cpp \
    $${CORE_DIR}/core/algorithms/SecondChanceAlgorithm.cpp \
    $${CORE_DIR}/core/algorithms/LRUAlgorithm.cpp \
    $${CORE_DIR}/core/algorithms/NRUAlgorithm.cpp \
    $${CORE_DIR}/core/algorithms/NFUAlgorithm.cpp \
    $${CORE_DIR}/core/algorithms/NFUNoAgingAlgorithm.cpp

HEADERS += \
    $${CORE_DIR}/Simulation.h \
    $${CORE_DIR}/TraceLoader.h \
    $${CORE_DIR}/core/CoreStructs.h \
    $${CORE_DIR}/core/PagingAlgorithm.h \
    $${CORE_DIR}/core/MemoryAccessEvent.h \
    $${CORE_DIR}/des/Event.h \
    $${CORE_DIR}/des/EventQueue.h \
    $${CORE_DIR}/core/algorithms/FIFOAlgorithm.h \
    $${CORE_DIR}/core/algorithms/SecondChanceAlgorithm.h \
    $${CORE_DIR}/core/algorithms/LRUAlgorithm.h \
    $${CORE_DIR}/core/algorithms/NRUAlgorithm.h \
    $${CORE_DIR}/core/algorithms/NFUAlgorithm.h \
    $${CORE_DIR}/core/algorithms/NFUNoAgingAlgorithm.h

# -------- (Optional) copy sample trace file next to the built exe --------
QMAKE_POST_LINK += $$quote($(MKDIR) "$$OUT_PWD/resources") && \
                   $$quote($(COPY_FILE) "$$PWD/resources/trace.txt" "$$OUT_PWD/resources/trace.txt")

# -------- Install target (optional) --------
qnx:  target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
