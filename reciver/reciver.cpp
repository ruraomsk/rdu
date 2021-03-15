#include "reciver.h"


Reciver::Reciver()
{
    error="";
    isLoadCross=ini.getBool("viewcross");
    errorJSON="";
    Shift=ini.getInt("shift");
    QString ip=ini.getString("ip");

    int port=ini.getInt("port");
    if (ip.size()==0) ip="127.0.0.1";
    if (port==0) port=2050;
    if (Shift==0) Shift=40;

    queue.clear();
    socket=new QTcpSocket(this);
    socket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption,QVariant(maxSize));
    socket->connectToHost(ip,port);
    if (!socket->waitForConnected(1000)){
        error="not connected "+ip+":"+QString::number(port);
        return;
    }
    work=true;
    socket->setReadBufferSize(maxSize);
    mutex.lock();
    loadSetup();
    loadCrosses();
    loadAllTables();
    loadStates();
    loadAllStates();
    loadAllDatas();
    loadAllMessages();
    mutex.unlock();
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
            sendCmd();
        } while((QTime::currentTime().minute()%StepDev)!=0);
        int shift=Shift;
        while(shift-->0){
            sendCmd();
        }
        ReadAll();
        do{
            sendCmd();
        } while((QTime::currentTime().minute()%StepDev)==0);
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

int Reciver::getEndTime(int region)
{
    int count=-1;
    foreach(auto r,Regions){
        count++;
        if (r!=region) continue;
        QTime now=QTime::currentTime();
        int h=now.hour()+DiffTime[count];
        if (h>24) h=h%24;
        if (h<0) h+=24;
        return h*60+now.minute();
    }
    return 0;
}

void Reciver::setCmdXTOff(State *state)
{
    mutex.lock();
    reRead=true;
    queue.enqueue(QString::asprintf("stateset,%d,%d,%d,0\n",state->Region,state->Area,state->SubArea));
    mutex.unlock();
}

void Reciver::setCmdXTOn(State *state)
{
    mutex.lock();
    reRead=true;
    queue.enqueue(QString::asprintf("stateset,%d,%d,%d,1\n",state->Region,state->Area,state->SubArea));
    mutex.unlock();
}

void Reciver::setCalcXTOff(State *state)
{
    mutex.lock();
    reRead=true;
    queue.enqueue(QString::asprintf("stateset,%d,%d,%d,2\n",state->Region,state->Area,state->SubArea));
    mutex.unlock();

}

void Reciver::setCalcXTOn(State *state)
{
    mutex.lock();
    reRead=true;
    queue.enqueue(QString::asprintf("stateset,%d,%d,%d,3\n",state->Region,state->Area,state->SubArea));
    mutex.unlock();

}

void Reciver::setCmd(int idevice, int code, int pk)
{
    mutex.lock();
    queue.enqueue(QString::asprintf("devicecmd,%d,%d,%d\n",idevice,code,pk));
    mutex.unlock();
}

void Reciver::restart()
{
    mutex.lock();
    socket->write(QString("restart\n").toUtf8());
    socket->flush();
    work=false;
    mutex.unlock();
}

QString Reciver::getFromServer(QString message)
{
    socket->write(message.toUtf8());
    socket->flush();
    socket->waitForBytesWritten(timeout);
    QString json;
    int count=0;
    while (socket->waitForReadyRead(timeout)){
        json=json.append(socket->readAll());
        count++;
        if(json.endsWith("\n")) break;
    }
    qDebug()<<QTime::currentTime().toString()<<message<<count<<json.size();
    return json;

}

void Reciver::loadAllTables()
{
    if(!work) return;
    foreach (auto r, listCrosses) {
        QString json=getFromServer(r.crossGet());
        QJsonParseError jError;
        QJsonDocument jdoc=QJsonDocument::fromJson(json.toUtf8(),&jError);
        QMap<QString, QVariant>  map=jdoc.toVariant().toMap();
        if (jError.error!=QJsonParseError::NoError){
            return;
        }
        auto xc=Xcross(map);
        xcrs[xc.region.toKey()]=xc;

    }
}

void Reciver::loadAllStates()
{
    if(!work) return;
    foreach (auto r, listStates) {
        QString json=getFromServer(r.stateGet());
        QJsonParseError jError;
        QJsonDocument jdoc=QJsonDocument::fromJson(json.toUtf8(),&jError);
        QMap<QString, QVariant>  map=jdoc.toVariant().toMap();
        if (jError.error!=QJsonParseError::NoError){
            return;
        }
        auto st=State(map);
        auto reg=Region(st.Region,st.Area,st.SubArea);
        sts[reg.toKey()]=st;
    }
}

