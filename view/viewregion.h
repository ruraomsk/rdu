#ifndef VIEWREGION_H
#define VIEWREGION_H

#include <QGridLayout>
#include <QObject>
#include <QWidget>
#include <QTableWidget>
#include "viewmessages.h"
#include "reciver/reciver.h"
#include "../streetBox/xctrl/state.h"
#include "setup.h"

extern Setup ini;

class ViewRegion : public QWidget
{
    Q_OBJECT
public:
    ViewRegion();
    ViewRegion(Reciver *reciver,int region);
public slots:
    void Update();
signals:
private:
    void table();
    Reciver *reciver;
    QList<State> states;
    QList<Region> regions;
    QTableWidget *wtable;
    QGridLayout *grid;
    ViewMessages *messages;
    int region;

};

#endif // VIEWREGION_H
