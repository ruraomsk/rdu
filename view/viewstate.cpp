#include "viewstate.h"

ViewState::ViewState()
{

}

ViewState::ViewState(Reciver *reciver,ReaderDevices *reader,Region region)
{
    this->reciver=reciver;
    this->region=region;
    this->reader=reader;
    state=reciver->getState(region);
    tab=new QTabWidget;
    mainState=new ViewMainState(reciver,region);
    tab->addTab(mainState,"Основной:");
    foreach (auto x, state.xctrls) {
        tab->addTab(new ViewXctrl(reciver,region,x),"XT:"+region.shortKey()+":"+x->name);
    }
    QGridLayout *maingrid=new QGridLayout(this);
    maingrid->addWidget(tab,0,1);
    setLayout(maingrid);

}

void ViewState::Update()
{
    mainState->Update();
    for (int i = 0; i < tab->count(); ++i) {
        if (tab->tabText(i).contains("XT:")){
            static_cast<ViewXctrl*>(tab->widget(i))->Update();
            continue;
        }
    }
    update();
}



ViewMainState::ViewMainState(Reciver *reciver,Region region)
{
    this->reciver=reciver;
    this->region=region;
    state=reciver->getState(region);
    wtop=new QTextEdit;
    wtop->setFixedHeight(30);
    wtop->setFixedWidth(800);
    QMenuBar *menuBar=new QMenuBar();
    QMenu *xt=new QMenu("Расчет ХТ");
    xt->addAction("Включить",this,SLOT(xtCalcOn()));
    xt->addAction("Отключить",this,SLOT(xtCalcOff()));
    menuBar->addMenu(xt);

    wtabend=new QTableWidget();
    wtabext=new QTableWidget();
    wtabpri=new QTableWidget();
    maingrid=new QGridLayout(this);
    top();
    tablend();
    tablext();
    tablpri();
    maingrid->setMenuBar(menuBar);
    setLayout(maingrid);
}

void ViewMainState::Update()
{
    state=reciver->getState(region);
    top();
    tablend();
    tablext();
    tablpri();
    update();
}

void ViewMainState::xtCalcOn()
{
    if (state.Release) return;
    reciver->setCalcXTOn(&state);
    state.Switch=true;
    top();
    update();
}

void ViewMainState::xtCalcOff()
{
    if (state.Release) return;
    reciver->setCalcXTOff(&state);
    state.Switch=false;
    top();
    update();
}

void ViewMainState::top()
{
    QString w="**Расчет XT** ";
    w.append(state.Switch?"Включен ":"Выключен ");
    w.append("**Управление** ");
    w.append(state.Release?"Включено ":"Выключено ");

    if (state.LastTime!=0 ) w.append(QString::asprintf("**Время %02d:%02d** ",state.LastTime/60,state.LastTime%60));
    else w="**Время Не известно** ";
    if (state.PKCalc==0) w.append("Расчет не возможен "); else w.append("Расчитан ПК"+QString::number(state.PKCalc)+" ");
    if (state.PKNow==0) w.append("**Управление по ВР** "); else w.append("**Назначен ПК"+QString::number(state.PKNow)+"**");
        wtop->setMarkdown(w);
        maingrid->addWidget(wtop,0,0);

}

void ViewMainState::tablend()
{
    delete wtabend;
    wtabend=new QTableWidget;
    auto data=reciver->getData(region,"result");
    int limit=reciver->getEndTime(region.region);
    qDebug()<<limit/60<<limit%60;
    if (data.lines.size()==0) return;
    wtabend->setColumnCount(3);
    wtabend->setHorizontalHeaderItem(0,new QTableWidgetItem("КС на ДК"));
    wtabend->setHorizontalHeaderItem(1,new QTableWidgetItem("ПК"));
    wtabend->setHorizontalHeaderItem(2,new QTableWidgetItem("Качество"));

    int row=0;
    foreach (auto d, data.lines) {
        if(d.Time>limit) break;
        wtabend->insertRow(row);
        wtabend->setVerticalHeaderItem(row,new QTableWidgetItem(QString::asprintf("%02d:%02d",d.Time/60,d.Time%60)));
        if(d.Values.size()==2){
            wtabend->setItem(row,0,new QTableWidgetItem(QString::number(d.Values[0])));
            wtabend->setItem(row,1,new QTableWidgetItem(QString::number(state.getPK(d.Values[0]))));
            wtabend->setItem(row,2,new QTableWidgetItem(d.Good?"":"н/д"));
            row++;
        } else {
            qDebug()<<region.toKey()<<d.Values.size();
        }
    }
    wtabend->resizeColumnsToContents();
    maingrid->addWidget(wtabend,1,0);
}

void ViewMainState::tablext()
{
    delete wtabext;
    wtabext=new QTableWidget;
    wtabext->setColumnCount(2);
    wtabext->setHorizontalHeaderItem(0,new QTableWidgetItem("КС на ДК"));
    wtabext->setHorizontalHeaderItem(1,new QTableWidgetItem("ПК"));

    int row=0;
    for (int i = 0; i < 12; ++i) {
        wtabext->insertRow(row);
            wtabext->setItem(row,0,new QTableWidgetItem(QString::number(state.external[i][0])));
            wtabext->setItem(row,1,new QTableWidgetItem(QString::number(state.external[i][1])));
            row++;
    }
    wtabext->resizeColumnsToContents();
    maingrid->addWidget(wtabext,1,1);
}

void ViewMainState::tablpri()
{
    delete wtabpri;
    wtabpri=new QTableWidget;
    wtabpri->setColumnCount(3);
    wtabpri->setHorizontalHeaderItem(0,new QTableWidgetItem("0"));
    wtabpri->setHorizontalHeaderItem(1,new QTableWidgetItem("1"));
    wtabpri->setHorizontalHeaderItem(2,new QTableWidgetItem("2"));

    int row=0;
    for (int i = 0; i < 4; ++i) {
        wtabpri->insertRow(row);
            wtabpri->setItem(row,0,new QTableWidgetItem(QString::number(state.prioryty.prior[i][0])));
            wtabpri->setItem(row,1,new QTableWidgetItem(QString::number(state.prioryty.prior[i][1])));
            wtabpri->setItem(row,2,new QTableWidgetItem(QString::number(state.prioryty.prior[i][2])));
            row++;
    }
    wtabpri->resizeColumnsToContents();
    maingrid->addWidget(wtabpri,1,2);
}
