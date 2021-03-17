#ifndef VIEWTREE_H
#define VIEWTREE_H

#include <QObject>
#include <QWidget>
#include <QObject>
#include <QVariant>
#include <QWidget>
#include <QGridLayout>
#include <QSplitter>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "dbase/regiondata.h"
#include "viewregion.h"
#include "viewarea.h"
#include "viewdevice.h"
#include "support.h"
class ViewTree : public QWidget
{
    Q_OBJECT
public:
    ViewTree(RegionData *regData);

signals:
private slots:
    void click(QTreeWidgetItem *item, int column);
private:
    void clearGrid();
    QTreeWidget *view;
    QHBoxLayout *grid;
    QGridLayout *gridview;
    RegionData *regData;
    ViewRegion *vregion;
    QMap<int,ViewArea*> areas;
    QMap<QString,ViewDevice*> subareas;

};

#endif // VIEWTREE_H
