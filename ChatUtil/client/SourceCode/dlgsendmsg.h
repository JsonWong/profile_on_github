#ifndef DLGSENDMSG_H
#define DLGSENDMSG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class DlgSendMsg;
}

class DlgSendMsg : public QDialog
{
    Q_OBJECT

public:
    explicit DlgSendMsg(QWidget *parent = 0);
    ~DlgSendMsg();

    void setLabelText(QString s);
    void setMode(int mode);

signals:
    void sendMsg(int mode, QString Receiver, QString Msg);

private slots:
    void on_btnSend_clicked();

private:
    Ui::DlgSendMsg *ui;

    int m_mode;
};

#endif // DLGSENDMSG_H
