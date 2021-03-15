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
    connect(reciver,SIGNAL(startWaite()),this,SLOT(busy()));
    connect(reciver,SIGNAL(endWaite()),this,SLOT(work()));


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

    tab=new QTabWidget;
    viewMake();
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
    this->setCursor(Qt::WaitCursor);
    regData->Update();
    QTabWidget *tabl;
    for (int i = 0; i < tab->count(); ++i) {
        if (tab->tabText(i).contains("Характерные точки")){
            tabl=static_cast<QTabWidget*>(tab->widget(i));
            for (int j = 0; j < tabl->count(); ++j) {
                static_cast<ViewState*>(tabl->widget(j))->Update();
            }
            continue;
        }
        if (tab->tabText(i).contains("Управление")){
//            static_cast<ViewRegion*>(tab->widget(i))->Update();
            continue;
        }
        //        if (tab->tabText(i).contains("Подрайоны")){
        //            tabl=static_cast<QTabWidget*>(tab->widget(i));
        //            for (int j = 0; j < tabl->count(); ++j) {
        //                static_cast<ViewArea*>(tabl->widget(j))->Update();
        //            }
        //            continue;
        //        }
        if (tab->tabText(i).contains("Статистика")){
            tabl=static_cast<QTabWidget*>(tab->widget(i));
            for (int j = 0; j < tabl->count(); ++j) {
                static_cast<ViewCross*>(tabl->widget(j))->Update();
            }
            continue;
        }
    }
    this->setCursor(Qt::ArrowCursor);
}

void MainWindow::busy()
{
    this->setCursor(Qt::WaitCursor);
}

void MainWindow::work()
{
    this->setCursor(Qt::ArrowCursor);

}
void MainWindow::Deviceloaded()
{
    //    qDebug()<<"deviceloaded start";
    this->setCursor(Qt::WaitCursor);
    regData->DeviceUpdate();
//    QTabWidget *tabl;
    for (int i = 0; i < tab->count(); ++i) {
        if (tab->tabText(i).contains("Характерные точки")){
            continue;
        }
        if (tab->tabText(i).contains("Управление")){
//            static_cast<ViewRegion*>(tab->widget(i))->DeviceUpdate();
            continue;
        }
        if (tab->tabText(i).contains("Подрайоны")){
            continue;
        }
    }
    this->setCursor(Qt::ArrowCursor);
    //    qDebug()<<"deviceloaded stop";
}

void MainWindow::ExitProgramm()
{
    emit close();
}

void MainWindow::SetupEdit()
{
    ViewSetup vs(this);
    vs.exec();
    tab->clear();
    region=ini.getInt("region");
    viewMake();
}

void MainWindow::Restart()
{
    this->setCursor(Qt::WaitCursor);
    reciver->restart();
    QThread::msleep(10000l);
    this->setCursor(Qt::ArrowCursor);
    Support::Message("Необходимо перезапустить программу");
    ExitProgramm();

}

void MainWindow::viewMake()
{
    region=ini.getInt("region");
    this->setCursor(Qt::WaitCursor);
    regData=new RegionData(reciver,reader,region);
    tab->addTab(new ViewRegion(regData),"Управление");
    tabl=new QTabWidget;
    auto lsts=reciver->getListStates();
    foreach (auto sts, lsts) {
        if(sts.region==region){
            auto st=reciver->getState(sts);
            tabl->addTab(new ViewState(reciver,reader,sts),"XT:"+sts.shortKey());
        }
    }
    tab->addTab(tabl,"Характерные точки");
    tabl=new QTabWidget;
    auto listsubarea=regData->getSubAreas();
    foreach (auto s, listsubarea) {
        tabl->addTab(new ViewDevice(s ),"Подрайон:"+s->region.shortKey());
    }
    tab->addTab(tabl,"Подрайоны");
    tabl=new QTabWidget;
    auto listarea=regData->getAreas();
    foreach (auto s, listarea) {
        tabl->addTab(new ViewArea(s ),"Район:"+QString::number(s->region.area));
    }
    tab->addTab(tabl,"Районы");
    tabl=new QTabWidget;
    if (ini.getBool("viewcross")){
        auto lcrs=reciver->getListCrosses();
        foreach (auto cr, lcrs) {
            if(cr.region==region){
                auto c=new ViewCross(reciver,cr);
                tabl->addTab(c,cr.shortKey());
            }
        }
        tab->addTab(tabl,"Статистика");
    }
    this->setCursor(Qt::ArrowCursor);
}

