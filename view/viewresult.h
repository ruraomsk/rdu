#ifndef VIEWRESULT_H
#define VIEWRESULT_H

#include <QGridLayout>
#include <QObject>
#include <QTableWidget>
#include <QWidget>

#include "reciver/reciver.h"

class ViewResult : public QWidget
{
    Q_OBJECT
public:
    ViewResult();
    ViewResult(Reciver *reciver,Region key,QString name);

public slots:
    void Update();
signals:


private:
    void table();
    Reciver *reciver;
    Region key;
    QString name;
    Data data;
    QTableWidget *wtable;
    QGridLayout *grid;
    bool result;

};

#endif // VIEWRESULT_H
