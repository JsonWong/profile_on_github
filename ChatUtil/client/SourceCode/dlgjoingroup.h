#ifndef DLGJOINGROUP_H
#define DLGJOINGROUP_H

#include <QDialog>

namespace Ui {
class DlgJoinGroup;
}

class DlgJoinGroup : public QDialog
{
    Q_OBJECT

public:
    explicit DlgJoinGroup(QWidget *parent = 0);
    ~DlgJoinGroup();

signals:
    void sendGroupName(QString GroupName);

private slots:
    void on_btnOK_clicked();

private:
    Ui::DlgJoinGroup *ui;
};

#endif // DLGJOINGROUP_H
