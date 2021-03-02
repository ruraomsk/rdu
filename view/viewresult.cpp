#include "viewresult.h"

ViewResult::ViewResult()
{

}

ViewResult::ViewResult(Reciver *reciver, Region key, QString name)
{
    this->reciver=reciver;
    this->key=key;
    this->name=name;
    wtable=new QTableWidget();
    grid=new QGridLayout(this);
    setLayout(grid);
    table();
}

void ViewResult::Update()
{
    table();
    update();
}

void ViewResult::table()
{
    delete wtable;
    wtable=new QTableWidget;
    data=reciver->getData(key,name);
    if (data.lines.size()==0) return;
        wtable->setColumnCount(5);
        wtable->setHorizontalHeaderItem(0,new QTableWidgetItem("Время"));
        wtable->setHorizontalHeaderItem(1,new QTableWidgetItem("Прямой"));
        wtable->setHorizontalHeaderItem(2,new QTableWidgetItem("Обратный"));
        wtable->setHorizontalHeaderItem(3,new QTableWidgetItem("КС на ДК"));
        wtable->setHorizontalHeaderItem(4,new QTableWidgetItem("Качество"));

    int row=0;
    foreach (auto d, data.lines) {
        wtable->insertRow(row);
        wtable->setItem(row,0,new QTableWidgetItem(QString::asprintf("%02d:%02d",d.Time/60,d.Time%60)));
        wtable->setItem(row,1,new QTableWidgetItem(QString::number(d.Values[0])));
        wtable->setItem(row,2,new QTableWidgetItem(QString::number(d.Values[1])));
        wtable->setItem(row,3,new QTableWidgetItem(QString::number(d.Values[2])));
        wtable->setItem(row,4,new QTableWidgetItem(d.Good?"":"н/д"));
        row++;
    }
    wtable->resizeColumnsToContents();
    grid->addWidget(wtable,0,0);

}
