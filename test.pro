QT += widgets
QT += printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

win32: QMAKE_CXXFLAGS += -Wa,-mbig-obj

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainclass.cpp \
    qcustomplot.cpp

HEADERS += \
    mainclass.h \
    qcustomplot.h

FORMS += \
    mainclass.ui

TRANSLATIONS += \
    test_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../../..//Users/user/Downloads/QCustomPlot.tar.gz
