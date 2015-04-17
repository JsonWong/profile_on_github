#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("");    
    setFixedSize(276, 339);

    ui->btnSendMessage->setEnabled(false);
    ui->btnSendGroupMessage->setEnabled(false);
    ui->btnJoinGroup->setEnabled(false);
    ui->btnLogout->setEnabled(false);
    ui->btnLogin->setEnabled(false);
    ui->btnRegister->setEnabled(false);
    ui->btnConnect->setDefault(true);

    m_TCPSocket = new QTcpSocket(this);
    m_TCPSocket->abort();
    connect(m_TCPSocket, SIGNAL(readyRead()), this, SLOT(readTCPMsg()));
    connect(m_TCPSocket, SIGNAL(connected()), this, SLOT(TCPSocketConnected()));
    connect(m_TCPSocket, SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(displaySocketErr(QAbstractSocket::SocketError)));

    m_UDPSocket = new QUdpSocket(this);
    m_UDPSocket->bind(6667, QUdpSocket::ShareAddress);
    connect(m_UDPSocket, SIGNAL(readyRead()), this, SLOT(readUDPMsg()));
}

MainWindow::~MainWindow()
{    
    delete ui;
    delete m_TCPSocket;
    delete m_UDPSocket;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    m_TCPSocket->write("@#$5", strlen("@#$5"));
    m_TCPSocket->waitForReadyRead();
    m_TCPSocket->disconnectFromHost();
    m_TCPSocket->close();
}

void MainWindow::recvUserInfo(QString UserName, QString Password)
{
    m_UserName = UserName;
    m_Password = Password;

    m_TCPSocket->write("@#$1", strlen("@#$1"));
    if (m_TCPSocket->waitForReadyRead() == false)
    {
        QMessageBox::warning(NULL, "", "服务器超时未响应", 0, 0);
        return;
    }

    m_TCPSocket->write(m_UserName.toStdString().c_str(), strlen(m_UserName.toStdString().c_str()));
    if (m_TCPSocket->waitForReadyRead() == false)
    {
        QMessageBox::warning(NULL, "", "服务器超时未响应", 0, 0);
        return;
    }

    m_TCPSocket->write(m_Password.toStdString().c_str(), strlen(m_Password.toStdString().c_str()));
    if (m_TCPSocket->waitForReadyRead() == false)
    {
        QMessageBox::warning(NULL, "", "服务器超时未响应", 0, 0);
        return;
    }
    if (m_TCPSocket->waitForReadyRead() == false)
    {
        QMessageBox::warning(NULL, "", "服务器超时未响应", 0, 0);
        return;
    }

    if (m_msgTCP == "O")
    {
        QMessageBox::warning(NULL, "", "登录成功", 0, 0);

        ui->btnSendMessage->setEnabled(true);
        ui->btnSendGroupMessage->setEnabled(true);
        ui->btnJoinGroup->setEnabled(true);
        ui->btnLogout->setEnabled(true);
        ui->btnLogin->setEnabled(false);
        ui->btnRegister->setEnabled(false);
        setWindowTitle(m_UserName);
    }
    else if (m_msgTCP.at(0) == 'X')
    {
        m_msgTCP.remove(0, 1);
        QMessageBox::warning(NULL, "", m_msgTCP, 0, 0);
    }
}

void MainWindow::readTCPMsg()
{
    QByteArray ba = m_TCPSocket->readAll();
    m_msgTCP = QVariant(ba).toString();
}

void MainWindow::on_btnLogin_clicked()
{
    m_dlgLogin = new dlgLogin();
    m_dlgLogin->show();
    connect(m_dlgLogin, SIGNAL(sendUserInfo(QString,QString)), this, SLOT(recvUserInfo(QString,QString)));
}

void MainWindow::on_btnConnect_clicked()
{
    m_TCPSocket->connectToHost(ui->leServerIP->text(), 6666);
    ui->btnConnect->setEnabled(false);
}

void MainWindow::TCPSocketConnected()
{
    ui->leServerIP->setEnabled(false);
    ui->btnLogin->setEnabled(true);
    ui->btnRegister->setEnabled(true);
}

void MainWindow::displaySocketErr(QAbstractSocket::SocketError err)
{
    QMessageBox::warning(NULL, "", "未连接服务器", 0, 0);
    ui->btnConnect->setEnabled(true);
}

