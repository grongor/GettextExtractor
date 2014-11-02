TEMPLATE = app
TARGET = GettextExtractor
CONFIG += c++11

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    main.cpp \
    extractor.cpp \
    writer.cpp \
    parser.cpp \
    mainWindow.cpp \
    configManager.cpp

HEADERS += \
    extractor.h \
    writer.h \
    parser.h \
    mainWindow.h \
    configManager.h

FORMS += \
    mainWindow.ui
