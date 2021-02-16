#include "viewxctrl.h"


ViewXctrl::ViewXctrl(Xctrl *xctrl, Reciver *reciver, QWidget *parent) : QWidget(parent)
{
    this->xctrl=xctrl;
    this->reciver=reciver;
    QWidget *base=new QWidget(this);
    tab=new QTabWidget;
    tab->setTabPosition(QTabWidget::West);
    vstrategy=new ViewStrategy(xctrl);
    connect(vstrategy,SIGNAL(updated()),this,SLOT(updated()));
    grid=new QGridLayout(this);
    grid->addWidget(vstrategy,1,1);
    vor=new Voronoi(qMax(xctrl->Left,xctrl->Right),qMax(xctrl->Left,xctrl->Right));
    QList<Strategy> strates;
    foreach(auto st,xctrl->Strategys){
        vor->addSet(st);
        strates.append(st);
    }
    vor->makeDiagramm();
    voroni=new ViewVoronoi(vor);
    grid->addWidget(voroni,0,2,2,2);
    top();
    grid->addWidget(wtop,0,1);
    base->setLayout(grid);
    tab->addTab(base,"Базовые");
    vpoints=new ViewPoints(project,xctrl);
    connect(vpoints,SIGNAL(updated()),this,SLOT(updated()));
    vcalc=new ViewCalculate(project,xctrl);
    connect(vcalc,SIGNAL(newSpray()),this,SLOT(addSpray()));
    vsum=new SumGraph();
    areal=new ViewAreal(project,xctrl);

//    tab->addTab(vstrategy,"Стратегии");
    tab->addTab(areal,"Области");
    tab->addTab(vpoints,"Точки");
    tab->addTab(vcalc,"Расчет");
    tab->addTab(vsum,"Графики");

    QGridLayout *maingrid=new QGridLayout(this);
    maingrid->addWidget(tab,0,1);
    setLayout(maingrid);

}

void ViewXctrl::Update()
{

}
