#include "xcross.h"

Region::Region()
{

}

Region::Region(int region, int area, int id)
{
    this->region=region;
    this->area=area;
    this->id=id;
}

Region::Region(QMap<QString, QVariant> map)
{
    region=map["Region"].toInt();
    area=map["Area"].toInt();
    id=map["ID"].toInt();
}

QString Region::crossGet()
{
    QString result;
    return result.append(QString::asprintf("crossget,%d,%d,%d\n",region,area,id));
}

QString Region::stateGet()
{
    QString result;
    return result.append(QString::asprintf("stateget,%d,%d,%d\n",region,area,id));

}

QString Region::dataGet(QString name)
{
    QString result;
    result.append(QString::asprintf("dataget,%d,%d,%d,",region,area,id));
    result.append(name+"\n");
    return result;
}


QString Region::toKey()
{
    QString result;
    return result.append(QString::asprintf("%d:%d:%d",region,area,id));
}

QString Region::fullKey(QString name)
{
    return toKey().append(":"+name);
}

Xcross::Xcross()
{

}

Xcross::Xcross(QMap<QString, QVariant> map)
{
    Step=map["Step"].toInt();
    Count=map["Count"].toInt();
    region=Region(map["Region"].toMap());
    foreach(auto v,map["Values"].toList()){
        values.append(Value(v.toMap()));
    }

}

Value::Value()
{

}

Value::Value(QMap<QString, QVariant> map)
{
    Time=map["Time"].toInt();
    Def=map["Def"].toBool();
    foreach(auto c,map["Chanels"].toList()){
        channels.append(c.toInt());
    }
    foreach(auto g,map["Good"].toList()){
        goods.append(g.toBool());
    }

}

DataLine::DataLine()
{
    Values.clear();
    Time=0;
    Good=false;
}

DataLine::DataLine(QMap<QString, QVariant> map)
{
    Values.clear();
    Time=map["Time"].toInt();
    Good=map["Good"].toBool();
    foreach(auto v,map["Value"].toList()){
        Values.append(v.toInt());
    }
}

Data::Data()
{
    lines.clear();
}

