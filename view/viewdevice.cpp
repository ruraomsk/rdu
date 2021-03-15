#include "viewdevice.h"


ViewDevice::ViewDevice(SubAreaData *subarea)
{
    this->subarea=subarea;
    connect(subarea,SIGNAL(DataUpdate()),this,SLOT(Update()));

    wtable=new QTableWidget();
    menuBar=new QMenuBar();
    pk=new QMenu("ПК");
    ck=new QMenu("СК");
    nk=new QMenu("НК");
    xt=new QMenu("ХТ");
//    bp=new QMenu("BP");
//    bp->addAction("Включить",this,SLOT(bpOn()));
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
//    menuBar->addMenu(bp);

    wdown=new QTextEdit();
    wdown->setFixedHeight(30);
    wdown->setFixedWidth(800);
    grid=new QGridLayout(this);
    grid->setMenuBar(menuBar);
    setLayout(grid);
    up();
    table();

}

void ViewDevice::Update()
{
    table();
    up();
    update();
}

void ViewDevice::bpOn()
{
    auto cmd=CommandData();
    cmd.isSetPK=true;
    subarea->SetCommand(cmd);
    cmd=CommandData();
    cmd.isSetCK=true;
    subarea->SetCommand(cmd);
    cmd=CommandData();
    cmd.isSetNK=true;
    subarea->SetCommand(cmd);
}

void ViewDevice::SETPK()
{
    auto cmd=CommandData();
    cmd.isSetPK=true;
    cmd.SetPK=static_cast<QAction*>(sender())->text().toInt();
    subarea->SetCommand(cmd);
}

void ViewDevice::SETCK()
{
    auto cmd=CommandData();
    cmd.isSetCK=true;
    cmd.SetCK=static_cast<QAction*>(sender())->text().toInt();
    subarea->SetCommand(cmd);
}

void ViewDevice::SETNK()
{
    auto cmd=CommandData();
    cmd.isSetNK=true;
    cmd.SetNK=static_cast<QAction*>(sender())->text().toInt();
    subarea->SetCommand(cmd);
}

void ViewDevice::xtOn()
{
    if (!subarea->isXT) return;
    if (!subarea->isReadyToXT()) return;
    auto cmd=CommandData();
    cmd.isSetXT=true;
    cmd.setXT=true;
    subarea->SetCommand(cmd);
}
void ViewDevice::xtOff()
{
    if (!subarea->isXT) return;
    if (!subarea->isWorkXT()) return;
    auto cmd=CommandData();
    cmd.isSetXT=true;
    cmd.setXT=false;
    subarea->SetCommand(cmd);
}

void ViewDevice::table()
{
    delete wtable;
    wtable=new QTableWidget;
    wtable->setColumnCount(8);
    wtable->setHorizontalHeaderItem(0,new QTableWidgetItem("ДК"));
    wtable->setHorizontalHeaderItem(1,new QTableWidgetItem("Наименование"));
    wtable->setHorizontalHeaderItem(2,new QTableWidgetItem("Устройство"));
    wtable->setHorizontalHeaderItem(3,new QTableWidgetItem("Состояние"));
    wtable->setHorizontalHeaderItem(4,new QTableWidgetItem("ПК"));
    wtable->setHorizontalHeaderItem(5,new QTableWidgetItem("СК"));
    wtable->setHorizontalHeaderItem(6,new QTableWidgetItem("НК"));
    wtable->setHorizontalHeaderItem(7,new QTableWidgetItem("Статус"));

    int row=0;
    auto devices=subarea->getDevices();
    foreach (auto dev, devices) {
        wtable->insertRow(row);
        wtable->setItem(row,0,new QTableWidgetItem(QString::number(dev.id)));
        wtable->setItem(row,1,new QTableWidgetItem(dev.name));
        wtable->setItem(row,2,new QTableWidgetItem(QString::number(dev.idevice)));
        wtable->setItem(row,3,new QTableWidgetItem(dev.getCrossStatus()));
        if (dev.isConnected()){
            wtable->setItem(row,4,new QTableWidgetItem(QString::number(dev.pk)));
            wtable->setItem(row,5,new QTableWidgetItem(QString::number(dev.ck)));
            wtable->setItem(row,6,new QTableWidgetItem(QString::number(dev.nk)));
            wtable->setItem(row,7,new QTableWidgetItem(dev.getStatus()));
        }
        row++;
    }
    wtable->resizeColumnsToContents();
    grid->addWidget(wtable,1,0);

}

void ViewDevice::up()
{
    QString r;
    foreach (auto s, subarea->sayStatus()) {
        r.append(s+" ");
    }
    wdown->setMarkdown(r);
    grid->addWidget(wdown,0,0);
}


