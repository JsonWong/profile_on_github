#include "dlgsendmsg.h"
#include "ui_dlgsendmsg.h"

DlgSendMsg::DlgSendMsg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSendMsg)
{
    ui->setupUi(this);
    setWindowTitle("");
    setFixedSize(287, 318);
    ui->leReceiver->setValidator(new QIntValidator());
}

DlgSendMsg::~DlgSendMsg()
{
    delete ui;
}

void DlgSendMsg::setLabelText(QString s)
{
    ui->label->setText(s);
}

void DlgSendMsg::setMode(int mode)
{
    m_mode = mode;
}

void DlgSendMsg::on_btnSend_clicked()
{
    if (ui->leReceiver->text() == "")
    {
        QMessageBox::warning(NULL, "", "请输入接受号码", 0, 0);
    }
    else if (ui->teMsg->toPlainText() != "")
    {
        emit sendMsg(m_mode, ui->leReceiver->text(), ui->teMsg->toPlainText());
        delete this;
    }
}
