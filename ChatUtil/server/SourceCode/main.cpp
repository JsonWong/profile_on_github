#include <iostream>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include "userip.h"
#include <vector>

#define PORT 6666

using namespace std;

int processOpt(char opt[]);
int login(string UserName, string Password);
int userRegister(string UserName, string Password);
ssize_t _read(int fd, char buffer[], ssize_t size);//from telnet

int main()
{
    int ss, sc;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int err;
    pid_t pid;

    ss = socket(AF_INET, SOCK_STREAM, 0);
    if (ss < 0)
    {
        cout<<"socket error!"<<endl;
        return -1;
    }

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    err = bind(ss, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (err < 0)
    {
        cout<<"bind error!"<<endl;
        return -1;
    }

    err = listen(ss, 20);
    if (err < 0)
    {
        cout<<"listen error!"<<endl;
        return -1;
    }

    //清空IP绑定文件
    fstream clear("UserOnline.dat", ios::trunc | ios::out);
    clear.close();

    for (;;)
    {
        socklen_t addrlen = sizeof(struct sockaddr);
        sc = accept(ss, (struct sockaddr*)&client_addr, &addrlen);
        //cout<<inet_ntoa(client_addr.sin_addr)<<" comes"<<endl;
        if (sc < 0)
        {
            continue;
        }

        pid = fork();
        if (pid == 0)
        {
            close(ss);
///////////////////////////////////////////////////////////////////////////////////////////
            ssize_t size;
            char SendBuffer[1024], RecvBuffer[1024];
            bzero(SendBuffer, 1024);
            bzero(RecvBuffer, 1024);
            bool isLogin = false, isRecvUserOnline = false, isUserInGroup = false;
            fstream in, out;
            vector<UserIP> vUserIP;
            vector<UserIP>::iterator it;
            string UserName, Password, Name, IP, RecvUserName, msg, GroupID, GroupFileName;

            //消息向接受者的6667端口发送，使用UDP数据报
            int sockRecvUser;
            struct sockaddr_in RecvUser_addr;
            sockRecvUser = socket(AF_INET, SOCK_DGRAM, 0);
            bzero(&RecvUser_addr, sizeof(RecvUser_addr));
            RecvUser_addr.sin_family = AF_INET;
            RecvUser_addr.sin_port = htons(6667);

            for (;;)
            {
                //size = read(sc, RecvBuffer, 1024);//读用户命令
                size = _read(sc, RecvBuffer, 1024);//读用户命令
                if (size == 0)
                    break;

                bzero(SendBuffer, 1024);
                sprintf(SendBuffer, "ACK");
                write(sc, SendBuffer, strlen(SendBuffer) + 1);

                int opt = processOpt(RecvBuffer);
                switch (opt)
                {
                case 1://login
                    cout<<"login"<<endl;

                    //read(sc, RecvBuffer, 1024);//接收用户账号
                    bzero(RecvBuffer, 1024);
                    _read(sc, RecvBuffer, 1024);//接收用户账号
                    UserName = RecvBuffer;

                    bzero(SendBuffer, 1024);
                    sprintf(SendBuffer, "ACK");
                    write(sc, SendBuffer, strlen(SendBuffer) + 1);

                    bzero(RecvBuffer, 1024);
                    //read(sc, RecvBuffer, 1024);//接收密码
                    _read(sc, RecvBuffer, 1024);//接收密码
                    write(sc, SendBuffer, strlen(SendBuffer) + 1);
                    Password = RecvBuffer;

                    if (login(UserName, Password) == 1)
                    {
                        bzero(SendBuffer, 1024);
                        sprintf(SendBuffer, "O");
                        write(sc, SendBuffer, strlen(SendBuffer) + 1);

                        isLogin = true;

                        //绑定用户IP
                        out.open("UserOnline.dat", ios::out | ios::app);
                        out<<UserName<<" "<<inet_ntoa(client_addr.sin_addr)<<"    " ;
                        out.close();
                    }
                    else if (login(UserName, Password) == 0)
                    {
                        bzero(SendBuffer, 1024);
                        sprintf(SendBuffer, "X密码错误");
                        write(sc, SendBuffer, strlen(SendBuffer) + 1);
                    }
                    else
                    {
                        bzero(SendBuffer, 1024);
                        sprintf(SendBuffer, "X账号未注册");
                        write(sc, SendBuffer, strlen(SendBuffer) + 1);
                    }

                    break;
                case 2://send message
                    cout<<"send message"<<endl;

                    //接收用户账号
                    bzero(RecvBuffer, 1024);
                    //read(sc, RecvBuffer, 1024);
                    _read(sc, RecvBuffer, 1024);
                    RecvUserName = RecvBuffer;

                    bzero(SendBuffer, 1024);
                    sprintf(SendBuffer, "ACK");
                    write(sc, SendBuffer, strlen(SendBuffer) + 1);

                    //接收消息
                    bzero(RecvBuffer, 1024);
                    //read(sc, RecvBuffer, 1024);
                    _read(sc, RecvBuffer, 1024);
                    write(sc, SendBuffer, strlen(SendBuffer) + 1);
                    msg = RecvBuffer;

                    isRecvUserOnline = false;
                    in.open("UserOnline.dat", ios::in);
                    while (in.eof() == false)
                    {
                        in>>Name>>IP;

                        if (Name == RecvUserName)
                        {
                            isRecvUserOnline = true;

                            inet_pton(AF_INET, IP.c_str(), &RecvUser_addr.sin_addr);

                            bzero(SendBuffer, 1024);
                            sprintf(SendBuffer, "来自%s的消息：%s", UserName.c_str(), msg.c_str());
                            sendto(sockRecvUser, SendBuffer, strlen(SendBuffer) + 1, 0,
                                   (struct sockaddr*)&RecvUser_addr, sizeof(RecvUser_addr));

                            bzero(SendBuffer, 1024);
                            sprintf(SendBuffer, "O");
                            write(sc, SendBuffer, strlen(SendBuffer) + 1);

                            break;
                        }
                    }
                    if (isRecvUserOnline == false)
                    {
                        bzero(SendBuffer, 1024);
                        sprintf(SendBuffer, "X发送失败，对方不在线");
                        write(sc, SendBuffer, strlen(SendBuffer) + 1);
                    }
                    in.close();

                    break;
                case 3://send group message
                    cout<<"send group message"<<endl;

                    //接收群号
                    bzero(RecvBuffer, 1024);
                    //read(sc, RecvBuffer, 1024);
                    _read(sc, RecvBuffer, 1024);
                    sprintf(SendBuffer, "ACK");
                    write(sc, SendBuffer, strlen(SendBuffer) + 1);
                    GroupID = RecvBuffer;
                    GroupFileName = GroupID + ".group";

                    //接收消息
                    bzero(RecvBuffer, 1024);
                    //read(sc, RecvBuffer, 1024);
                    _read(sc, RecvBuffer, 1024);
                    write(sc, SendBuffer, strlen(SendBuffer) + 1);
                    msg = RecvBuffer;

                    isUserInGroup = false;
                    in.open(GroupFileName.c_str(), ios::in);
                    if (in.is_open())
                    {
                        if (in.eof() == false)
                            in>>Name>>IP;
                        while (in.eof() == false)
                        {
                            if (Name == UserName)
                            {
                                isUserInGroup = true;
                            }
                            else
                            {
                                UserIP u(Name, IP);
                                vUserIP.push_back(u);
                            }

                            in>>Name>>IP;
                        }
                    }
                    in.close();

                    if (isUserInGroup == true)
                    {
                        for (it = vUserIP.begin(); it != vUserIP.end(); ++it)
                        {
                            isRecvUserOnline = false;
                            in.open("UserOnline.dat", ios::in);
                            if (in.eof() == false)
                                in>>Name>>IP;
                            while (in.eof() == false)
                            {
                                if (Name == it->getUserName())
                                {
                                    isRecvUserOnline = true;

                                    inet_pton(AF_INET, IP.c_str(), &RecvUser_addr.sin_addr);

                                    bzero(SendBuffer, 1024);
                                    sprintf(SendBuffer, "来自%s的消息：%s", UserName.c_str(), msg.c_str());
                                    sendto(sockRecvUser, SendBuffer, strlen(SendBuffer) + 1, 0,
                                           (struct sockaddr*)&RecvUser_addr, sizeof(RecvUser_addr));
                                    break;
                                }

                                in>>Name>>IP;
                            }
                            if (isRecvUserOnline == false)
                            {
                                RecvUser_addr.sin_addr = client_addr.sin_addr;
                                bzero(SendBuffer, 1024);
                                sprintf(SendBuffer, "%s目前不在线，无法接收群发消息", it->getUserName().c_str());
                                sendto(sockRecvUser, SendBuffer, strlen(SendBuffer) + 1, 0,
                                       (struct sockaddr*)&RecvUser_addr, sizeof(RecvUser_addr));
                            }
                            else
                            {
                                RecvUser_addr.sin_addr = client_addr.sin_addr;
                                bzero(SendBuffer, 1024);
                                sprintf(SendBuffer, "给%s的群发消息已成功发送", it->getUserName().c_str());
                                sendto(sockRecvUser, SendBuffer, strlen(SendBuffer) + 1, 0,
                                       (struct sockaddr*)&RecvUser_addr, sizeof(RecvUser_addr));
                            }
                            in.close();
                        }

                        bzero(SendBuffer, 1024);
                        sprintf(SendBuffer, "O");
                        write(sc, SendBuffer, strlen(SendBuffer) + 1);
                    }
                    else
                    {
                        bzero(SendBuffer, 1024);
                        sprintf(SendBuffer, "X你不在该群中");
                        write(sc, SendBuffer, strlen(SendBuffer) + 1);
                    }
                    vUserIP.clear();

                    break;
                case 4://join group
                    cout<<"join group"<<endl;

                    //接收群号
                    bzero(RecvBuffer, 1024);
                    //read(sc, RecvBuffer, 1024);
                    _read(sc, RecvBuffer, 1024);
                    sprintf(SendBuffer, "ACK");
                    write(sc, SendBuffer, strlen(SendBuffer) + 1);
                    GroupID = RecvBuffer;
                    GroupFileName = GroupID + ".group";

                    in.open(GroupFileName.c_str(), ios::in);
                    if (in.is_open())
                    {
                        while (in.eof() == false)
                        {
                            in>>Name>>IP;

                            if (Name == UserName)
                            {
                                isUserInGroup = true;
                            }
                        }
                    }
                    in.close();

                    if (isUserInGroup == false)
                    {
                        out.open(GroupFileName.c_str(), ios::out | ios::app);
                        if (out.is_open())
                        {
                            out<<UserName<<" "<<inet_ntoa(client_addr.sin_addr)<<"    ";

                            bzero(SendBuffer, 1024);
                            sprintf(SendBuffer, "O");
                            write(sc, SendBuffer, strlen(SendBuffer) + 1);
                        }
                        else
                        {
                            bzero(SendBuffer, 1024);
                            sprintf(SendBuffer, "X服务器错误，请重新操作");
                            write(sc, SendBuffer, strlen(SendBuffer) + 1);
                        }
                        out.close();
                    }
                    else
                    {
                        bzero(SendBuffer, 1024);
                        sprintf(SendBuffer, "X您已经在群中");
                        write(sc, SendBuffer, strlen(SendBuffer) + 1);
                    }

                    isUserInGroup = false;

                    break;
                case 5://logout
                    cout<<"logout"<<endl;

                    isLogin = false;
                    in.open("UserOnline.dat", ios::in);
                    if (in.is_open())
                    {
                        if (in.eof() == false)
                            in>>Name>>IP;
                        while (in.eof() == 0)
                        {
                            if (Name != UserName)
                            {
                                UserIP u(Name, IP);
                                vUserIP.push_back(u);
                            }

                            in>>Name>>IP;
                        }
                        in.close();

                        out.open("UserOnline.dat", ios::trunc | ios::out);
                        for (it = vUserIP.begin(); it != vUserIP.end(); ++it)
                        {
                            out<<it->getUserName()<<" "<<it->getIP()<<"    ";
                        }
                        out.close();
                        vUserIP.clear();;

                        UserName = "";
                        Password = "";

                        //告知登出成功
                        bzero(SendBuffer, 1024);
                        sprintf(SendBuffer, "O");
                        write(sc, SendBuffer, strlen(SendBuffer) + 1);
                    }
                    else
                    {
                        //告知登出失败
                        bzero(SendBuffer, 1024);
                        sprintf(SendBuffer, "X退出失败");
                        write(sc, SendBuffer, strlen(SendBuffer) + 1);
                    }

                    break;
                case 6://register
                    cout<<"register"<<endl;

                    //read(sc, RecvBuffer, 1024);//接收用户账号
                    bzero(RecvBuffer, 1024);
                    _read(sc, RecvBuffer, 1024);//接收用户账号
                    UserName = RecvBuffer;

                    bzero(SendBuffer, 1024);
                    sprintf(SendBuffer, "ACK");
                    write(sc, SendBuffer, strlen(SendBuffer) + 1);

                    bzero(RecvBuffer, 1024);
                    //read(sc, RecvBuffer, 1024);//接收密码
                    _read(sc, RecvBuffer, 1024);//接收密码
                    write(sc, SendBuffer, strlen(SendBuffer) + 1);
                    Password = RecvBuffer;

                    if (userRegister(UserName, Password) == 1)
                    {
                        bzero(SendBuffer, 1024);
                        sprintf(SendBuffer, "O");
                        write(sc, SendBuffer, strlen(SendBuffer) + 1);
                    }
                    else
                    {
                        bzero(SendBuffer, 1024);
                        sprintf(SendBuffer, "X注册失败，该账号已注册");
                        write(sc, SendBuffer, strlen(SendBuffer) + 1);
                    }

                    UserName = "";
                    Password = "";

                    break;
                case -1:
                    cout<<"invalid command"<<endl;
                default:
                    break;
                }

                bzero(SendBuffer, 1024);
                bzero(RecvBuffer, 1024);
            }
///////////////////////////////////////////////////////////////////////////////////////////
        }
        else
        {
            close(sc);
        }
    }
}

int processOpt(char opt[])
{
    if (opt[0] == 0)
        return 0;

    if (opt[0] == '@' && opt[1] == '#' && opt[2] == '$')//command format
    {
        if (opt[3] == '1')//login
            return 1;
        else if (opt[3] == '2')//send message
            return 2;
        else if (opt[3] == '3')//send group message
            return 3;
        else if (opt[3] == '4')//join group
            return 4;
        else if (opt[3] == '5')//logout
            return 5;
        else if (opt[3] == '6')//register
            return 6;
        else
            return -1;
    }
    else
    {
        return -1;
    }
}

int login(string UserName, string Password)
{
    string _username, _password;
    bool isUserExist = false;

    fstream in;
    in.open("UserInfo.dat", ios::in);
    if (in.is_open())
    {
        while (in.eof() == 0)
        {
            in>>_username>>_password;

            if (UserName == _username)
            {
                isUserExist = true;
                if (Password == _password)
                {
                    in.close();
                    return 1;
                }
                else
                {
                    in.close();
                    return 0;//密码错误
                }
            }
        }
        if (isUserExist == false)
        {
            in.close();
            return -1;//用户未注册
        }
    }
    in.close();
    return -1;//用户未注册
}

int userRegister(string UserName, string Password)
{
    string _Name, _Password;
    fstream in;
    in.open("UserInfo.dat", ios::in);
    if (in.is_open())
    {
        while (in.eof() == false)
        {
            in>>_Name>>_Password;

            if (UserName == _Name)
            {
                return 0;
            }
        }
    }
    in.close();

    fstream out;
    out.open("UserInfo.dat", ios::out | ios::app);
    if (out.is_open())
    {
        out<<UserName<<" "<<Password<<"    ";
        out.close();
        return 1;
    }
    out.close();
    return 0;
}

ssize_t _read(int fd, char buffer[], ssize_t size)
{
    int _size = read(fd, buffer, size);

    for (int i = 0; i < size; ++i)
    {
        if (buffer[i] == '\r')
        {
            buffer[i] = 0;
        }
    }

    return _size;
}
