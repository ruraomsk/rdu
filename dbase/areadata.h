#ifndef AREADATA_H
#define AREADATA_H

#include "subareadata.h"

#include <QList>
#include <QObject>
class AreaData : public QObject
{
    Q_OBJECT
public:
    AreaData(Reciver *reciver,ReaderDevices *reader,Region region);
    int CountDevices();
    int CountConnected();
    int CountWorked();
    void SetCommand(CommandData cmd);
    bool isReadyToXT();
    bool isWorkXT();
    QStringList sayStatus();
    QList<SubAreaData *> getSubAreas();
    Region region;
    bool isXT();
signals:
    void DataUpdate();
public slots:
    void DeviceUpdate();
    void Update();
private:
    QList<SubAreaData*> subs;
    Reciver *reciver;
    ReaderDevices *reader;
    CommandData cmdlast;
};

#endif // AREADATA_H
