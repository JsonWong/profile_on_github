#ifndef DLGLOGIN_H
#define DLGLOGIN_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class dlgLogin;
}

class dlgLogin : public QDialog
{
    Q_OBJECT

public:
    explicit dlgLogin(QWidget *parent = 0);
    ~dlgLogin();

signals:
    void sendUserInfo(QString UserName, QString Password);

private slots:
    void on_btnLogin_clicked();

private:
    Ui::dlgLogin *ui;
};

#endif // DLGLOGIN_H
