QT += core gui widgets testlib

# Define the target
TARGET = DijkstraVisualiserTest
TEMPLATE = app

# Include the necessary directories
INCLUDEPATH += ../DijkstraVisualiser
DEPENDPATH += ../DijkstraVisualiser

# Add the source and header files
SOURCES += test_edge.cpp \
           main.cpp \
           test_node.cpp \
           test_widget.cpp

# Link against the main project library
LIBS += -L$$OUT_PWD/../build-DijkstraVisualiser-Desktop_arm_darwin_generic_mach_o_64bit-Release -lDijkstraVisualiser

# Ensure the main project is built first
PRE_TARGETDEPS += ../build-DijkstraVisualiser-Desktop_arm_darwin_generic_mach_o_64bit-Release/libDijkstraVisualiser.a

# Google Test
INCLUDEPATH += $$PWD/../googletest/googletest/include
LIBS += -L$$PWD/../googletest/build/lib -lgtest -lgtest_main
