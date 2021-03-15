#ifndef VIEWDEVICE_H
#define VIEWDEVICE_H

#include <QGridLayout>
#include <QMenuBar>
#include <QObject>
#include <QTableWidget>
#include <QTextEdit>
#include <QWidget>
#include <QMenu>

#include "reciver/readerdevices.h"
#include "dbase/subareadata.h"

class ViewDevice : public QWidget
{
    Q_OBJECT
public:
    ViewDevice(SubAreaData *subarea);
private slots:
    void Update();
    void bpOn();
    void SETPK();
    void SETCK();
    void SETNK();
    void xtOn();
    void xtOff();
signals:
private:
    void table();
    void up();
    CommandData command;
    SubAreaData *subarea;;
    QTableWidget *wtable;
    QGridLayout *grid;
    QTextEdit *wdown;
    QMenuBar *menuBar;
    QMenu *pk;
    QMenu *ck;
    QMenu *nk;
    QMenu *xt;
    QMenu *bp;

};

#endif // VIEWDEVICE_H
