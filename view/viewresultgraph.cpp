#include "viewresultgraph.h"

ViewResultGraph::ViewResultGraph(Reciver *reciver,Region key,QString name)
{
    this->reciver=reciver;
    this->key=key;
    this->name=name;
    wgraph=new QScrollArea;
    grid=new QGridLayout;
    makeGraph();
    setLayout(grid);
}

void ViewResultGraph::Update()
{
    makeGraph();
    update();
}

void ViewResultGraph::makeGraph()
{
    delete wgraph;
    wgraph=new QScrollArea;
    auto data=reciver->getData(key,name);
    if(data.lines.size()==0) {
        return;
    }
    int limit=reciver->getEndTime(key.region);
    QVBoxLayout *vbox=new QVBoxLayout;
    QWidget *local=new QWidget;
    QChart *chart=new QChart();
    QDateTimeAxis *axisX=new QDateTimeAxis();
    axisX->setTickCount(25);
    axisX->setFormat("hh:mm");
    axisX->setTitleText("Время");
    QValueAxis *axisY=new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Значения");
    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignLeft);
    QDate date=QDate::currentDate();
    int max=0;
    for (int col = 0; col < 2; ++col) {
        QLineSeries *series=new QLineSeries;
          foreach(auto line,data.lines){
              if (line.Time>limit) continue;
              int hour=line.Time/60;
              hour=hour==24?0:hour;
              int min=line.Time%60;
              QTime time(hour,min);
              QDateTime dt(date,time);
              series->append(dt.toMSecsSinceEpoch(),line.Values[col]);
              max=qMax(max,line.Values[col]);
          }
          QTime time(0,0);
          QDateTime dt(date.addDays(1),time);
          axisX->setMax(dt);
          QDateTime dtm(date,time);
          axisX->setMin(dtm);

          series->setName(col==0?"Прямое":"Обратное");
          series->setPointLabelsVisible(true);
          series->setPointLabelsFormat("@yPoint");
          chart->addSeries(series);
          axisY->setMax(qreal(max+(0.1*max)));
          series->attachAxis(axisX);
          series->attachAxis(axisY);
    }
//    chart->legend()->hide();
    QChartView *viewchart=new QChartView(chart);
    viewchart->setRenderHint(QPainter::Antialiasing);
    viewchart->setMinimumSize(ini.getSize("chart/sizesmall"));
    vbox->addWidget(viewchart);
    local->setLayout(vbox);
    wgraph->setWidget(local);
    wgraph->setWindowTitle("График");
    wgraph->setMinimumSize(ini.getSize("chart/sizehuge"));
    grid->addWidget(wgraph,0,0);
}
