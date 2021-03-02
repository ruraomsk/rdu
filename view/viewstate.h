#ifndef VIEWSTATE_H
#define VIEWSTATE_H

#include <QObject>
#include <QTabWidget>
#include <QWidget>
#include "reciver/reciver.h"
#include "viewxctrl.h"
class ViewState : public QWidget
{
    Q_OBJECT
public:
    ViewState();
    ViewState(Reciver *reciver,Region region);
public slots:
    void Update();
signals:

private:
    Region region;
    Reciver *reciver;
    QTabWidget *tab;            //основной виджет редактора
    State state;
};

#endif // VIEWSTATE_H
