#include "viewcross.h"


ViewCross::ViewCross(Reciver *reciver, Region region)
{
    this->reciver=reciver;
    this->region=region;
    xcross=reciver->getCross(region);
    for (int var = 0; var < xcross.Count; ++var) {
        checked.append(false);
    }
    wtable=new QTableWidget();
    grid=new QGridLayout(this);
    setLayout(grid);
    table();
    wgraph=new QScrollArea;
    graph();
    this->show();
}

void ViewCross::Update()
{
    xcross=reciver->getCross(region);
    table();
    graph();
    update();
}

void ViewCross::table()
{
    delete wtable;
    wtable=new QTableWidget;
    wtable->setColumnCount(xcross.Count+2);
    QTableWidgetItem *t=new QTableWidgetItem("Время");
    t->setCheckState(Qt::Unchecked);
    wtable->setHorizontalHeaderItem(0,t);
    wtable->setSelectionBehavior(QAbstractItemView::SelectColumns);
    wtable->setSelectionMode(QAbstractItemView::MultiSelection);
    //    qDebug()<<checked;
    for (int var = 1; var < xcross.Count+1; ++var) {
        t=new QTableWidgetItem(QString::number(var));
        t->setCheckState(Qt::Checked);
        wtable->setHorizontalHeaderItem(var,t);
    }
    t=new QTableWidgetItem("Качество");
    t->setCheckState(Qt::Unchecked);
    wtable->setHorizontalHeaderItem(xcross.Count+1,t);

    wtable->setMaximumSize(ini.getSize("table/size"));
    for (int row = 0; row < wtable->rowCount(); ++row) {
        wtable->removeRow(row);
    }
    int row=0;
    foreach (auto var, xcross.values) {
        wtable->insertRow(row);
        QString time=QString::asprintf("%02d:%02d",var.Time/60,var.Time%60);
        wtable->setItem(row,0,new QTableWidgetItem(time));
        for (int j = 1; j < xcross.Count+1 ; ++j) {
            wtable->setItem(row,j,new QTableWidgetItem(QString::number(var.channels[j-1])));
        }
        for (int i = 0; i < checked.size(); ++i) {
            if (checked[i]){
                wtable->selectColumn(i+1);
            }
        }
        if (var.Def){
            QString r=" ";
            for (int i = 0; i < var.goods.size(); ++i) {
                if (!var.goods[i]) r+=QString::number(i+1)+" ";
            }
            wtable->setItem(row,xcross.Count+1,new QTableWidgetItem(r));

        } else {
            wtable->setItem(row,xcross.Count+1,new QTableWidgetItem("н/д"));

        }

        row++;
    }
    wtable->resizeColumnsToContents();
    connect(wtable,SIGNAL(itemSelectionChanged()),this,SLOT(itemCkliked()));
    grid->addWidget(wtable,0,0);
}

void ViewCross::graph()
{
    delete wgraph;
    QWidget *local=new QWidget;
    wgraph=new QScrollArea;
    QVBoxLayout *vbox=new QVBoxLayout;
    QChart *chart=new QChart();
    QDateTimeAxis *axisX=new QDateTimeAxis;
    axisX->setTickCount(25);
    axisX->setFormat("hh:mm");
    axisX->setTitleText("Время");
    QValueAxis *axisY=new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Значения");
    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignLeft);
    int max=0;
    for (int column = 0; column < checked.size(); ++column) {
        if (!checked[column]) continue;
        QLineSeries *series=new QLineSeries;
        foreach(auto m,xcross.values){
            int hour=m.Time/60;
            hour=hour==24?0:hour;
            int min=m.Time%60;
            QTime time(hour,min);
            QDateTime dt(QDate::currentDate(),time);
            series->append(dt.toMSecsSinceEpoch(),m.channels[column]);

            max=max<m.channels[column]?m.channels[column]:max;
        }
        QTime time(0,0);
        QDateTime dt(QDate::currentDate().addDays(1),time);
        axisX->setMax(dt);
        QDateTime dtm(QDate::currentDate(),time);
        axisX->setMin(dtm);

        series->setName(QString::number(column));
        series->setPointLabelsVisible(true);
        series->setPointLabelsFormat("@yPoint");
        chart->addSeries(series);
        axisY->setMax(qreal(max+(0.1*max)));
        series->attachAxis(axisX);
        series->attachAxis(axisY);
    }
    chart->legend()->hide();
    //        chart->setTitle(combo->itemText(index)+":"+cross->archs[date].Comment);
    //        chart->createDefaultAxes();
    QChartView *viewchart=new QChartView(chart);
    viewchart->setRenderHint(QPainter::Antialiasing);
    viewchart->setMinimumSize(ini.getSize("chart/sizesmall"));
    vbox->addWidget(viewchart);
    local->setLayout(vbox);
    wgraph->setWidget(local);
    wgraph->setWindowTitle("График");
    wgraph->setMinimumSize(ini.getSize("chart/sizehuge"));
    grid->addWidget(wgraph,0,1);
}

void ViewCross::itemCkliked()
{
    int len=checked.size();
    checked.clear();
    for (int i = 0; i < len; ++i) {
        checked.append(false);
    }
    foreach (auto it, wtable->selectedItems()) {
        if (it->column()>=1&&it->column()<=xcross.Count) checked[it->column()-1]=true;
    }
    //    qDebug()<<checked;
    graph();
    update();
}

