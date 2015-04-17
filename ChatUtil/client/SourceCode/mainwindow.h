#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <dlglogin.h>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QMessageBox>
#include <QCloseEvent>
#include <dlgregister.h>
#include <dlgjoingroup.h>
#include <dlgsendmsg.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:

    void recvUserInfo(QString UserName, QString Password);

    void on_btnLogin_clicked();

    void readTCPMsg();

    void on_btnConnect_clicked();

    void displaySocketErr(QAbstractSocket::SocketError);

    void TCPSocketConnected();

    void on_btnLogout_clicked();

    void on_btnRegister_clicked();

    void recvRegisterInfo(QString UserName, QString Password);

    void readUDPMsg();

    void on_btnJoinGroup_clicked();

    void RecvGroupName(QString GroupName);

    void RecvMsgToSend(int mode, QString Receiver, QString Msg);

    void on_btnSendMessage_clicked();

    void on_btnSendGroupMessage_clicked();

private:
    Ui::MainWindow *ui;
    dlgLogin *m_dlgLogin;
    QTcpSocket *m_TCPSocket;
    QUdpSocket *m_UDPSocket;
    DlgRegister *m_dlgReg;
    DlgJoinGroup *m_dlgJoinGroup;
    DlgSendMsg *m_dlgSendMsg;

    QString m_UserName, m_Password;
    QString m_msgTCP, m_msgUDP;
};

#endif // MAINWINDOW_H
