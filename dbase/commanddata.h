#ifndef COMMANDDATA_H
#define COMMANDDATA_H

#include <QString>


class CommandData
{
public:
    CommandData();
    bool isHandCommand();
    QString sayStatus();
    QString ToString();
    bool isSetPK=false;
    int  SetPK=0;
    bool isSetNK=false;
    int  SetNK=0;
    bool isSetCK=false;
    int  SetCK=0;
    bool isSetXT=false;
    bool setXT=false;

private:

};

#endif // COMMANDDATA_H
