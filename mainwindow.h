#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QStyleFactory>

#include "setup.h"
#include "support.h"
#include "viewsetup.h"
#include "reciver/reciver.h"
#include "viewcross.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    virtual void closeEvent(QCloseEvent *event);

private slots:
    void loaded();
    void ExitProgramm();
    void SetupEdit();

private:
    QMenu *setupMenu;
    QAction *editSetup;
    QAction *exitAct;
    QTabWidget *tab;

    int region;
    Ui::MainWindow *ui;
        QSqlDatabase db;
        Reciver *reciver;

};
#endif // MAINWINDOW_H
