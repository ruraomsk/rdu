#include "viewregion.h"

ViewRegion::ViewRegion()
{

}

ViewRegion::ViewRegion(Reciver *reciver,int region)
{
    this->reciver=reciver;
    this->region=region;
    regions=reciver->getListStates();
    states.clear();
    foreach (auto reg, regions) {
        if (reg.region==region)
        states.append(reciver->getState(reg));
    }
    messages=new ViewMessages(reciver);

    wtable=new QTableWidget();
    grid=new QGridLayout(this);
    setLayout(grid);
    grid->addWidget(messages,0,1);
    table();

}

void ViewRegion::Update()
{
    messages->Update();
    states.clear();
    foreach (auto reg, regions) {
        if (reg.region==region)
        states.append(reciver->getState(reg));
    }
    table();
    update();
}

void ViewRegion::table()
{
    delete wtable;
    wtable=new QTableWidget;
    wtable->setColumnCount(4);
    wtable->setHorizontalHeaderItem(0,new QTableWidgetItem("Район"));
    wtable->setHorizontalHeaderItem(1,new QTableWidgetItem("Подрайон"));
    wtable->setHorizontalHeaderItem(2,new QTableWidgetItem("Время"));
    wtable->setHorizontalHeaderItem(3,new QTableWidgetItem("Управление"));

    wtable->setMaximumSize(ini.getSize("table/size"));
    for (int row = 0; row < wtable->rowCount(); ++row) {
        wtable->removeRow(row);
    }
    int row=0;
    foreach (auto var, states) {
        wtable->insertRow(row);
            wtable->setItem(row,0,new QTableWidgetItem(QString::number(var.Area)));
            wtable->setItem(row,1,new QTableWidgetItem(QString::number(var.Area)));
            QString w;
            if (var.LastTime!=0 ) w.append(QString::asprintf("%02d:%02d",var.LastTime/60,var.LastTime%60));
            else w="Нет расчета";
            wtable->setItem(row,2,new QTableWidgetItem(w));
            w="";
            if (var.PKNow==0) w="По времени"; else w=QString::number(var.PKNow);
            wtable->setItem(row,3,new QTableWidgetItem(w));

        row++;
    }
    wtable->resizeColumnsToContents();
    grid->addWidget(wtable,0,0);

}
