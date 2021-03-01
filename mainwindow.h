#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QStyleFactory>

#include "setup.h"
#include "support.h"
#include "view/viewsetup.h"
#include "view/viewxctrl.h"
#include "reciver/reciver.h"
#include "view/viewcross.h"
#include "view/viewmessages.h"
#include "view/viewstate.h"
#include "view/viewregion.h"


#include "../streetBox/xctrl/state.h"
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
    void Restart();

private:
    QMenu *setupMenu;
    QAction *restart;
    QAction *editSetup;
    QAction *exitAct;
    QTabWidget *tab;

    int region;
    Ui::MainWindow *ui;
        QSqlDatabase db;
        Reciver *reciver;

};
#endif // MAINWINDOW_H
