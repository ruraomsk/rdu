#ifndef VIEWDEVICE_H
#define VIEWDEVICE_H

#include <QGridLayout>
#include <QObject>
#include <QTableWidget>
#include <QWidget>
#include "reciver/readerdevices.h"

class ViewDevice : public QWidget
{
    Q_OBJECT
public:
    ViewDevice(ReaderDevices *reader,Region region);
public slots:
    void DeviceUpdate();
signals:
private:
    void table();
    ReaderDevices *reader;
    Region region;
    QTableWidget *wtable;
    QGridLayout *grid;
};

#endif // VIEWDEVICE_H
