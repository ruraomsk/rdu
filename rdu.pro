QT       += core gui network sql charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dbase/areadata.cpp \
    dbase/commanddata.cpp \
    dbase/devicedata.cpp \
    dbase/regiondata.cpp \
    dbase/subareadata.cpp \
    main.cpp \
    mainwindow.cpp \
    reciver/readerdevices.cpp \
    reciver/reciver.cpp \
    reciver/xcross.cpp \
    setup.cpp \
    support.cpp \
    tree/control.cpp \
    view/bogko.cpp \
    view/viewarea.cpp \
    view/viewareal.cpp \
    view/viewcommand.cpp \
    view/viewcross.cpp \
    view/viewdevice.cpp \
    view/viewmessages.cpp \
    view/viewpoints.cpp \
    view/viewregion.cpp \
    view/viewresult.cpp \
    view/viewresultgraph.cpp \
    view/viewsetup.cpp \
    view/viewstate.cpp \
    view/viewstrategy.cpp \
    view/viewvoronoi.cpp \
    view/viewxctrl.cpp \
    view/voronoi.cpp \
    ../streetBox/xctrl/state.cpp \
    ../streetBox/xctrl/prioryty.cpp \
    ../streetBox/xctrl/xctrl.cpp


HEADERS += \
    dbase/areadata.h \
    dbase/commanddata.h \
    dbase/devicedata.h \
    dbase/regiondata.h \
    dbase/subareadata.h \
    mainwindow.h \
    reciver/readerdevices.h \
    reciver/reciver.h \
    reciver/xcross.h \
    setup.h \
    support.h \
    tree/control.h \
    view/bogko.h \
    view/viewarea.h \
    view/viewareal.h \
    view/viewcommand.h \
    view/viewcross.h \
    view/viewdevice.h \
    view/viewmessages.h \
    view/viewpoints.h \
    view/viewregion.h \
    view/viewresult.h \
    view/viewresultgraph.h \
    view/viewsetup.h \
    view/viewstate.h \
    view/viewstrategy.h \
    view/viewvoronoi.h \
    view/viewxctrl.h \
    view/voronoi.h \
    ../streetBox/xctrl/state.h \
    ../streetBox/xctrl/prioryty.h \
    ../streetBox/xctrl/xctrl.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    images/addXtEmpty.png \
    images/addXtFromJson.png \
    images/addcross.png \
    images/bd.png \
    images/checkbox-checked.png \
    images/checkbox.png \
    images/csv.png \
    images/editcopy.png \
    images/editcut.png \
    images/editpaste.png \
    images/editredo.png \
    images/editundo.png \
    images/exportpdf.png \
    images/filenew.png \
    images/fileopen.png \
    images/fileprint.png \
    images/filesave.png \
    images/format-indent-less.png \
    images/format-indent-more.png \
    images/removeCross.png \
    images/setup.png \
    images/streetBox.png \
    images/textbold.png \
    images/textcenter.png \
    images/textitalic.png \
    images/textjustify.png \
    images/textleft.png \
    images/textright.png \
    images/textunder.png \
    images/zoomin.png \
    images/zoomout.png \
    rdu.ini
