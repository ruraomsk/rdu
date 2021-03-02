#include "reciver.h"

extern Setup ini;

Reciver::Reciver()
{
    error="";
    QString ip=ini.getString("ip");
    int port=ini.getInt("port");
    if (ip.size()==0) ip="127.0.0.1";
    if (port==0) port=2050;

    socket=new QTcpSocket(this);
    socket->connectToHost(ip,port);
    if (!socket->waitForConnected(3000)){
        error="not connected "+ip+":"+QString::number(port);
        return;
    }
    work=true;
    socket->write(QString("setup\n").toUtf8());
    socket->flush();
    if (socket->waitForReadyRead(500)){
        QString json;
        json=json.append(socket->readAll()).trimmed();
        loadSetup(json);
    } else {
        error="not responce "+ip+":"+QString::number(port);
        return;
    }

    QString send=QString("crosslist\n");
    socket->write(send.toUtf8());
    socket->flush();
    QString json;
    while (socket->waitForReadyRead(100)){
        json=json.append(socket->readAll()).trimmed();
    }
    loadCrosses(json);
    loadAllTables();

    send=QString("statelist\n");
    socket->write(send.toUtf8());
    socket->flush();
    json.clear();
    while (socket->waitForReadyRead(100)){
        json=json.append(socket->readAll()).trimmed();
    }

    loadStates(json);
    loadAllStates();
    loadAllDatas();
    loadAllMessages();
}

Reciver::~Reciver()
{
    socket->close();
}

void Reciver::run()
{
    if (error.size()!=0) return;

    while(true){
        do{
            int minute=QTime::currentTime().minute();
            if(minute%StepDev==0) break;
            QThread::msleep(1000l);
        } while(true);
        QThread::msleep(60000l);
        loadAllTables();
        loadAllStates();
        loadAllDatas();
        loadAllMessages();
        emit loaded();

    }
}

QList<Region> Reciver::getListCrosses()
{
    mutex.lock();
    QList<Region> result;
    foreach (auto r, listCrosses) {
        result.append(r);
    }
    mutex.unlock();
    return result;
}

QList<Region> Reciver::getListStates()
{
    mutex.lock();
    QList<Region> result;
    foreach (auto r, listStates) {
        result.append(r);
    }
    mutex.unlock();
    return result;

}

Xcross Reciver::getCross(Region reg)
{
    mutex.lock();
    Xcross result=xcrs[reg.toKey()];
    mutex.unlock();
    return result;
}

State Reciver::getState(Region reg)
{
    mutex.lock();
    auto result=sts[reg.toKey()];
    mutex.unlock();
    return result;

}

QList<QString> Reciver::getMessages()
{
    QList<QString> result;
    mutex.lock();
    result=listMessages;
    mutex.unlock();
    return result;
}

Data Reciver::getData(Region reg, QString name)
{
    Data result;
    mutex.lock();
    result=datas[reg.fullKey(name)];
    mutex.unlock();
    return result;
}

void Reciver::restart()
{
    mutex.lock();
        socket->write(QString("restart\n").toUtf8());
        socket->flush();
        work=false;
    mutex.unlock();
}

void Reciver::loadAllTables()
{
    if(!work) return;
    int region=ini.getInt("region");
    mutex.lock();
    foreach (auto r, listCrosses) {
        if (r.region!=region) continue;;
        socket->write(r.crossGet().toUtf8());
        socket->flush();
        QString json;
        while (socket->waitForReadyRead(100)){
            json=json.append(socket->readAll()).trimmed();
        }
        QJsonParseError jError;
        QJsonDocument jdoc=QJsonDocument::fromJson(json.toUtf8(),&jError);
        QMap<QString, QVariant>  map=jdoc.toVariant().toMap();
        if (jError.error!=QJsonParseError::NoError){
            error=jError.errorString();
            return;
        }
        auto xc=Xcross(map);
        xcrs[xc.region.toKey()]=xc;

    }
    mutex.unlock();
//    qDebug()<<"read cancel";

}

