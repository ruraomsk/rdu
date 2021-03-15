#ifndef RECIVER_H
#define RECIVER_H

#include <QThread>
#include <QString>
#include <QTcpSocket>
#include <QMap>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QMutex>
#include <QQueue>
#include "xcross.h"
#include "../setup.h"
#include "../streetBox/xctrl/state.h"

extern Setup ini;

class Reciver: public QThread
{
    Q_OBJECT
public:
    Reciver();
    ~Reciver();
    void run() override;
    QList<Region> getListCrosses();
    QList<Region> getListStates();
    Xcross getCross(Region reg);
    State getState(Region reg);
    QList<QString> getMessages();
    Data getData(Region reg,QString name);
    int getEndTime(int region);
    QString error;
    QString errorJSON;
    void setCmdXTOff(State *state);
    void setCmdXTOn(State *state);
    void setCalcXTOff(State *state);
    void setCalcXTOn(State *state);
    void setCmd(int idevice,int code ,int pk);
    void restart();
signals:
    void loaded();
    void startWaite();
    void endWaite();

private:
    QList<Region> listCrosses;
    QList<Region> listStates;
    QList<QString> listMessages;
    QString getFromServer(QString message);
    QMap<QString,Xcross> xcrs;
    QMap<QString,State> sts;
    QMap<QString,Data> datas;
    int StepDev;                //Шаг опроса устройств в минутах
    int StepCalc;               //Шаг расчета
    QString ShiftDevice;        //Смещение от шага секунды
    QString ShiftCtrl;          //Смещение для запуска управления секунды
    QString ClearTime;          //С этого момента все начинаем сначала
    void loadCrosses();
    void loadStates();
    void loadSetup();
    Data loadOneData(Region region,QString name );
    void loadAllTables();
    void loadAllStates();
    void loadAllDatas();
    void loadAllMessages();
    void sendCmd();
    void ReadAll();
    bool needRestart;
    bool work=false;
    QTcpSocket *socket;
    QMutex mutex;
    QList<int> Regions;
    QList<int> DiffTime;
    int Shift=120;
    int timeout=2000;
    bool isLoadCross;
    qint64 maxSize=1024*1024;
    QQueue<QString> queue;
    bool reRead=false;

};

#endif // RECIVER_H
