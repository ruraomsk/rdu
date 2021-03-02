#include "viewmessages.h"

ViewMessages::ViewMessages()
{

}

ViewMessages::ViewMessages(Reciver *reciver)
{
    this->reciver=reciver;
    messages=reciver->getMessages();
    wtable=new QTableWidget();
    grid=new QGridLayout(this);
    setLayout(grid);
    table();
}

void ViewMessages::Update()
{
    messages=reciver->getMessages();
    table();
    update();
}

void ViewMessages::table()
{
    delete wtable;
    wtable=new QTableWidget;
    wtable->setColumnCount(2);
    wtable->setHorizontalHeaderItem(0,new QTableWidgetItem("Время"));
    wtable->setHorizontalHeaderItem(1,new QTableWidgetItem("Сообщение"));
    for (int row = 0; row < wtable->rowCount(); ++row) {
        wtable->removeRow(row);
    }
    int row=0;
    foreach (auto var, messages) {
        wtable->insertRow(row);
        auto ls=var.split(";");
        if (ls.size()==2){
            wtable->setItem(row,0,new QTableWidgetItem(ls[0]));
            wtable->setItem(row,1,new QTableWidgetItem(ls[1]));
        }
        row++;
    }
    wtable->resizeColumnsToContents();
    grid->addWidget(wtable,0,0);

}
