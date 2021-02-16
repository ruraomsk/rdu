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


