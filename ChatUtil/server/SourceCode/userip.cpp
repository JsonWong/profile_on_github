#include "userip.h"

UserIP::UserIP()
{
    this->UserName = "";
    this->IP = "";
}

UserIP::UserIP(string UserName, string IP)
{
    this->UserName = UserName;
    this->IP = IP;
}

string UserIP::getUserName()
{
    return this->UserName;
}

string UserIP::getIP()
{
    return this->IP;
}
