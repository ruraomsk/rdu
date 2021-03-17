#include "viewtree.h"


ViewTree::ViewTree(RegionData *regData)
{
    this->regData=regData;
    grid=new QHBoxLayout;
    gridview=new QGridLayout;
    view=new QTreeWidget;


    QSplitter *split = new QSplitter;
    split->addWidget(view);
    QWidget *CENT = new QWidget;
    CENT->setLayout(gridview);
    split->addWidget(CENT);
    split->setStretchFactor(1,0);
    grid->addWidget(split);


    view->headerItem()->setText(0,"Выбор");
    view->setColumnCount(2);
    auto region=new QTreeWidgetItem;
    region->setText(0,"Регион");
    region->setText(1,Support::loadNamesRegions()[regData->region.region]);

    vregion=new ViewRegion(regData);
    gridview->addWidget(vregion,0,0);
    auto listNameAreas=Support::loadNamesAreas();
    foreach(auto ar,regData->getAreas()){
        auto area=new QTreeWidgetItem;
        area->setText(0,"Район:"+QString::number(ar->region.area));
        ar->region.id=0;
        area->setText(1,listNameAreas[ar->region.toKey()]);
        areas[ar->region.area]=new ViewArea(ar);
        foreach(auto sub,ar->getSubAreas()){
            auto subarea=new QTreeWidgetItem;
            subareas[sub->region.toKey()]=new ViewDevice(sub);
            subarea->setText(0,"Подрайон:"+QString::number(sub->region.area)+":"+QString::number(sub->region.id));
            area->addChild(subarea);
        }
        region->addChild(area);
    }
    view->insertTopLevelItem(0,region);
    view->expandAll();
    view->resizeColumnToContents(0);
    view->resizeColumnToContents(1);
//    view->collapseAll();
    connect(view,&QTreeWidget::itemClicked,this,&ViewTree::click);
            setLayout(grid);
}

void ViewTree::click(QTreeWidgetItem *item, int column)
{
    if(item->text(column).contains("Регион")){
        clearGrid();
        vregion->setVisible(true);
        gridview->addWidget(vregion,0,0);
        update();
        return;
    }
    if(item->text(column).contains("Район:")){
        clearGrid();
        auto varea=areas[item->text(column).split(":").at(1).toInt()];
        varea->setVisible(true);
        gridview->addWidget(varea,0,0);
        update();
        return;
    }
    if(item->text(column).contains("Подрайон:")){
        clearGrid();
        auto ls=item->text(column).split(":");
        auto vsubarea=subareas[QString::number(regData->region.region)+":"+ls.at(1)+":"+ls.at(2)];
        vsubarea->setVisible(true);
        gridview->addWidget(vsubarea,0,0);
        update();
        return;
    }

}

void ViewTree::clearGrid()
{
        gridview->takeAt(0)->widget()->hide();

}
