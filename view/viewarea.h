#ifndef VIEWAREA_H
#define VIEWAREA_H

#include <QWidget>
#include <QObject>
#include <QGridLayout>
#include <QMenuBar>
#include <QObject>
#include <QTableWidget>
#include <QTextEdit>
#include <QWidget>
#include <QMenu>
#include "dbase/areadata.h"
class ViewArea : public QWidget
{
    Q_OBJECT
public:
    ViewArea(AreaData *area,QWidget *parent = nullptr);

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
    AreaData *area;
};

#endif // VIEWAREA_H
