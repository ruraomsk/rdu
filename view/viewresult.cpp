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
    int limit=reciver->getEndTime(key.region);
    if (data.lines.size()==0) return;
    wtable->setColumnCount(4);
    wtable->setHorizontalHeaderItem(0,new QTableWidgetItem("Прямой"));
    wtable->setHorizontalHeaderItem(1,new QTableWidgetItem("Обратный"));
    wtable->setHorizontalHeaderItem(2,new QTableWidgetItem("КС на ДК"));
    wtable->setHorizontalHeaderItem(3,new QTableWidgetItem("Качество"));

    int row=0;
    foreach (auto d, data.lines) {
        if(d.Time>limit) break;
        wtable->insertRow(row);
        wtable->setVerticalHeaderItem(row,new QTableWidgetItem(QString::asprintf("%02d:%02d",d.Time/60,d.Time%60)));
        if(d.Values.size()==3){
            wtable->setItem(row,0,new QTableWidgetItem(QString::number(d.Values[0])));
            wtable->setItem(row,1,new QTableWidgetItem(QString::number(d.Values[1])));
            wtable->setItem(row,2,new QTableWidgetItem(QString::number(d.Values[2])));
            wtable->setItem(row,3,new QTableWidgetItem(d.Good?"":"н/д"));
            row++;
        } else {
//            qDebug()<<key.fullKey(name)<<d.Values.size();
        }
    }
    wtable->resizeColumnsToContents();
    grid->addWidget(wtable,0,0);

}
