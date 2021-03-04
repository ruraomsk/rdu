#include "deviceinfo.h"


DeviceInfo::DeviceInfo()
{

}

DeviceInfo::DeviceInfo(QMap<QString,QVariant> map)
{
    ck=map["ck"].toInt();
    pk=map["pk"].toInt();
    nk=map["nk"].toInt();
    region=map["region"].toInt();
    area=map["area"].toInt();
    subarea=map["subarea"].toInt();
    id=map["id"].toInt();
    idevice=map["idevice"].toInt();
    status=map["status"].toInt();
    name=map["name"].toString();
}

bool DeviceInfo::Compare(DeviceInfo &d1, DeviceInfo &d2)
{
    if(d1.region!=d2.region) return d1.region<d2.region;
    if(d1.area!=d2.area) return d1.area<d2.area;
    if(d1.subarea!=d2.subarea) return d1.subarea<d2.subarea;
    return d1.id<d2.id;
}

QString DeviceInfo::getKey()
{
    QString r;
    r.append(QString::asprintf("%05d:%05d:%05d",region,area,id));
    return r;
}
