#include "viewregion.h"

ViewRegion::ViewRegion()
{

}

ViewRegion::ViewRegion(Reciver *reciver,ReaderDevices *reader,int region)
{
    this->reciver=reciver;
    this->region=region;
    this->reader=reader;
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

void ViewRegion::DeviceUpdate()
{

}

void ViewRegion::table()
{
    delete wtable;
    wtable=new QTableWidget;
    wtable->setColumnCount(7);
    wtable->setHorizontalHeaderItem(0,new QTableWidgetItem("Район"));
    wtable->setHorizontalHeaderItem(1,new QTableWidgetItem("Подрайон"));
    wtable->setHorizontalHeaderItem(2,new QTableWidgetItem("Расчет"));
    wtable->setHorizontalHeaderItem(3,new QTableWidgetItem("Управление"));

    wtable->setHorizontalHeaderItem(4,new QTableWidgetItem("Время"));
    wtable->setHorizontalHeaderItem(5,new QTableWidgetItem("Расчетный ПК"));

    wtable->setHorizontalHeaderItem(6,new QTableWidgetItem("Исполняемый ПК"));

    for (int row = 0; row < wtable->rowCount(); ++row) {
        wtable->removeRow(row);
    }
    int row=0;
    foreach (auto var, states) {
        wtable->insertRow(row);
            wtable->setItem(row,0,new QTableWidgetItem(QString::number(var.Area)));
            wtable->setItem(row,1,new QTableWidgetItem(QString::number(var.SubArea)));
            QString w;
            wtable->setItem(row,2,new QTableWidgetItem(var.Switch?"Включен":"Выключен"));
            wtable->setItem(row,3,new QTableWidgetItem(var.Switch?"Включено":"Выключено"));

            if (var.LastTime!=0 ) w.append(QString::asprintf("%02d:%02d",var.LastTime/60,var.LastTime%60));
            else w="Не известно";
            wtable->setItem(row,4,new QTableWidgetItem(w));
            w="";
            if (var.PKCalc==0) w="Расчет не возможен"; else w="Расчитан ПК"+QString::number(var.PKCalc);
            wtable->setItem(row,5,new QTableWidgetItem(w));
            if (var.PKNow==0) w="Управление по суточной карте"; else w="Назначен ПК"+QString::number(var.PKNow);
            wtable->setItem(row,6,new QTableWidgetItem(w));

        row++;
    }
    wtable->resizeColumnsToContents();
    grid->addWidget(wtable,0,0);

}
