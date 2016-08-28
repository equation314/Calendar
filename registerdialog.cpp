#include "registerdialog.h"
#include "ui_registerdialog.h"

#include <QCryptographicHash>

RegisterDialog::RegisterDialog(const std::map<QString, QString>& userData, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog),
    user_pswd(userData)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    ui->lineEdit_username->setFocus();
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_pushButton_create_clicked()
{
    QString user = ui->lineEdit_username->text();
    QString pswd = ui->lineEdit_password->text();
    QString pswd_2 = ui->lineEdit_password_2->text();
    if (!user.length())
    {
        ui->label_message->setText(tr("Username cannot be empty."));
        ui->lineEdit_username->setFocus();
        return;
    }
    if (user_pswd.find(user) != user_pswd.end())
    {
        ui->label_message->setText(tr("This username has been registered."));
        ui->lineEdit_username->selectAll();
        ui->lineEdit_username->setFocus();
        return;
    }
    for (auto i : user)
        if (!i.isLetter() && !i.isDigit() && i != '_')
        {
            ui->label_message->setText(tr("Username must contain only letters, digits or the underline character."));
            ui->lineEdit_username->selectAll();
            ui->lineEdit_username->setFocus();
            return;
        }
    if (pswd.length() < 6)
    {
        ui->label_message->setText(tr("Password must have at least 6 characters."));
        ui->lineEdit_password->selectAll();
        ui->lineEdit_password->setFocus();
        return;
    }
    if (pswd != pswd_2)
    {
        ui->label_message->setText(tr("Two passwords don't match."));
        ui->lineEdit_password_2->selectAll();
        ui->lineEdit_password_2->setFocus();
        return;
    }
    QByteArray md5 = QCryptographicHash::hash(pswd.toLocal8Bit(), QCryptographicHash::Md5);
    this->username = user;
    this->password = md5.toHex();
    this->accept();
}
