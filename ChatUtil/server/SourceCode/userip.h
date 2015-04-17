#ifndef USERIP_H
#define USERIP_H

#include <string>

using namespace std;

class UserIP
{
public:
    UserIP();
    UserIP(string UserName, string IP);

    string getUserName();
    string getIP();

private:
    string UserName;
    string IP;
};

#endif // USERIP_H
