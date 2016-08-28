#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>

namespace Ui
{
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(const std::map<QString, QString>& userData, QWidget *parent = nullptr);
    ~RegisterDialog();

    QString Username() const { return username; }
    QString Password() const { return password; }

private:
    Ui::RegisterDialog *ui;
    std::map<QString, QString> user_pswd;
    QString username, password;

private slots:
    void on_pushButton_create_clicked();
};

#endif // REGISTERDIALOG_H
