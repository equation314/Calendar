#include "dateselectdialog.h"
#include "ui_dateselectdialog.h"
#include <QDebug>

DateSelectDialog::DateSelectDialog(const QDate& date, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DateSelectDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    ui->calendarWidget->setCurrentPage(date.year(), date.month());
}

DateSelectDialog::~DateSelectDialog()
{
    delete ui;
}

QDate DateSelectDialog::SelectedDate() const
{
    return ui->calendarWidget->selectedDate();
}

void DateSelectDialog::on_calendarWidget_activated(const QDate& date)
{
    QDialog::accept();
}
