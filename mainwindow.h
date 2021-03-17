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
#include "view/viewarea.h"
#include "view/viewtree.h"
#include "reciver/readerdevices.h"
#include "dbase/regiondata.h"


#include "streetBox/state.h"
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
    void busy();
    void work();
    void Deviceloaded();
    void ExitProgramm();
    void SetupEdit();
    void Restart();

private:
    void viewMake();
    QMenu *setupMenu;
    QAction *restart;
    QAction *editSetup;
    QAction *exitAct;
    QTabWidget *tab;
    QTabWidget *tabl;
    int region;
    Ui::MainWindow *ui;
    QSqlDatabase db;
    Reciver *reciver;
    ReaderDevices *reader;
    RegionData *regData;
};
#endif // MAINWINDOW_H
