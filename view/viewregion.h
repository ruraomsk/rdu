#ifndef VIEWREGION_H
#define VIEWREGION_H

#include <QGridLayout>
#include <QObject>
#include <QWidget>
#include <QTableWidget>
#include "viewmessages.h"
#include "reciver/reciver.h"
#include "reciver/readerdevices.h"
#include "../streetBox/xctrl/state.h"
#include "setup.h"

extern Setup ini;

class ViewRegion : public QWidget
{
    Q_OBJECT
public:
    ViewRegion();
    ViewRegion(Reciver *reciver,ReaderDevices *reader,int region);
public slots:
    void Update();
    void DeviceUpdate();
signals:
private:
    void table();
    Reciver *reciver;
    ReaderDevices *reader;
    QList<State> states;
    QList<Region> regions;
    QTableWidget *wtable;
    QGridLayout *grid;
    ViewMessages *messages;
    int region;

};

#endif // VIEWREGION_H
