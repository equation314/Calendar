#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui
{
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    QString Username() const { return username; }

private slots:
    void on_pushButton_login_clicked();
    void on_pushButton_register_clicked();

private:
    Ui::LoginDialog *ui;
    std::map<QString, QString> user_pswd;
    QString username;

    void loadUserData();
};

#endif // LOGINDIALOG_H
