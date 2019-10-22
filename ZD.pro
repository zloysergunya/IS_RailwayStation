QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialog_addrace.cpp \
    dialog_adduser.cpp \
    dialog_addwagon.cpp \
    dialog_buyticket.cpp \
    dialog_controlraces.cpp \
    dialog_controlusers.cpp \
    dialog_controlwagons.cpp \
    dialog_handoverticket.cpp \
    dialog_infouser.cpp \
    dialog_menuadmin.cpp \
    dialog_menucashier.cpp \
    dialog_showtickets.cpp \
    main.cpp \
    mainwidget.cpp \
    ticket.cpp \
    train.cpp \
    user.cpp \
    wagon.cpp \
    window_authorization.cpp \
    window_registration.cpp \
    window_workwidget.cpp

HEADERS += \
    config.h \
    dialog_addrace.h \
    dialog_adduser.h \
    dialog_addwagon.h \
    dialog_buyticket.h \
    dialog_controlraces.h \
    dialog_controlusers.h \
    dialog_controlwagons.h \
    dialog_handoverticket.h \
    dialog_infouser.h \
    dialog_menuadmin.h \
    dialog_menucashier.h \
    dialog_showtickets.h \
    mainwidget.h \
    ticket.h \
    train.h \
    user.h \
    wagon.h \
    window_authorization.h \
    window_registration.h \
    window_workwidget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    dialog_addrace.ui \
    dialog_adduser.ui \
    dialog_addwagon.ui \
    dialog_buyticket.ui \
    dialog_controlraces.ui \
    dialog_controlusers.ui \
    dialog_controlwagons.ui \
    dialog_handoverticket.ui \
    dialog_infouser.ui \
    dialog_menuadmin.ui \
    dialog_menucashier.ui \
    dialog_showtickets.ui \
    window_authorization.ui \
    window_registration.ui \
    window_workwidget.ui
