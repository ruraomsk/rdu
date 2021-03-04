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
    if (reciver->errorJSON.size()!=0){
        Support::ErrorMessage("Ошибка JSON:"+reciver->errorJSON);
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
    reader=new ReaderDevices();

    QThread *threadReader=new QThread;
    reader->moveToThread(threadReader);
    connect(reader,SIGNAL(loaded()),this,SLOT(Deviceloaded()));
    reader->start();
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
    tab->addTab(new ViewRegion(reciver,reader,region=ini.getInt("region")),"Управление");
    auto lsts=reciver->getListStates();
    foreach (auto sts, lsts) {
        if(sts.region==region){
            auto st=reciver->getState(sts);
            tab->addTab(new ViewState(reciver,reader,sts),"XT:"+sts.toKey());
        }
    }

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
            static_cast<ViewState*>(tab->widget(i))->Update();
            continue;
        }
        if (tab->tabText(i).contains("Управление")){
            static_cast<ViewRegion*>(tab->widget(i))->Update();
            continue;
        }

        static_cast<ViewCross*>(tab->widget(i))->Update();
    }
}
void MainWindow::Deviceloaded()
{
    for (int i = 0; i < tab->count(); ++i) {
        if (tab->tabText(i).contains("XT:")){
            static_cast<ViewState*>(tab->widget(i))->DeviceUpdate();
            continue;
        }
        if (tab->tabText(i).contains("Управление")){
            static_cast<ViewRegion*>(tab->widget(i))->DeviceUpdate();
            continue;
        }

        static_cast<ViewCross*>(tab->widget(i))->Update();
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

