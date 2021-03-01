#ifndef VIEWSETUP_H
#define VIEWSETUP_H

#include <QObject>
#include <QDialog>
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QErrorMessage>
#include <QString>
#include <QLabel>
#include <QFileDialog>

#include "setup.h"
extern Setup ini;

class ViewSetup : public QDialog
{
    Q_OBJECT
public:
    explicit ViewSetup(QWidget *parent = nullptr);
private slots:
    void moveDataToSetup();

private:
    QLineEdit *dbname;
    QLineEdit *hostname;
    QLineEdit *password;
    QLineEdit *port;
    QLineEdit *username;

    QLineEdit *regionCode;
    QCheckBox *viewcross;

private:



};

#endif // VIEWSETUP_H
