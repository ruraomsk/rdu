#include "mainwindow.h"
#include "ui_mainwindow.h"
extern Setup ini;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    reciver=new Reciver();
    if (reciver->error.size()!=0){
        Support::ErrorMessage("Нет связи с системой:"+reciver->error);
        MainWindow::close();
        return;
    }
    QThread *thread=new QThread;
    reciver->moveToThread(thread);
    connect(reciver,SIGNAL(loaded()),this,SLOT(loaded()));
    reciver->start();

    resize(ini.getSize("window/size"));
    setGeometry(ini.getPoint("window/point").x(),ini.getPoint("window/point").y(),ini.getSize("window/size").width(),ini.getSize("window/size").height());
    db=QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(ini.getString("db/hostname"));
    db.setPort(ini.getInt("db/port"));
    db.setDatabaseName(ini.getString("db/dbname"));
    db.setUserName(ini.getString("db/username"));
    db.setPassword(ini.getString("db/password"));
    bool ok=db.open();
    if (!ok){
        Support::ErrorMessage("Нет связи с БД");
        return;
    }
    editSetup=new QAction(QIcon(":/images/setup.png"),tr("Настройка"),this);
    editSetup->setStatusTip("Изменение настройки программы");
    connect(editSetup, SIGNAL(triggered()), this, SLOT(SetupEdit()));
    restart=new QAction(tr("Перезапуск XT"),this);
    restart->setStatusTip("Производится перезапуск расчета");
    connect(restart, SIGNAL(triggered()), this, SLOT(Restart()));
    exitAct=new QAction(tr("&Выход"),this);
    exitAct->setShortcut((QKeySequence::Quit));
    exitAct->setStatusTip(tr("Завершение программы"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(ExitProgramm()));

    setupMenu=menuBar()->addMenu((tr("&Управление")));
    setupMenu->addAction(restart);
    setupMenu->addAction(editSetup);
    setupMenu->addAction(exitAct);
    region=ini.getInt("region");
    tab=new QTabWidget;
    tab->addTab(new ViewRegion(reciver,region=ini.getInt("region")),"Управление");
    auto lsts=reciver->getListStates();
    foreach (auto sts, lsts) {
        if(sts.region==region){
            auto st=reciver->getState(sts);
            tab->addTab(new ViewState(reciver,sts),"XT:"+sts.toKey());
        }
    }
//    tab->addTab(new ViewMessages(reciver),"Сообщения");

    if (ini.getBool("viewcross")){
        auto lcrs=reciver->getListCrosses();
        foreach (auto cr, lcrs) {
            if(cr.region==region){
                auto c=new ViewCross(reciver,cr);
                tab->addTab(c,cr.toKey());
            }
        }
    }

    setCentralWidget(tab);
    show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    db.close();
    QMainWindow::closeEvent(event);
}

void MainWindow::loaded()
{
    for (int i = 0; i < tab->count(); ++i) {
        if (tab->tabText(i).contains("XT:")){
            ViewXctrl *ct=static_cast<ViewXctrl*>(tab->widget(i));
            ct->Update();
            continue;
        }
//        if (tab->tabText(i).contains("Сообщения")){
//            ViewMessages *ms=static_cast<ViewMessages*>(tab->widget(i));
//            ms->Update();
//            continue;
//        }
        if (tab->tabText(i).contains("Управление")){
            ViewRegion *reg=static_cast<ViewRegion*>(tab->widget(i));
            reg->Update();
            continue;
        }

        ViewCross *xc=static_cast<ViewCross*>(tab->widget(i));
        xc->Update();

    }
}

void MainWindow::ExitProgramm()
{
    emit close();
}

void MainWindow::SetupEdit()
{
    ViewSetup vs(this);
    vs.exec();

}

void MainWindow::Restart()
{
    reciver->restart();
    QThread::msleep(10000l);
    Support::Message("Необходимо перезапустить программу");
    ExitProgramm();

}

