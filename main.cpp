#include "mainwindow.h"

#include <QApplication>
#include "reciver/reciver.h"
#include "setup.h"
#include <QThread>
Setup ini ("rdu.ini");

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setStyle(QStyleFactory::create("Fusion"));
    w.setWindowTitle(ini.getString("name"));
    w.show();
    return a.exec();
}
