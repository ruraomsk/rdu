#include "support.h"




void Support::ErrorMessage(QString message)
{
    QErrorMessage err;
    err.setModal(true);
    err.showMessage(message);
    err.exec();
    return;
}

void Support::Message(QString message)
{
    QMessageBox msg;
    msg.setModal(true);
    msg.setText(message);
    msg.exec();
    return;
}

QMap<int, QString> Support::loadStatuses()
{
    QMap<int, QString> statuses;
    QSqlQuery query;
    query.exec("select id,description,control from public.status");
    while(query.next()){
        statuses[query.value(0).toInt()]=query.value(1).toString()+":"+(query.value(2).toBool()?"true":"false");
    }
    query.finish();
    return statuses;
}

QList<Region> Support::listSubareas(int region)
{
    QList<Region> result;
    QMap<QString,Region> map;
    QSqlQuery query;
    query.exec(QString::asprintf("select area,subarea from public.cross where region=%d;",region));
    while(query.next()){
        Region key=Region(region,query.value(0).toInt(),query.value(1).toInt());
        map[key.toKey()]=key;
    }
    query.finish();
    foreach (auto s, map) {
        result.append(s);
    }
    std::sort(result.begin(),result.end(),Region::Compare);
    return result;
}

QMap<int, QString> Support::loadNamesRegions()
{
    QMap<int,QString> result;
    QSqlQuery query;
    query.exec("select region,area,nameregion,namearea from public.region ;");
    while(query.next()){
        result[query.value(0).toInt()]=query.value(2).toString();
    }
    query.finish();
    return result;

}

QMap<QString, QString> Support::loadNamesAreas()
{
    QMap<QString,QString> result;
    QSqlQuery query;
    query.exec("select region,area,nameregion,namearea from public.region ;");
    while(query.next()){
        Region key=Region(query.value(0).toInt(),query.value(1).toInt(),0);
        result[key.toKey()]=query.value(3).toString();
    }
    query.finish();
    return result;

}


