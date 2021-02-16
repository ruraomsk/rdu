#ifndef RECIVER_H
#define RECIVER_H

#include <QThread>
#include <QString>
#include <QTcpSocket>
#include <QMap>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QMutex>
#include "xcross.h"
#include "../setup.h"
#include "../streetBox/xctrl/state.h"

class Reciver: public QThread
{
    Q_OBJECT
public:
    Reciver();
    void run() override;
    QList<Region> getListCrosses();
    Xcross getCross(Region reg);
    QString error;
signals:
    void loaded();
private:
    QList<Region> listCrosses;
    QList<Region> listStates;
    QMap<QString,Xcross> xcrs;
    QMap<QString,State> sts;
    QMap<QString,Data> datas;
    int StepDev;                //Шаг опроса устройств в минутах
    int StepCalc;               //Шаг расчета
    QString ShiftDevice;        //Смещение от шага секунды
    QString ShiftCtrl;          //Смещение для запуска управления секунды
    QString ClearTime;          //С этого момента все начинаем сначала
    void loadCrosses(QString json);
    void loadStates(QString json);
    void loadSetup(QString json);
    Data loadOneData(Region region,QString name );
    void loadAllTables();
    void loadAllStates();
    void loadAllDatas();

    QTcpSocket *socket;
    QMutex mutex;
};

#endif // RECIVER_H
