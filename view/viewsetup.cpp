#include "viewsetup.h"

ViewSetup::ViewSetup(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Укажите параметры настройки");
    hostname=new QLineEdit(ini.getString("db/hostname"));
    port=new QLineEdit(QString::number(ini.getInt("db/port")));
    dbname=new QLineEdit(ini.getString("db/dbname"));
    username=new QLineEdit(ini.getString("db/username"));
    password=new QLineEdit(ini.getString("db/password"));

    regionCode=new QLineEdit(QString::number(ini.getInt("region")));
    QFormLayout *hbox=new QFormLayout();
    hbox->addRow(new QLabel("Раздел БД"));
    hbox->addRow("IP Сервер",hostname);
    hbox->addRow("Порт",port);
    hbox->addRow("Имя базы данных",dbname);
    hbox->addRow("Пользователь",username);
    hbox->addRow("Пароль",password);
    hbox->addRow("Код региона",regionCode);
    QPushButton *okBtn=new QPushButton("Изменить");
    connect(okBtn,SIGNAL(clicked()),this,SLOT(moveDataToSetup()));
    hbox->addWidget(okBtn);
    setLayout(hbox);
    setMinimumSize(500,300);

}

void ViewSetup::moveDataToSetup()
{
    ini.setString("db/hostname",hostname->text());
    ini.setInt("db/port",port->text().toInt());
    ini.setString("db/dbname",dbname->text());
    ini.setString("db/username",username->text());
    ini.setString("db/password",password->text());
    ini.setInt("region",regionCode->text().toInt());
    emit accept();
}

