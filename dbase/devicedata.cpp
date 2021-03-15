#include "devicedata.h"


DeviceData::DeviceData()
{

}

DeviceData::DeviceData(QMap<QString,QVariant> map)
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

void DeviceData::AppendInfo(QMap<QString, QVariant> map)
{
    techmode=map["techmode"].toInt();
    auto s=map["StatusCommandDU"].toMap();
    ispk=s["IsPK"].toBool();
    isck=s["IsCK"].toBool();
    isnk=s["IsNK"].toBool();
    auto DK=map["DK"].toMap();
    edk=DK["edk"].toInt();
}

bool DeviceData::Compare(DeviceData &d1, DeviceData &d2)
{
    if(d1.region!=d2.region) return d1.region<d2.region;
    if(d1.area!=d2.area) return d1.area<d2.area;
    if(d1.subarea!=d2.subarea) return d1.subarea<d2.subarea;
    return d1.id<d2.id;
}

QString DeviceData::getKey()
{
    QString r;
    r.append(QString::asprintf("%05d:%05d:%05d",region,area,id));
    return r;
}

QString DeviceData::getStatus()
{
    QString res="";
    switch (techmode) {
    case 1: res="ВР-СК"; break;
    case 2: res="ВР-НК"; break;
    case 3: res="ДУ-СК"; break;
    case 4: res="ДУ-НК"; break;
    case 5: res="ДУ-ПК"; break;
    case 6: res="РП"; break;
    case 7: res="Коррекция с ИП"; break;
    case 8: res="Коррекция с Сервера"; break;
    case 9: res="ПК годовой карты"; break;
    case 10: res="ПК по ХТ"; break;
    case 11: res="ПК по картограмме"; break;
    case 12: res="Противозаторовое"; break;
    }
    if (ispk) res+=" Назначен ПК";
    if (isck) res+=" Назначена СК";
    if (isnk) res+=" Назначена НК";

    return res;
}

QString DeviceData::getCrossStatus()
{
    if (edk==1) return "ПЕРЕХОД";
    auto s=Status.split(":");
    return s[0];
}

bool DeviceData::isConnected()
{
    auto s=Status.split(":");
    if (s[1].compare("true")==0){
        return true;
    }
    return false;
}

bool DeviceData::isWorked(CommandData cmd)
{
    if (!isConnected()) return false;
    if(cmd.isSetPK){
        if (cmd.SetPK==0) return true;
        if (pk==cmd.SetPK) return true;
        else return false;
    }
    if(cmd.isSetCK){
        if (cmd.SetCK==0) return true;
        if (ck==cmd.SetCK) return true;
        else return false;
    }
    if(cmd.isSetNK){
        if (cmd.SetNK==0) return true;
        if (nk==cmd.SetNK) return true;
        else return false;
    }
    return true;
}

bool DeviceData::isReadyToXT()
{
    if (!isConnected()) return true;
    switch (techmode) {
    case 1: return true;//res="ВР-СК"; break;
    case 2: return true;//res="ВР-НК"; break;
    case 3: return false;//res="ДУ-СК"; break;
    case 4: return false;//res="ДУ-НК"; break;
    case 5: return false;//res="ДУ-ПК"; break;
    case 6: return true;//res="РП"; break;
    case 7: return true;//res="Коррекция с ИП"; break;
    case 8: return true;//res="Коррекция с Сервера"; break;
    case 9: return true;//res="ПК годовой карты"; break;
    case 10: return true;//res="ПК по ХТ"; break;
    case 11: return true;//res="ПК по картограмме"; break;
    case 12: return true;//res="Противозаторовое"; break;
    }
    if (ispk) return false;//res+=" Назначен ПК";
    if (isck) return false;//res+=" Назначена СК";
    if (isnk) return false;//res+=" Назначена НК";
    return true;
}
