#ifndef XCROSS_H
#define XCROSS_H

#include <QThread>
#include <QString>
#include <QTcpSocket>
#include <QMap>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QMutex>

class DataLine{
public:
    DataLine();
    DataLine(QMap<QString,QVariant> map);
    int Time;
    QList<int> Values;
    bool Good;
};

class Data{
public:
    Data();
    QList<DataLine> lines;
};

class Region{
public:
    Region();
    Region(int region,int area,int id);
    Region(QMap<QString,QVariant> map);
    QString crossGet();
    QString stateGet();
    QString dataGet(QString name);


    QString toKey();
    QString fullKey(QString name);
    int region;
    int area;
    int id;
};
class Value{
public:
    Value();
    Value(QMap<QString,QVariant> map);
    int Time;
    bool Def;
    QList<int> channels;
    QList<bool> goods;
};
class Xcross {
public:
    Xcross();
    Xcross(QMap<QString,QVariant> map);
    Region region;
    int Step;
    int Count;
    QList<Value> values;

};

#endif // XCROSS_H
