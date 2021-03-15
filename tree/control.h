#ifndef CONTROL_H
#define CONTROL_H

#include <QWidget>
#include "dbase/regiondata.h"
class Control : public QWidget
{
    Q_OBJECT
public:
    explicit Control(RegionData *regData,QWidget *parent = nullptr);

signals:

private:
    RegionData *regData;

};

#endif // CONTROL_H
