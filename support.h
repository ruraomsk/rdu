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
class Support
{

public:
    static void ErrorMessage(QString message);
    static void Message(QString message);
private:
};


#endif // SUPPORT_H
