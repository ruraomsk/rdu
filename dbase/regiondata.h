#ifndef REGIONINFO_H
#define REGIONINFO_H

#include "areadata.h"
class RegionData: public QObject
{
    Q_OBJECT
public:
    RegionData(Reciver *reciver,ReaderDevices *reader,int region);
    int CountDevices();
    int CountConnected();
    int CountWorked();
    void SetCommand(CommandData cmd);
    bool isReadyToXT();
    bool isWorkXT();
    QList<SubAreaData *> getSubAreas();
    QList<AreaData *> getAreas();
    QStringList sayStatus();
    bool isXT();
    Region region;
signals:
    void DataUpdate();
public slots:
    void DeviceUpdate();
    void Update();

private:
    QList<AreaData*> areas;
    Reciver *reciver;
    ReaderDevices *reader;
    CommandData cmdlast;
};

#endif // REGIONINFO_H
