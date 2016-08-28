#include "const.h"
#include "logindialog.h"
#include "registerdialog.h"
#include "ui_logindialog.h"

#include <QFile>
#include <QMessageBox>
#include <QCryptographicHash>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    ui->lineEdit_username->setFocus();
    loadUserData();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::loadUserData()
{
    QFile file(Const::REGISTER_INFO_FILE);
    if (!file.open(QIODevice::ReadOnly)) return;

    QDataStream in(&file);
    int count;
    user_pswd.clear();

    in >> count;
    for (int i = 0; i < count; i++)
    {
        QString user, password;
        in >> user >> password;
        user_pswd[user] = password;
    }
    file.close();
}

void LoginDialog::on_pushButton_login_clicked()
{
    QByteArray md5 = QCryptographicHash::hash(ui->lineEdit_password->text().toLocal8Bit(), QCryptographicHash::Md5);
    QString user = ui->lineEdit_username->text();
    QString pswd = md5.toHex();

    if (!user.length())
    {
        ui->label_message->setText(tr("Please input username."));
        ui->lineEdit_username->setFocus();
        return;
    }
    if (user_pswd.find(user) == user_pswd.end() || user_pswd[user] != pswd)
    {
        ui->label_message->setText(tr("Incorrect username or password."));
        ui->lineEdit_password->selectAll();
        ui->lineEdit_password->setFocus();
        return;
    }
    this->username = user;
    this->accept();
}

void LoginDialog::on_pushButton_register_clicked()
{
    RegisterDialog dialog(user_pswd, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        QMessageBox::information(this, tr("Register Successfully"), tr("Register Successfully."));
        ui->lineEdit_username->clear();
        ui->lineEdit_username->setFocus();
        ui->lineEdit_password->clear();

        user_pswd[dialog.Username()] = dialog.Password();

        QFile file(Const::REGISTER_INFO_FILE);
        if (!file.open(QIODevice::WriteOnly)) return;

        QDataStream out(&file);
        out << (int)user_pswd.size();
        for (auto i : user_pswd)
            out << i.first << i.second;
        file.close();
    }
}
