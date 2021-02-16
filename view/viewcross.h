#ifndef VIEWCROSS_H
#define VIEWCROSS_H

#include <QObject>
#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QGridLayout>
#include <QScrollArea>
#include <QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCore/QDateTime>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QDateTime>

#include "reciver/reciver.h"
#include "reciver/xcross.h"
#include "setup.h"

QT_CHARTS_USE_NAMESPACE

extern Setup ini;

class ViewCross : public QWidget
{
    Q_OBJECT
public:
//    explicit
    ViewCross(Reciver *reciver,Region region);

public slots:
    void Update();
    void itemCkliked();
signals:

private:
    QList<bool> checked;
    void table();
    void graph();
    Reciver *reciver;
    Region region;
    Xcross xcross;
    QTableWidget *wtable;
    QGridLayout *grid;
    QScrollArea *wgraph;
};

#endif // VIEWCROSS_H