void MainWindow::on_btnLogout_clicked()
{
    m_TCPSocket->write("@#$5", strlen("@#$5"));
    if (m_TCPSocket->waitForReadyRead() == false)
    {
        QMessageBox::warning(NULL, "", "服务器超时未响应", 0, 0);
        return;
    }
    if (m_TCPSocket->waitForReadyRead() == false)
    {
        QMessageBox::warning(NULL, "", "服务器超时未响应", 0, 0);
        return;
    }

    if (m_msgTCP == "O")
    {
        QMessageBox::warning(NULL, "", "退出成功", 0, 0);

        ui->btnSendMessage->setEnabled(false);
        ui->btnSendGroupMessage->setEnabled(false);
        ui->btnJoinGroup->setEnabled(false);
        ui->btnLogout->setEnabled(false);
        ui->btnLogin->setEnabled(true);
        ui->btnRegister->setEnabled(true);
        setWindowTitle("");
    }
    else if (m_msgTCP.at(0) == 'X')
    {
        m_msgTCP.remove(0, 1);
        QMessageBox::warning(NULL, "", m_msgTCP, 0, 0);
    }
}

void MainWindow::on_btnRegister_clicked()
{
    m_dlgReg = new DlgRegister();
    m_dlgReg->show();
    connect(m_dlgReg, SIGNAL(sendRegisterInfo(QString,QString)), this, SLOT(recvRegisterInfo(QString,QString)));
}

void MainWindow::recvRegisterInfo(QString UserName, QString Password)
{
    m_TCPSocket->write("@#$6", strlen("@#$6"));
    if (m_TCPSocket->waitForReadyRead() == false)
    {
        QMessageBox::warning(NULL, "", "服务器超时未响应", 0, 0);
        return;
    }

    m_TCPSocket->write(UserName.toStdString().c_str(), strlen(UserName.toStdString().c_str()));
    if (m_TCPSocket->waitForReadyRead() == false)
    {
        QMessageBox::warning(NULL, "", "服务器超时未响应", 0, 0);
        return;
    }

    m_TCPSocket->write(Password.toStdString().c_str(), strlen(Password.toStdString().c_str()));
    if (m_TCPSocket->waitForReadyRead() == false)
    {
        QMessageBox::warning(NULL, "", "服务器超时未响应", 0, 0);
        return;
    }
    if (m_TCPSocket->waitForReadyRead() == false)
    {
        QMessageBox::warning(NULL, "", "服务器超时未响应", 0, 0);
        return;
    }

    if (m_msgTCP == "O")
    {
        QMessageBox::warning(NULL, "", "注册成功", 0, 0);

        ui->btnSendMessage->setEnabled(false);
        ui->btnSendGroupMessage->setEnabled(false);
        ui->btnJoinGroup->setEnabled(false);
        ui->btnLogout->setEnabled(false);
        ui->btnLogin->setEnabled(true);
        ui->btnRegister->setEnabled(true);
    }
    else if (m_msgTCP.at(0) == 'X')
    {
        m_msgTCP.remove(0, 1);
        QMessageBox::warning(NULL, "", m_msgTCP, 0, 0);
    }
}

void MainWindow::readUDPMsg()
{
    while (m_UDPSocket->hasPendingDatagrams())
    {
        QByteArray ba;
        ba.resize(m_UDPSocket->pendingDatagramSize());
        m_UDPSocket->readDatagram(ba.data(), ba.size());
        m_msgUDP = ba.data();
        ui->teMsgRecv->setText(ui->teMsgRecv->toPlainText() + m_msgUDP + "\n");
    }
}

void MainWindow::on_btnJoinGroup_clicked()
{
    m_dlgJoinGroup = new DlgJoinGroup();
    m_dlgJoinGroup->show();
    connect(m_dlgJoinGroup, SIGNAL(sendGroupName(QString)), this, SLOT(RecvGroupName(QString)));
}

