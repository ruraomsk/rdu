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
    Region(QString key);
    QString crossGet();
    QString stateGet();
    QString dataGet(QString name);
    static bool Compare(Region &r1,Region &r2);
    static bool Eq(Region &r1,Region &r2);


    QString toKey();
    QString fullKey(QString name);
    QString shortKey();
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
    int DiffTime;
    QList<Value> values;

};

#endif // XCROSS_H
