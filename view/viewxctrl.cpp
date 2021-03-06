#include "viewxctrl.h"

ViewXctrl::ViewXctrl(Reciver *reciver,Region region,Xctrl *xctrl,QWidget *parent) : QWidget(parent)
{
    this->reciver=reciver;
    this->xctrl=xctrl;
    this->region=region;
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
    vpoints=new ViewPoints(xctrl);
    voroni=new ViewVoronoi(vor);

    grid->addWidget(voroni,0,2,2,2);
    top();
    grid->addWidget(wtop,0,1);
    grid->addWidget(vpoints,2,1);
    base->setLayout(grid);
    tab->addTab(base,"Базовые");
    areal=new ViewAreal(xctrl);
    vresult=new ViewResult(reciver,region,xctrl->name);
    vresultgraph=new ViewResultGraph(reciver,region,xctrl->name);
    tab->addTab(areal,"Области");
    auto calc=new QWidget();
    auto local=new QGridLayout();
    local->addWidget(vresultgraph,0,0);
    local->addWidget(vresult,1,0);
    calc->setLayout(local);
    tab->addTab(calc,"Расчет");

    QGridLayout *maingrid=new QGridLayout(this);
    maingrid->addWidget(tab,0,1);
    setLayout(maingrid);
    makeSpray();

}

QString ViewXctrl::getName()
{
    return "XT:"+xctrl->name;
}



Xctrl* ViewXctrl::getXctrl()
{
    return xctrl;
}


void ViewXctrl::Update()
{
    makeSpray();

    vresult->Update();
    vresultgraph->Update();
    update();
}

void ViewXctrl::makeSpray()
{
    auto data=reciver->getData(region,xctrl->name);
    int limit=reciver->getEndTime(region.region);
    if (data.lines.size()==0) return;
    vor->clearSpray();
    areal->vor->clearSpray();
    foreach (auto d, data.lines) {
        if(d.Time>limit) break;
        auto point=Point(d.Values[1],d.Values[0]);
        auto name=QString::asprintf("%02d:%02d",d.Time/60,d.Time%60);
        if (!d.Good) continue;
        vor->addSpray(point,name);
        areal->vor->addSpray(point,name);
    }
    areal->redraw();
}






void ViewXctrl::top()
{
#define maxSize 500,100
    wtop=new QWidget;
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

