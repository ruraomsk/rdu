#ifndef SUBAREADATA_H
#define SUBAREADATA_H

#include <reciver/readerdevices.h>
#include <reciver/reciver.h>
#include "streetBox/state.h"
#include "devicedata.h"


class SubAreaData : public QObject
{
    Q_OBJECT
public:
    SubAreaData();
    SubAreaData(Reciver *reciver,ReaderDevices *reader,Region region);
    int CountDevices();
    int CountConnected();
    int CountWorked();
    void SetCommand(CommandData cmd);
    bool isReadyToXT();
    bool isWorkXT();
    QList<DeviceData> getDevices();
    QStringList sayStatus();
    Region region;
    bool isXT=false;
    static bool Compare (SubAreaData &s1,SubAreaData &s2);
signals:
    void DataUpdate();
public slots:
    void DeviceUpdate();
    void Update();
private:
    Reciver *reciver;
    ReaderDevices *reader;
    QList<DeviceData> devices;
    State state;
    CommandData cmdlast;
};

#endif // SUBAREADATA_H
