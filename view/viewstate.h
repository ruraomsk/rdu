#ifndef VIEWSTATE_H
#define VIEWSTATE_H

#include <QObject>
#include <QTabWidget>
#include <QWidget>
#include "viewdevice.h"
#include "reciver/reciver.h"
#include "reciver/readerdevices.h"
#include "viewxctrl.h"
class ViewMainState : public QWidget
{
    Q_OBJECT
public:
    explicit ViewMainState(Reciver *reciver,Region region);
public slots:
    void Update();
signals:
private slots:
    void xtCalcOn();
    void xtCalcOff();
private:
    void top();
    void tablend();
    void tablext();
    void tablpri();
    Reciver *reciver;
    Region region;
    QGridLayout *maingrid;
    QTableWidget *wtabend;
    QTableWidget *wtabext;
    QTableWidget *wtabpri;
    QTextEdit *wtop;
    State state;
};
class ViewState : public QWidget
{
    Q_OBJECT
public:
    ViewState();
    ViewState(Reciver *reciver,ReaderDevices *reader,Region region);
public slots:
    void Update();
signals:

private:
    Region region;
    Reciver *reciver;
    ReaderDevices *reader;
    ViewDevice *devices;
    QTabWidget *tab;            //основной виджет редактора
    State state;
    ViewMainState *mainState;

};

#endif // VIEWSTATE_H
