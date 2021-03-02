#ifndef VIEWRESULTGRAPH_H
#define VIEWRESULTGRAPH_H

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

class ViewResultGraph : public QWidget
{
    Q_OBJECT
public:
    ViewResultGraph(Reciver *reciver,Region key,QString name);
public slots:
    void Update();
signals:
private:
    void makeGraph();
    Reciver *reciver;
    Region key;
    QString name;

    QScrollArea *wgraph;
    QGridLayout *grid;

};
#endif // VIEWRESULTGRAPH_H
