#include "viewpoints.h"

ViewPoints::ViewPoints(Xctrl *xctrl,QWidget *parent) : QWidget(parent)
{
    this->xctrl=xctrl;
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout( mainLayout );

    m_view = new QTableView;
    //    m_view->horizontalHeader().setResetResizeMode( QHeaderView::Stretch );
    m_view->setModel( ptable = new PointsTable(this->xctrl,this) );
    m_view->resizeColumnsToContents();
    mainLayout->addWidget( m_view );
    QHBoxLayout *panelLayout = new QHBoxLayout;
    mainLayout->addLayout( panelLayout );

}

void ViewPoints::removeSelected()
{
    QModelIndex index=m_view->selectionModel()->currentIndex();
    if (xctrl->Calculates.size()==1) return;
    ptable->removeSelected(index);
}

void ViewPoints::updateTable()
{
    emit updated();
}

PointsTable::PointsTable(Xctrl *xctrl, ViewPoints *parent)
{
    this->xctrl=xctrl;
    this->parent=parent;
}

int PointsTable::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return xctrl->Calculates.size();
}

int PointsTable::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 5;
}

QVariant PointsTable::data(const QModelIndex &index, int role) const
{
    if (
            !index.isValid() ||
            xctrl->Calculates.size() <= index.row() ||
            ( role != Qt::DisplayRole && role != Qt::EditRole )
            )
        return QVariant();
    Calc c=xctrl->Calculates[index.row()];
    switch( index.column() ) {
    case 0:
        return  c.Region;
    case 1:
        return c.Area;
    case 2:
        return c.ID;
    case 3:{
        QString list;
        foreach (auto val, c.ChanL) {
            list+=" "+QString::number(val);
        }
        return list;
    }
    case 4:{
        QString list;
        foreach (auto val, c.ChanR) {
            list+=" "+QString::number(val);
        }
        return list;
    }
    }

    return QVariant();
}

bool PointsTable::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(value)
    if( !index.isValid() || role != Qt::EditRole || xctrl->Calculates.size() <= index.row() ) {
        return false;
    }
        return false;

}

QVariant PointsTable::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( role != Qt::DisplayRole ) {
        return QVariant();
    }

    if( orientation == Qt::Vertical ) {
        return section;
    }

    switch( section ) {
    case 0:
        return  "Регион";
    case 1:
        return "Район";
    case 2:
        return "Перекресток";
    case 3:
        return "Номера каналов прямого";
    case 4:
        return "Номера каналов обратного";
    }

    return QVariant();
}

Qt::ItemFlags PointsTable::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags( index );
    if( index.isValid() && index.row()<4) {
        flags |= Qt::ItemIsEditable;
    }
    return flags;
}

void PointsTable::removeSelected(const QModelIndex& index)
{
    if (xctrl->Calculates.size()==1) return;
    if(index.isValid()){
        beginRemoveRows(QModelIndex(),index.row(),index.row());
        xctrl->Calculates.removeAt(index.row());
        errors.removeAt(index.row());
        endRemoveRows();
        emit updated();
    }
}

void PointsTable::addRecord()
{
    Calc c(1,1,0);
    int row=xctrl->Calculates.size();
    if (row==1) return;
    beginInsertRows(QModelIndex(),row,row);
    xctrl->Calculates.append(c);
    errors.append("");
    endInsertRows();
    emit updated();
}

