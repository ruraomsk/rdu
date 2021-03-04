#include "support.h"




void Support::ErrorMessage(QString message)
{
    QErrorMessage err;
    err.setModal(true);
    err.showMessage(message);
    err.exec();
    return;
}

void Support::Message(QString message)
{
    QMessageBox msg;
    msg.setModal(true);
    msg.setText(message);
    msg.exec();
    return;
}

QMap<int, QString> Support::loadStatuses()
{
    QMap<int, QString> statuses;
    QSqlQuery query;
    query.exec("select id,description,control from public.status");
    while(query.next()){
        statuses[query.value(0).toInt()]=query.value(1).toString()+":"+(query.value(2).toBool()?"true":"false");
    }
    query.finish();
    return statuses;
}


