#ifndef VIEWXCTRL_H
#define VIEWXCTRL_H

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

//#include "../project/project.h"
//#include "../setup.h"
//#include "viewareal.h"
//#include "viewpoints.h"
//#include "viewstrategy.h"
//#include "viewcalculate.h"
//#include "viewvoronoi.h"
//#include "voronoi.h"
//#include "bogko.h"
//#include "sumgraph.h"
#include "reciver/reciver.h"
#include "../streetBox/xctrl/xctrl.h"

#include "setup.h"

extern Setup ini;

QT_CHARTS_USE_NAMESPACE

class ViewXctrl : public QWidget
{
    Q_OBJECT
public:
    explicit ViewXctrl(Xctrl *xctrl,Reciver *reciver,QWidget *parent = nullptr);

public slots:
    void Update();

signals:

private:
    Xctrl *xctrl;
    Reciver *reciver;
    QLineEdit *lname;           //Описание XT
    QLineEdit *lLeftRel;        //Отношение для прямого направления
    QLineEdit *lRightRel;        //Отношение для обратного направления

    QWidget *wtop;              //Виджет редактирования параметров
//    QWidget *wgraph;
    QWidget *strateg;
    QWidget *points;
    ViewAreal *areal;
    ViewVoronoi *voroni;

    Voronoi *vor;               // Для циклограмм

    QTabWidget *tab;            //основной виджет редактора
    QGridLayout *grid;
    ViewStrategy *vstrategy;
    ViewPoints *vpoints;
    ViewCalculate *vcalc;
    SumGraph *vsum;

};

#endif // VIEWXCTRL_H
