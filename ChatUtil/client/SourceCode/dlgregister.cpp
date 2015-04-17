#include "dlgregister.h"
#include "ui_dlgregister.h"

DlgRegister::DlgRegister(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgRegister)
{
    ui->setupUi(this);
    setWindowTitle("");
    setFixedSize(159, 234);
    ui->leUserName->setValidator(new QIntValidator());
    ui->lePassword1->setEchoMode(QLineEdit::Password);
    ui->lePassword2->setEchoMode(QLineEdit::Password);
}

DlgRegister::~DlgRegister()
{
    delete ui;
}

void DlgRegister::on_btnRegister_clicked()
{
    if (ui->leUserName->text() != "" && ui->lePassword1->text() != "" && ui->lePassword2->text() != "")
    {
        if (ui->lePassword1->text() != ui->lePassword2->text())
        {
            QMessageBox::warning(NULL, "", "密码输入不一致", 0, 0);
        }
        else
        {
            emit sendRegisterInfo(ui->leUserName->text(), ui->lePassword1->text());
            delete this;
        }
    }
    else
    {
        QMessageBox::warning(NULL, "", "输入无效", 0, 0);
    }
}
