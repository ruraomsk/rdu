#ifndef VIEWSTATE_H
#define VIEWSTATE_H

#include <QObject>
#include <QWidget>
#include "reciver/reciver.h"
class ViewState : public QWidget
{
    Q_OBJECT
public:
    ViewState();
    ViewState(Reciver *reciver,Region region);
public slots:
    void Update();
signals:

private:
    Region region;

};

#endif // VIEWSTATE_H
