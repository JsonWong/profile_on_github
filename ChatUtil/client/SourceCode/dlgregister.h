#ifndef DLGREGISTER_H
#define DLGREGISTER_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class DlgRegister;
}

class DlgRegister : public QDialog
{
    Q_OBJECT

public:
    explicit DlgRegister(QWidget *parent = 0);
    ~DlgRegister();

signals:
    void sendRegisterInfo(QString UserName, QString Password);

private slots:
    void on_btnRegister_clicked();

private:
    Ui::DlgRegister *ui;
};

#endif // DLGREGISTER_H
