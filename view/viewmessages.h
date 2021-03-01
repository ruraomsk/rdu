#ifndef VIEWMESSAGES_H
#define VIEWMESSAGES_H

#include <QGridLayout>
#include <QTableWidget>
#include <QWidget>

#include "reciver/reciver.h"
#include "setup.h"

extern Setup ini;

class ViewMessages: public QWidget
{
    Q_OBJECT
public:
    ViewMessages();
    ViewMessages(Reciver *reciver);
public slots:
    void Update();

private:
    void table();
    Reciver *reciver;
    QList<QString> messages;
    QTableWidget *wtable;
    QGridLayout *grid;

};

#endif // VIEWMESSAGES_H
