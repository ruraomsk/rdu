#ifndef DEVICEDATA_H
#define DEVICEDATA_H

#include <QObject>
#include <QMap>
#include <QVariant>
#include "commanddata.h"

class DeviceData
{
public:
    DeviceData();
    DeviceData(QMap<QString,QVariant> map);
    void AppendInfo(QMap<QString,QVariant> map);
    static bool Compare (DeviceData &d1,DeviceData &d2);
    QString getKey();
    QString getStatus();
    QString getCrossStatus();
    bool isConnected();
    bool isWorked(CommandData cmd);
    bool isReadyToXT();
public:
    int region ;
    int area;
    int subarea;
    int id;
    int idevice;
    int status;
    QString Status;
    int ck;
    int pk;
    int nk;
    QString name;
    int techmode;
    bool ispk=false;
    bool isck=false;
    bool isnk=false;
    int edk=false;


};

#endif // DEVICEDATA_H
