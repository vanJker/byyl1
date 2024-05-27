QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CodeGen.cpp \
    DFA.cpp \
    FileProcessor.cpp \
    Lexer.cpp \
    MinDFA.cpp \
    NFA.cpp \
    Regex.cpp \
    TotalDFA.cpp \
    main.cpp
#    main.cpp \
 #    mainwindow.cpp

HEADERS += \
    CodeGen.h \
    DFA.h \
    FileProcessor.h \
    Lexer.h \
    MinDFA.h \
    NFA.h \
    Regex.h \
    TotalDFA.h
 #    mainwindow.h

FORMS += \
 \#    mainwindow.ui
    LexicalAnalysis.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
