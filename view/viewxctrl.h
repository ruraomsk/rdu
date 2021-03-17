#ifndef VIEWXCTRL_H
#define VIEWXCTRL_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QTabWidget>
#include <QGridLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QScrollArea>

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include "../setup.h"
#include "viewareal.h"
#include "viewpoints.h"
#include "viewstrategy.h"
#include "viewresult.h"
#include "viewresultgraph.h"
#include "viewvoronoi.h"
#include "voronoi.h"
#include "bogko.h"
#include "reciver/reciver.h"
extern Setup ini;

QT_CHARTS_USE_NAMESPACE

class ViewXctrl : public QWidget
{
    Q_OBJECT
public:
    explicit ViewXctrl(Reciver *reciver,Region region,Xctrl *xctrl,QWidget *parent = nullptr);
    QString getName();
    Xctrl* getXctrl();
    QList<QVector<QString>> getMatrix();
public slots:
    void Update();
signals:

private slots:
public:

private:
    void makeSpray();
    void top();
    Reciver *reciver;
    Xctrl *xctrl;
    Region region;
    QLabel *lname;           //Описание XT
    QLabel *lLeftRel;        //Отношение для прямого направления
    QLabel *lRightRel;        //Отношение для обратного направления

    QWidget *wtop;              //Виджет редактирования параметров
    QWidget *strateg;
    QWidget *points;
    ViewAreal *areal;
    ViewVoronoi *voroni;

    Voronoi *vor;               // Для циклограмм

    QTabWidget *tab;            //основной виджет редактора
    QGridLayout *grid;
    ViewStrategy *vstrategy;
    ViewPoints *vpoints;
    ViewResult *vresult;
    ViewResultGraph *vresultgraph;
};

#endif // VIEWXCTRL_H
