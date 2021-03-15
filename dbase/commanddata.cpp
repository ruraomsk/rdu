#include "commanddata.h"

CommandData::CommandData()
{

}

bool CommandData::isHandCommand()
{
    //Истина если команда ручного управления
    if (isSetPK || isSetCK  || isSetNK ) return true;
    return false;
}

QString CommandData::sayStatus()
{
    if(!isHandCommand() && !setXT) return "**ВР** ";
    if(!isHandCommand() && setXT)  return "**XT** ";
    QString res;
    if (isSetPK) res.append(QString::asprintf("**ДУ-ПК%d** ",SetPK));
    if (isSetCK) res.append(QString::asprintf("**ДУ-CК%d** ",SetCK));
    if (isSetNK) res.append(QString::asprintf("**ДУ-HК%d** ",SetNK));
    return res;
}

QString CommandData::ToString()
{
    if (isSetPK && isSetCK && isSetNK && SetPK==0 && SetCK==0 && SetNK==0) return "ДУ автомат))";
    return "Непонятно";
}