void MainWindow::RecvGroupName(QString GroupName)
{
    m_TCPSocket->write("@#$4", strlen("@#$4"));
    if (m_TCPSocket->waitForReadyRead() == false)
    {
        QMessageBox::warning(NULL, "", "服务器超时未响应", 0, 0);
        return;
    }

    m_TCPSocket->write(GroupName.toStdString().c_str(), strlen(GroupName.toStdString().c_str()));
    if (m_TCPSocket->waitForReadyRead() == false)
    {
        QMessageBox::warning(NULL, "", "服务器超时未响应", 0, 0);
        return;
    }
    if (m_TCPSocket->waitForReadyRead() == false)
    {
        QMessageBox::warning(NULL, "", "服务器超时未响应", 0, 0);
        return;
    }

    if (m_msgTCP == "O")
    {
        QMessageBox::warning(NULL, "", "加入群成功", 0, 0);
    }
    else if (m_msgTCP.at(0) == 'X')
    {
        m_msgTCP.remove(0, 1);
        QMessageBox::warning(NULL, "", m_msgTCP, 0, 0);
    }
}

void MainWindow::on_btnSendMessage_clicked()
{
    m_dlgSendMsg = new DlgSendMsg();
    m_dlgSendMsg->setLabelText("请输入对方账号：");
    m_dlgSendMsg->setMode(0);
    m_dlgSendMsg->show();
    connect(m_dlgSendMsg, SIGNAL(sendMsg(int,QString,QString)), this, SLOT(RecvMsgToSend(int,QString,QString)));
}

void MainWindow::on_btnSendGroupMessage_clicked()
{
    m_dlgSendMsg = new DlgSendMsg();
    m_dlgSendMsg->setLabelText("请输入群号：");
    m_dlgSendMsg->setMode(1);
    m_dlgSendMsg->show();
    connect(m_dlgSendMsg, SIGNAL(sendMsg(int,QString,QString)), this, SLOT(RecvMsgToSend(int,QString,QString)));
}

void MainWindow::RecvMsgToSend(int mode, QString Receiver, QString Msg)
{
    if (mode == 0)
    {
        m_TCPSocket->write("@#$2", strlen("@#$2"));
        if (m_TCPSocket->waitForReadyRead() == false)
        {
            QMessageBox::warning(NULL, "", "服务器超时未响应", 0, 0);
            return;
        }

        m_TCPSocket->write(Receiver.toStdString().c_str(), strlen(Receiver.toStdString().c_str()));
        if (m_TCPSocket->waitForReadyRead() == false)
        {
            QMessageBox::warning(NULL, "", "服务器超时未响应", 0, 0);
            return;
        }

        m_TCPSocket->write(Msg.toStdString().c_str(), strlen(Msg.toStdString().c_str()));
        if (m_TCPSocket->waitForReadyRead() == false)
        {
            QMessageBox::warning(NULL, "", "服务器超时未响应", 0, 0);
            return;
        }
        if (m_TCPSocket->waitForReadyRead() == false)
        {
            QMessageBox::warning(NULL, "", "服务器超时未响应", 0, 0);
            return;
        }

        if (m_msgTCP == "O")
        {
            QMessageBox::warning(NULL, "", "发送成功", 0, 0);
        }
        else if (m_msgTCP.at(0) == 'X')
        {
            m_msgTCP.remove(0, 1);
            QMessageBox::warning(NULL, "", m_msgTCP, 0, 0);
        }
    }
    else if (mode == 1)
    {
        m_TCPSocket->write("@#$3", strlen("@#$3"));
        if (m_TCPSocket->waitForReadyRead() == false)
        {
            QMessageBox::warning(NULL, "", "服务器超时未响应", 0, 0);
            return;
        }

        m_TCPSocket->write(Receiver.toStdString().c_str(), strlen(Receiver.toStdString().c_str()));
        if (m_TCPSocket->waitForReadyRead() == false)
        {
            QMessageBox::warning(NULL, "", "服务器超时未响应", 0, 0);
            return;
        }

        m_TCPSocket->write(Msg.toStdString().c_str(), strlen(Msg.toStdString().c_str()));
        if (m_TCPSocket->waitForReadyRead() == false)
        {
            QMessageBox::warning(NULL, "", "服务器超时未响应", 0, 0);
            return;
        }
        if (m_TCPSocket->waitForReadyRead() == false)
        {
            QMessageBox::warning(NULL, "", "服务器超时未响应", 0, 0);
            return;
        }

        if (m_msgTCP == "O")
        {
            QMessageBox::warning(NULL, "", "发送成功", 0, 0);
        }
        else if (m_msgTCP.at(0) == 'X')
        {
            m_msgTCP.remove(0, 1);
            QMessageBox::warning(NULL, "", m_msgTCP, 0, 0);
        }
    }
}
