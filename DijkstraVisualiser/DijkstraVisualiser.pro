QT += core gui widgets

# Define the target
TARGET = DijkstraVisualiser
TEMPLATE = app

# Build a static library
CONFIG += staticlib

# Include the necessary directories
INCLUDEPATH += .
DEPENDPATH += .

# Add the source and header files
SOURCES += main.cpp \
           widget.cpp \
           node.cpp \
           edge.cpp

HEADERS += widget.h \
           node.h \
           edge.h

FORMS += \
    widget.ui



# QT       += core gui

# greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# CONFIG += c++17
# QT += quick
# QT += quickcontrols2



# # You can make your code fail to compile if it uses deprecated APIs.
# # In order to do so, uncomment the following line.
# #DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# SOURCES += \
#     edge.cpp \
#     main.cpp \
#     node.cpp \
#     widget.cpp

# HEADERS += \
#     edge.h \
#     node.h \
#     widget.h

# FORMS += \
#     widget.ui

# # Default rules for deployment.
# qnx: target.path = /tmp/$${TARGET}/bin
# else: unix:!android: target.path = /opt/$${TARGET}/bin
# !isEmpty(target.path): INSTALLS += target

# DISTFILES += \
#     psuedo \
#     style.css
