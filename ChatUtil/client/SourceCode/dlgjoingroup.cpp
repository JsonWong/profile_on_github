#include "dlgjoingroup.h"
#include "ui_dlgjoingroup.h"

DlgJoinGroup::DlgJoinGroup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgJoinGroup)
{
    ui->setupUi(this);
    setWindowTitle("");
    setFixedSize(156, 109);
    ui->leGroupName->setValidator(new QIntValidator());
}

DlgJoinGroup::~DlgJoinGroup()
{
    delete ui;
}

void DlgJoinGroup::on_btnOK_clicked()
{
    emit sendGroupName(ui->leGroupName->text());
    delete this;
}
