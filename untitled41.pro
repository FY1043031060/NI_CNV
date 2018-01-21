#-------------------------------------------------
#
# Project created by QtCreator 2017-12-29T10:53:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled41
TEMPLATE = app
LIBS += -L$$PWD -lcvinetv
LIBS += -L$$PWD \
-lcvirt\
    -lcviauto \
    -lcviddc \
    -lcvidotnet \
    -lcvinetstreams \
    -lcvinetv \
    -lcvintwrk \
    -lcvirt \
    -lcvisupp \
    -lcvitdms\
        -lcvi
LIBS += -L$$PWD -lcviauto
INCLUDEPATH += $$PWD

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    CustCNV.cpp \
    CustViewWid.cpp \
    CCnvProxyView.cpp

HEADERS += \
    CustCNV.h \
    CustViewWid.h \
    CCnvProxyView.h

FORMS += \
        CnvWid.ui \
    CustCNVEditor.ui \
    CustViewWid.ui \
    CCnvProxyView.ui
