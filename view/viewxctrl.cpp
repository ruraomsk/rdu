#include "viewxctrl.h"

ViewXctrl::ViewXctrl(Reciver *reciver,Xctrl *xctrl,QWidget *parent) : QWidget(parent)
{
    this->reciver=reciver;
    this->xctrl=xctrl;
    QWidget *base=new QWidget(this);
    tab=new QTabWidget;
    tab->setTabPosition(QTabWidget::West);
    vstrategy=new ViewStrategy(xctrl);
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
    vpoints=new ViewPoints(xctrl);
    vcalc=new ViewCalculate(xctrl);
    vsum=new SumGraph();
    areal=new ViewAreal(xctrl);

    tab->addTab(areal,"Области");
    tab->addTab(vpoints,"Точки");
    tab->addTab(vcalc,"Расчет");
    tab->addTab(vsum,"Графики");

    QGridLayout *maingrid=new QGridLayout(this);
    maingrid->addWidget(tab,0,1);
    setLayout(maingrid);
}

QString ViewXctrl::getName()
{
    return "XT:"+xctrl->name;
}



Xctrl* ViewXctrl::getXctrl()
{
    return xctrl;
}

QList<QVector<QString> > ViewXctrl::getMatrix()
{
    return vcalc->getMatrix();
}

void ViewXctrl::Update()
{
    qDebug()<<"Update "<<xctrl->name;
}






void ViewXctrl::top()
{
#define maxSize 500,100
    wtop=new QWidget;
//    xctrl->Left=0;
//    xctrl->Right=0;
//    foreach (auto s, xctrl->Strategys) {
//        xctrl->Left=qMax(xctrl->Left,s.L);
//        xctrl->Right=qMax(xctrl->Right,s.R);
//    }
    lname=new QLabel(xctrl->name,this);
    lname->setMaximumSize(maxSize);
    lLeftRel=new QLabel(QString::number(xctrl->Left),this);
    lLeftRel->setMaximumSize(maxSize);
    lRightRel=new QLabel(QString::number(xctrl->Right),this);
    lRightRel->setMaximumSize(maxSize);
    QFormLayout *hbox=new QFormLayout();
    hbox->addRow("Наименование XT",lname);
    hbox->addRow("Максимум прямого",lLeftRel);
    hbox->addRow("Максимум обратного",lRightRel);
    hbox->addRow(voroni->text);
    wtop->setLayout(hbox);
}

