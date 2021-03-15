#ifndef READERDEVICES_H
#define READERDEVICES_H

#include <QList>
#include <QMap>
#include <QObject>
#include "support.h"
#include "xcross.h"
#include "setup.h"
#include "../dbase/devicedata.h"

extern Setup ini;

class ReaderDevices : public QThread
{
    Q_OBJECT
public:
    ReaderDevices();
    void addSubregion(Region region);
    QList<DeviceData> getDevices(Region);
    void run() override;
signals:
    void loaded();
private:
    QMap<int,QString> statuses;
    QList<Region>  needLoaded;
    QMap<QString,QList<DeviceData>> devices;
    QMutex mutex;
    QSqlDatabase db;
};

#endif // READERDEVICES_H
