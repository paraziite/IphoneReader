QT  += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11



LIBS += -L$$PWD/lib -lusbmuxd
LIBS += -L$$PWD/lib -lplist
LIBS += -L$$PWD/lib -limobiledevice


SOURCES += \
    iphonereaderusb.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    iphonereaderusb.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    IO/libimobiledevice/libimobiledevice-1.0.pc.in
