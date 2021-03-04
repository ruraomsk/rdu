#include "viewdevice.h"

ViewDevice::ViewDevice(ReaderDevices *reader,Region region)
{
    this->reader=reader;
    this->region=region;
    reader->addSubregion(region);
    wtable=new QTableWidget();
    grid=new QGridLayout(this);
    setLayout(grid);
    table();
}

void ViewDevice::DeviceUpdate()
{
    table();
    update();
}

void ViewDevice::table()
{
    delete wtable;
    wtable=new QTableWidget;
    wtable->setColumnCount(7);
    wtable->setHorizontalHeaderItem(0,new QTableWidgetItem("ДК"));
    wtable->setHorizontalHeaderItem(1,new QTableWidgetItem("Наименование"));
    wtable->setHorizontalHeaderItem(2,new QTableWidgetItem("Устройство"));
    wtable->setHorizontalHeaderItem(3,new QTableWidgetItem("Состояние"));
    wtable->setHorizontalHeaderItem(4,new QTableWidgetItem("ПК"));
    wtable->setHorizontalHeaderItem(5,new QTableWidgetItem("СК"));
    wtable->setHorizontalHeaderItem(6,new QTableWidgetItem("НК"));

    int row=0;
    auto devices=reader->getDevices(region);
    foreach (auto dev, devices) {
        wtable->insertRow(row);
        wtable->setItem(row,0,new QTableWidgetItem(QString::number(dev.id)));
        wtable->setItem(row,1,new QTableWidgetItem(dev.name));
        wtable->setItem(row,2,new QTableWidgetItem(QString::number(dev.idevice)));
        auto s=dev.Status.split(":");
        wtable->setItem(row,3,new QTableWidgetItem(s[0]));
        if (s[1].compare("true")==0){
            wtable->setItem(row,4,new QTableWidgetItem(QString::number(dev.pk)));
            wtable->setItem(row,5,new QTableWidgetItem(QString::number(dev.ck)));
            wtable->setItem(row,6,new QTableWidgetItem(QString::number(dev.nk)));
        }
        row++;
    }
    wtable->resizeColumnsToContents();
    grid->addWidget(wtable,0,0);

}
