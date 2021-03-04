#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <QObject>
#include <QMap>
#include <QVariant>

class DeviceInfo
{
public:
    DeviceInfo();
    DeviceInfo(QMap<QString,QVariant> map);
    static bool Compare (DeviceInfo &d1,DeviceInfo &d2);
    QString getKey();
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


};

#endif // DEVICEINFO_H
