#ifndef VIEWSTATE_H
#define VIEWSTATE_H

#include <QObject>
#include <QTabWidget>
#include <QWidget>
#include "viewdevice.h"
#include "reciver/reciver.h"
#include "reciver/readerdevices.h"
#include "viewxctrl.h"
class ViewState : public QWidget
{
    Q_OBJECT
public:
    ViewState();
    ViewState(Reciver *reciver,ReaderDevices *reader,Region region);
public slots:
    void Update();
    void DeviceUpdate();
signals:

private:
    Region region;
    Reciver *reciver;
    ReaderDevices *reader;
    ViewDevice *devices;
    QTabWidget *tab;            //основной виджет редактора
    State state;
};

#endif // VIEWSTATE_H
