#ifndef SUPPORT_H
#define SUPPORT_H

#include <QtGlobal>
#include <QApplication>
#include <QString>
#include <QErrorMessage>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMap>
#include <QList>
#include <QtSql>
#include <QMessageBox>
#include <QRandomGenerator>
#include "reciver/xcross.h"
class Support
{

public:
    static void ErrorMessage(QString message);
    static void Message(QString message);
    static QMap<int,QString> loadStatuses();
    static QList<Region> listSubareas(int region);
    static QMap<int,QString> loadNamesRegions();
    static QMap<QString,QString> loadNamesAreas();
private:
};


#endif // SUPPORT_H
