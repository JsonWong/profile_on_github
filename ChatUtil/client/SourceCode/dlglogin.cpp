#include "dlglogin.h"
#include "ui_dlglogin.h"

dlgLogin::dlgLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgLogin)
{
    ui->setupUi(this);
    setWindowTitle("");
    setFixedSize(251, 175);
    ui->leUserName->setValidator(new QIntValidator());
    ui->lePassword->setEchoMode(QLineEdit::Password);
}

dlgLogin::~dlgLogin()
{    
    delete ui;
}

void dlgLogin::on_btnLogin_clicked()
{
    if (ui->leUserName->text() != "" && ui->lePassword->text() != "")
    {
        emit sendUserInfo(ui->leUserName->text(), ui->lePassword->text());
        delete this;
    }
    else
    {
        QMessageBox::warning(NULL, "", "输入无效", 0, 0);
    }
}