void Reciver::loadAllStates()
{
    if(!work) return;
    int region=ini.getInt("region");
    mutex.lock();
    foreach (auto r, listStates) {
        if (r.region!=region) continue;;
        socket->write(r.stateGet().toUtf8());
        socket->flush();
        QString json;
        while (socket->waitForReadyRead(100)){
            json=json.append(socket->readAll()).trimmed();
        }
        QJsonParseError jError;
        QJsonDocument jdoc=QJsonDocument::fromJson(json.toUtf8(),&jError);
        QMap<QString, QVariant>  map=jdoc.toVariant().toMap();
        if (jError.error!=QJsonParseError::NoError){
            error=jError.errorString();
            return;
        }
        auto st=State(map);
        auto reg=Region(st.Region,st.Area,st.SubArea);
        sts[reg.toKey()]=st;

    }
    mutex.unlock();
}

void Reciver::loadAllDatas()
{
    if(!work) return;
    mutex.lock();
    foreach (auto s, sts) {
        auto reg=Region(s.Region,s.Area,s.SubArea);

        foreach (auto x,s.xctrls){
            datas[reg.fullKey(x->name)]=loadOneData(reg,x->name);
        }
        datas[reg.fullKey("result")]=loadOneData(reg,"result");
    }
    mutex.unlock();

}

void Reciver::loadAllMessages()
{
    if(!work) return;
    mutex.lock();
        socket->write(QString("messages\n").toUtf8());
        socket->flush();
        QString json;
        while (socket->waitForReadyRead(100)){
            json=json.append(socket->readAll()).trimmed();
        }
        if (json.size()==0) {
            mutex.unlock();
            return;
        }
        QJsonParseError jError;
        QJsonDocument jdoc=QJsonDocument::fromJson(json.toUtf8(),&jError);
        QMap<QString, QVariant>  map=jdoc.toVariant().toMap();
        if (jError.error!=QJsonParseError::NoError){
            error=jError.errorString();
            return;
        }
        auto jarray=map["Messages"].toJsonArray();
        listMessages.clear();
        foreach(auto j,jarray){
            listMessages.append(j.toString());
        }
//        auto xc=Xcross(map);
//        xcrs[xc.region.toKey()]=xc;

    mutex.unlock();

}

void Reciver::loadCrosses(QString json)
{
    if(!work) return;
    QJsonParseError jError;
    QJsonDocument jdoc=QJsonDocument::fromJson(json.toUtf8(),&jError);
    if (jError.error!=QJsonParseError::NoError){
        error=jError.errorString();
        return;
    }
    QMap<QString, QVariant>  map=jdoc.toVariant().toMap();
    listCrosses.clear();
    foreach(auto r,map["ls"].toList()){
        listCrosses.append(Region(r.toMap()));
    }
}

void Reciver::loadStates(QString json)
{
    if(!work) return;
    QJsonParseError jError;
    QJsonDocument jdoc=QJsonDocument::fromJson(json.toUtf8(),&jError);
    if (jError.error!=QJsonParseError::NoError){
        error=jError.errorString();
        return;
    }
    QMap<QString, QVariant>  map=jdoc.toVariant().toMap();
    listStates.clear();
    foreach(auto r,map["ls"].toList()){
        listStates.append(Region(r.toMap()));
    }
}

void Reciver::loadSetup(QString json)
{
    if(!work) return;
    QJsonParseError jError;
    QJsonDocument jdoc=QJsonDocument::fromJson(json.toUtf8(),&jError);
    if (jError.error!=QJsonParseError::NoError){
        error=jError.errorString();
        return;
    }
    QMap<QString, QVariant>  map=jdoc.toVariant().toMap();
    StepDev=map["StepDev"].toInt();
    StepCalc=map["StepCalc"].toInt();
    ShiftDevice=map["ShiftDevice"].toString();        //Смещение от шага секунды
    ShiftCtrl=map["ShiftCtrl"].toString();;          //Смещение для запуска управления секунды
    ClearTime=map["ClearTime"].toString();;          //С этого момента все начинаем сначала
}

Data Reciver::loadOneData(Region region, QString name)
{
    Data result=Data();
    if(!work) return result ;
    socket->write(region.dataGet(name).toUtf8());
    socket->flush();
    QString json;
    while (socket->waitForReadyRead(100)){
        json=json.append(socket->readAll()).trimmed();
    }
    QJsonParseError jError;
    QJsonDocument jdoc=QJsonDocument::fromJson(json.toUtf8(),&jError);
    QMap<QString, QVariant>  map=jdoc.toVariant().toMap();
    if (jError.error!=QJsonParseError::NoError){
        error=jError.errorString();
        return result;
    }
    foreach(auto d,map["datas"].toList()){
        result.lines.append(DataLine(d.toMap()));
    }
    return result;
}



