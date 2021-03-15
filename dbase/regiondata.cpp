#include "regiondata.h"


RegionData::RegionData(Reciver *reciver, ReaderDevices *reader, int region)
{
    this->reader=reader;
    this->region=Region(region,0,0);
    this->reciver=reciver;
    auto ss=Support::listSubareas(this->region.region);
    QMap<int,Region> sss;
    foreach (auto s, ss) {
        if(s.region==this->region.region ) {
            sss[s.area]=s;
        }
    }
    foreach(auto s,sss){
        areas.append(new AreaData(reciver,reader,s));
    }

}

void RegionData::DeviceUpdate()
{
    foreach (auto ar, areas) {
        ar->DeviceUpdate();
    }
    emit DataUpdate();
}

void RegionData::Update()
{
    foreach (auto ar, areas) {
        ar->Update();
    }
    emit DataUpdate();
}

int RegionData::CountDevices()
{
    int count=0;
    foreach (auto ar, areas) {
        count+=ar->CountDevices();
    }
    return count;
}

int RegionData::CountConnected()
{
    int count=0;
    foreach (auto ar, areas) {
        count+=ar->CountConnected();
    }
    return count;
}

int RegionData::CountWorked()
{
    int count=0;
    foreach (auto ar, areas) {
        count+=ar->CountWorked();
    }
    return count;
}

void RegionData::SetCommand(CommandData cmd)
{
    if (cmd.isHandCommand()){
        foreach (auto ar, areas) {
            ar->SetCommand(cmd);
        }
        if (cmdlast.isSetPK && cmdlast.SetPK!=0 && cmd.isSetPK && cmd.SetPK==0) {
            cmdlast.isSetPK=false;
            cmdlast.SetPK=0;
        } else {
            if (cmd.isSetPK){
                cmdlast.isSetPK=cmd.isSetPK;
                cmdlast.SetPK=cmd.SetPK;
            }
        }
        if (cmdlast.isSetCK && cmdlast.SetCK!=0 && cmd.isSetCK && cmd.SetCK==0) {
            cmdlast.isSetCK=false;
            cmdlast.SetCK=0;
        } else {
            if (cmd.isSetCK){
                cmdlast.isSetCK=cmd.isSetCK;
                cmdlast.SetCK=cmd.SetCK;
            }
        }
        if (cmdlast.isSetNK && cmdlast.SetNK!=0 && cmd.isSetNK && cmd.SetNK==0) {
            cmdlast.isSetNK=false;
            cmdlast.SetNK=0;
        } else {
            if (cmd.isSetNK){
                cmdlast.isSetNK=cmd.isSetNK;
                cmdlast.SetNK=cmd.SetNK;
            }
        }
        if (cmdlast.isSetPK && cmdlast.SetPK==0){
            cmdlast.isSetPK=false;
        }
        if (cmdlast.isSetCK && cmdlast.SetCK==0){
            cmdlast.isSetCK=false;
        }
        if (cmdlast.isSetNK && cmdlast.SetNK==0){
            cmdlast.isSetNK=false;
        }

        emit DataUpdate();
        return;
    }
    foreach (auto ar, areas) {
        ar->SetCommand(cmd);
    }
    cmdlast=cmd;
    emit DataUpdate();
}

bool RegionData::isReadyToXT()
{
    foreach (auto ar, areas) {
        if (!ar->isReadyToXT()) return false;;
    }
    return true;
}

bool RegionData::isWorkXT()
{
    foreach (auto ar, areas) {
        if (!ar->isWorkXT()) return false;;
    }
    return true;

}

QList<SubAreaData *> RegionData::getSubAreas()
{
    QList<SubAreaData *> result;
    foreach (auto ar, areas) {
        foreach(auto a,ar->getSubAreas()){
            result.append(a);
        }

    }
    return result;
}

QList<AreaData *> RegionData::getAreas()
{
    return areas;
}

QStringList RegionData::sayStatus()
{
    QStringList result;
    double p1=0.0,p2=0.0;
    if (CountDevices()!=0) p1=((double)CountConnected()/(double)CountDevices());
    if (CountDevices()!=0) p2=((double)CountWorked()/(double)CountDevices());
    result.append(QString::asprintf("Всего ДК %d на связи %.2f%s подчинены %.2f%s ",
                                    CountDevices(),p1*100.0,"%",p2*100.0,"%"));
    result.append("**Назначен** "+cmdlast.sayStatus());
    if (isWorkXT()) {
        result.append("**Выполняется ХТ** ");
    } else{
        if(isXT()){
            if (isReadyToXT()) result.append("*Готов к запуску XT* ");
            else result.append("***ДК не готовы к запуску XT*** ");
        }else {
            result.append("*ХТ для данного региона отсутствует* ");
        }
    }
    return result;


}

bool RegionData::isXT()
{
    bool is=true;
    foreach(auto a,areas){
        is=is&a->isXT();
    }
    return is;
}