void Reciver::loadAllDatas()
{
    if(!work) return;
    foreach (auto s, sts) {
        auto reg=Region(s.Region,s.Area,s.SubArea);

        foreach (auto x,s.xctrls){
            datas[reg.fullKey(x->name)]=loadOneData(reg,x->name);
        }
        datas[reg.fullKey("result")]=loadOneData(reg,"result");
    }
}

void Reciver::loadAllMessages()
{
    if(!work) return;
    QString json=getFromServer("messages\n");
    QJsonParseError jError;
    QJsonDocument jdoc=QJsonDocument::fromJson(json.toUtf8(),&jError);
    QMap<QString, QVariant>  map=jdoc.toVariant().toMap();
    if (jError.error!=QJsonParseError::NoError){
        return;;
    }
    auto jarray=map["Messages"].toJsonArray();
    listMessages.clear();
    foreach(auto j,jarray){
        listMessages.append(j.toString());
    }
}

void Reciver::sendCmd()
{
    mutex.lock();
    while (true){
        if (queue.isEmpty()) {
            mutex.unlock();
            break;
        }
        getFromServer(queue.dequeue());
    }
    mutex.lock();
    bool r=false;
    if (reRead) {
        reRead=false;
        r=true;
    }
    mutex.unlock();
    if (r) ReadAll();
    QThread::msleep(100);
}

void Reciver::ReadAll()
{
    emit startWaite();
    mutex.lock();
    loadAllTables();
    loadAllStates();
    loadAllDatas();
    loadAllMessages();
    mutex.unlock();
    emit endWaite();
    emit loaded();

}

void Reciver::loadCrosses()
{
    if(!work) return;
    QString json=getFromServer("crosslist\n");
    QJsonParseError jError;
    QJsonDocument jdoc=QJsonDocument::fromJson(json.toUtf8(),&jError);
    if (jError.error!=QJsonParseError::NoError){
        return;
    }
    QMap<QString, QVariant>  map=jdoc.toVariant().toMap();
    listCrosses.clear();
    foreach(auto r,map["ls"].toList()){
        listCrosses.append(Region(r.toMap()));
    }
}

void Reciver::loadStates()
{
    if(!work) return;
    QString json=getFromServer("statelist\n");
    QJsonParseError jError;
    QJsonDocument jdoc=QJsonDocument::fromJson(json.toUtf8(),&jError);
    if (jError.error!=QJsonParseError::NoError){
        return;
    }
    QMap<QString, QVariant>  map=jdoc.toVariant().toMap();
    listStates.clear();
    foreach(auto r,map["ls"].toList()){
        listStates.append(Region(r.toMap()));
    }
}

void Reciver::loadSetup()
{
    if(!work) return;
    QString json=getFromServer("setup\n");
    QJsonParseError jError;
    QJsonDocument jdoc=QJsonDocument::fromJson(json.toUtf8(),&jError);
    if (jError.error!=QJsonParseError::NoError){
        return;
    }
    QMap<QString, QVariant>  map=jdoc.toVariant().toMap();
    StepDev=map["StepDev"].toInt();
    StepCalc=map["StepCalc"].toInt();
    ShiftDevice=map["ShiftDevice"].toString();          //Смещение от шага секунды
    ShiftCtrl=map["ShiftCtrl"].toString();              //Смещение для запуска управления секунды
    ClearTime=map["ClearTime"].toString();              //С этого момента все начинаем сначала
    auto array=map["Regions"].toJsonArray();
    Regions.clear();
    DiffTime.clear();
    for (int i = 0; i < array.size(); ++i) {
        Regions.append(array.at(i).toArray().at(0).toInt());
        DiffTime.append(array.at(i).toArray().at(1).toInt());
    }
}

Data Reciver::loadOneData(Region region, QString name)
{
    Data result=Data();
    if(!work) return result ;
    QString json=getFromServer(region.dataGet(name));
    QJsonParseError jError;
    QJsonDocument jdoc=QJsonDocument::fromJson(json.toUtf8(),&jError);
    QMap<QString, QVariant>  map=jdoc.toVariant().toMap();
    if (jError.error!=QJsonParseError::NoError){
        return result;
    }
    foreach(auto d,map["datas"].toList()){
        result.lines.append(DataLine(d.toMap()));
    }
    return result;
}



