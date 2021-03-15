#include "viewarea.h"

ViewArea::ViewArea(AreaData *area,QWidget *parent) : QWidget(parent)
{
    this->area=area;
    connect(area,&AreaData::DataUpdate,this,&ViewArea::Update);

    wtable=new QTableWidget();
    menuBar=new QMenuBar();
    pk=new QMenu("ПК");
    ck=new QMenu("СК");
    nk=new QMenu("НК");
    xt=new QMenu("ХТ");
    for (int i = 0; i < 13; ++i) {
        pk->addAction(QString::number(i),this,SLOT(SETPK()));
        ck->addAction(QString::number(i),this,SLOT(SETCK()));
        nk->addAction(QString::number(i),this,SLOT(SETNK()));
    }
    xt->addAction("Включить",this,SLOT(xtOn()));
    xt->addAction("Отключить",this,SLOT(xtOff()));

    menuBar->addMenu(pk);
    menuBar->addMenu(ck);
    menuBar->addMenu(nk);
    menuBar->addMenu(xt);

    wdown=new QTextEdit();
    wdown->setFixedHeight(30);
    wdown->setFixedWidth(800);
    grid=new QGridLayout(this);
    grid->setMenuBar(menuBar);
    setLayout(grid);
    up();
    table();
}

void ViewArea::Update()
{
    up();
    table();
    update();
}

void ViewArea::SETPK()
{
    auto cmd=CommandData();
    cmd.isSetPK=true;
    cmd.SetPK=static_cast<QAction*>(sender())->text().toInt();
    area->SetCommand(cmd);
}

void ViewArea::SETCK()
{
    auto cmd=CommandData();
    cmd.isSetCK=true;
    cmd.SetCK=static_cast<QAction*>(sender())->text().toInt();
    area->SetCommand(cmd);
}

void ViewArea::SETNK()
{
    auto cmd=CommandData();
    cmd.isSetNK=true;
    cmd.SetNK=static_cast<QAction*>(sender())->text().toInt();
    area->SetCommand(cmd);
}

void ViewArea::xtOn()
{
    if(!area->isXT()) return;
    if (!area->isReadyToXT()) return;
    auto cmd=CommandData();
    cmd.isSetXT=true;
    cmd.setXT=true;
    area->SetCommand(cmd);

}

void ViewArea::xtOff()
{
    if(!area->isXT()) return;
    auto cmd=CommandData();
    cmd.isSetXT=true;
    cmd.setXT=false;
    area->SetCommand(cmd);
}

void ViewArea::table()
{
    delete wtable;
    wtable=new QTableWidget;
    wtable->setColumnCount(4);
    wtable->setHorizontalHeaderItem(0,new QTableWidgetItem("Подрайон"));
    wtable->setHorizontalHeaderItem(1,new QTableWidgetItem("Устройства"));
    wtable->setHorizontalHeaderItem(2,new QTableWidgetItem("Текущее состояние"));
    wtable->setHorizontalHeaderItem(3,new QTableWidgetItem("Состояние ХТ"));

    int row=0;
    auto subareas=area->getSubAreas();
    int max=0;
    foreach (auto sub, subareas) {
        max=sub->region.id>max?sub->region.id:max;
    }
    for (int i = 1; i <= max; ++i) {
        foreach (auto sub, subareas) {
            if(i==sub->region.id){
                wtable->insertRow(row);
                wtable->setItem(row,0,new QTableWidgetItem(QString::number(sub->region.id)));
                wtable->setItem(row,1,new QTableWidgetItem(sub->sayStatus()[0].replace("*","")));
                wtable->setItem(row,2,new QTableWidgetItem(sub->sayStatus()[1].replace("*","")));
                wtable->setItem(row,3,new QTableWidgetItem(sub->sayStatus()[2].replace("*","")));
                row++;
                break;
            }
        }
    }
    wtable->resizeColumnsToContents();
    grid->addWidget(wtable,1,0);


}

void ViewArea::up()
{
    QString r;
    foreach (auto s, area->sayStatus()) {
        r.append(s+" ");
    }
    wdown->setMarkdown(r);
    grid->addWidget(wdown,0,0);

}
