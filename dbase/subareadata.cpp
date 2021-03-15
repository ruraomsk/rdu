#include "subareadata.h"


SubAreaData::SubAreaData()
{

}

SubAreaData::SubAreaData(Reciver *reciver,ReaderDevices *reader,Region region)
{
    this->reader=reader;
    this->region=region;
    this->reciver=reciver;
    reader->addSubregion(region);
    auto sts=reciver->getListStates();
    isXT=false;
    foreach (auto s, sts) {
        if (Region::Eq(s,region)){
            isXT=true;
            state=reciver->getState(region);
            break;
        }
    }
    devices=reader->getDevices(region);
    if(isWorkXT()){
        cmdlast.isSetXT=true;
        cmdlast.setXT=true;
    }

}

int SubAreaData::CountDevices()
{
    return devices.size();
}

int SubAreaData::CountConnected()
{
    int count=0;
    foreach (auto d, devices) {
        if(d.isConnected()) count++;
    }
    return count;
}

int SubAreaData::CountWorked()
{
    int count=0;
    if(isXT && cmdlast.isSetXT && cmdlast.setXT){
        foreach(auto d,devices){
            if(d.isConnected()){
                if(state.PKNow==0) count++;
                else {
                    if(d.pk==state.PKNow) count++;
                }
            }
        }
        return count;
    }
    foreach(auto d,devices){
        if(d.isWorked(cmdlast)) count++;
    }
    return count;
}

void SubAreaData::SetCommand(CommandData cmd)
{
    if (cmd.isHandCommand() && isXT && state.Release){
        //Нужно выключить ХТ если оно включено
        reciver->setCmdXTOff(&state);
        state.Release=false;
        state.PKNow=0;
        cmdlast.setXT=false;
        cmdlast.isSetXT=false;
    }
    if (cmd.isHandCommand()){
        foreach(auto d,devices){
            if(d.isConnected()) {
                if (cmd.isSetPK)reciver->setCmd(d.idevice,5,cmd.SetPK);
                if (cmd.isSetCK)reciver->setCmd(d.idevice,6,cmd.SetCK);
                if (cmd.isSetNK)reciver->setCmd(d.idevice,7,cmd.SetNK);

            }
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
    if(cmd.isSetXT){
        if (isXT){
            if(cmd.setXT)   {
                reciver->setCmdXTOn(&state);
                cmdlast=cmd;
                state.Release=true;
                state.PKNow=0;
            }
            else {
                reciver->setCmdXTOff(&state);
                state.Release=false;
                state.PKNow=0;
                foreach(auto d,devices){
                    if(d.isConnected()) {
                        reciver->setCmd(d.idevice,5,0);
                    }
                }
                cmdlast=CommandData();
            }
        }
        emit DataUpdate();
        return;
    }
    emit DataUpdate();
}

bool SubAreaData::isReadyToXT()
{
    foreach (auto d, devices) {
        if(!d.isReadyToXT()) return false;;
    }
    return true;;
}

bool SubAreaData::isWorkXT()
{
    return state.Release;
}

QList<DeviceData> SubAreaData::getDevices()
{
    return devices;
}

QStringList SubAreaData::sayStatus()
{
    QStringList result;
    result.append(QString::asprintf("Всего ДК %d на связи %d подчинены %d ",
                                    CountDevices(),CountConnected(),CountWorked()));
    result.append("**Назначен** "+cmdlast.sayStatus());
    if (isWorkXT()) {
        result.append(QString::asprintf("**Выполняется ХТ время %02d:%02d ПК%d** ",
                                        state.LastTime/60,state.LastTime%60,state.PKNow));
    } else{
        if(isXT){
            if (isReadyToXT()) result.append("*Готов к запуску XT* ");
            else result.append("***ДК не готовы к запуску XT*** ");
        }else {
            result.append("*ХТ для данного подрайона отсутствует* ");
        }
    }
    return result;
}

bool SubAreaData::Compare(SubAreaData &s1, SubAreaData &s2)
{
    if(s1.region.region!=s2.region.region) return s1.region.region<s2.region.region;
    if(s1.region.area!=s2.region.area) return s1.region.area<s2.region.area;
    return s1.region.id<s2.region.id;
}

void SubAreaData::DeviceUpdate()
{
    devices=reader->getDevices(region);
    emit DataUpdate();
}

void SubAreaData::Update()
{
    if(!isXT) return;
    state=reciver->getState(region);
    emit DataUpdate();
}
