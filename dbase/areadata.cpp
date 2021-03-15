#include "areadata.h"

AreaData::AreaData(Reciver *reciver, ReaderDevices *reader, Region region)
{
    this->reader=reader;
    this->region=region;
    this->reciver=reciver;
    auto ss=Support::listSubareas(region.region);
    foreach (auto s, ss) {
        if(s.region==region.region && s.area==region.area) {
            subs.append(new SubAreaData(reciver,reader,s));
        }
    }
}

int AreaData::CountDevices()
{
    int count=0;
    foreach (auto sb, subs) {
        count+=sb->CountDevices();
    }
    return count;
}

int AreaData::CountConnected()
{
    int count=0;
    foreach (auto sb, subs) {
        count+=sb->CountConnected();
    }
    return count;

}

int AreaData::CountWorked()
{
    int count=0;
    foreach (auto sb, subs) {
        count+=sb->CountWorked();
    }
    return count;

}

void AreaData::SetCommand(CommandData cmd)
{
    if (cmd.isHandCommand()){
        foreach (auto sb, subs) {
            sb->SetCommand(cmd);
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
    foreach (auto sb, subs) {
        sb->SetCommand(cmd);
    }
    cmdlast=cmd;
    emit DataUpdate();
}


bool AreaData::isReadyToXT()
{
    foreach (auto sb, subs) {
        if (!sb->isReadyToXT()) return false;;
    }
    return true;
}

bool AreaData::isWorkXT()
{
    foreach (auto sb, subs) {
        if (!sb->isWorkXT()) return false;;
    }
    return true;
}

QStringList AreaData::sayStatus()
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
            result.append("*ХТ для данного района отсутствует* ");
        }
    }
    return result;

}

QList<SubAreaData *> AreaData::getSubAreas()
{
    return subs;
}

bool AreaData::isXT()
{
    bool is=true;
    foreach(auto s,subs){
        is=is&s->isXT;
    }
    return is;
}

void AreaData::DeviceUpdate()
{
    foreach (auto sb, subs) {
        sb->DeviceUpdate();
    }
    emit DataUpdate();
}

void AreaData::Update()
{
    foreach (auto sb, subs) {
        sb->Update();
    }
    emit DataUpdate();
}
