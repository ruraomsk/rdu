#include "readerdevices.h"

ReaderDevices::ReaderDevices()
{
    statuses=Support::loadStatuses();
}

void ReaderDevices::addSubregion(Region region)
{
    mutex.lock();
    needLoaded.append(region);
    mutex.unlock();
}

QList<DeviceInfo> ReaderDevices::getDevices(Region region)
{
    mutex.lock();
    auto result=devices[region.toKey()];
    mutex.unlock();
    std::sort(result.begin(),result.end(),DeviceInfo::Compare);
    return result;
}

void ReaderDevices::run()
{
    int step=ini.getInt("stepDevice");
    if (step==0){
        step=2;
        ini.setInt("stepDevice",step);
    }
    db=QSqlDatabase::addDatabase("QPSQL","readerdevices");
    db.setHostName(ini.getString("db/hostname"));
    db.setPort(ini.getInt("db/port"));
    db.setDatabaseName(ini.getString("db/dbname"));
    db.setUserName(ini.getString("db/username"));
    db.setPassword(ini.getString("db/password"));
    bool ok=db.open();
    if (!ok){
        Support::ErrorMessage("Нет связи с БД");
        return;
    }

    while (true){
        QThread::msleep(step*1000l);
        mutex.lock();
        foreach (auto region, needLoaded) {
            QList<DeviceInfo> list;
            QString w;
            QSqlQuery query(db);
            w.append(QString::asprintf("select state from public.cross where region=%d and area=%d and subarea=%d;",region.region
                                       ,region.area,region.id));
            query.exec(w);
            while(query.next()){
                QString json=query.value(0).toString();
                QJsonParseError jError;
                QJsonDocument jdoc=QJsonDocument::fromJson(json.toUtf8(),&jError);
                QMap<QString, QVariant>  map=jdoc.toVariant().toMap();
                if (jError.error!=QJsonParseError::NoError){
                    qDebug()<<jError.errorString();
                    return;
                }
                auto dev=new DeviceInfo(map);
                dev->Status=statuses[dev->status];
                list.append(*dev);
            }
            query.finish();
            devices[region.toKey()]=list;
        }
        mutex.unlock();
        emit loaded();
    }
}
