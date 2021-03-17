#ifndef VIEWREGION_H
#define VIEWREGION_H

#include <QGridLayout>
#include <QObject>
#include <QWidget>
#include <QTableWidget>
#include <QTextEdit>
#include <QMenuBar>
#include "viewmessages.h"
#include "reciver/reciver.h"
#include "reciver/readerdevices.h"
#include "streetBox/state.h"
#include "dbase/regiondata.h"
#include "setup.h"

extern Setup ini;

class ViewRegion : public QWidget
{
    Q_OBJECT
public:
    ViewRegion();
    ViewRegion(RegionData *regData);
private slots:
    void Update();
    void SETPK();
    void SETCK();
    void SETNK();
    void xtOn();
    void xtOff();
private:
    void table();
    void up();
    CommandData command;
    QTableWidget *wtable;
    QGridLayout *grid;
    QTextEdit *wdown;
    QMenuBar *menuBar;
    QMenu *pk;
    QMenu *ck;
    QMenu *nk;
    QMenu *xt;
    RegionData *regData;

};

#endif // VIEWREGION_H
