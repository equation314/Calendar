#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    date(QDate::currentDate()),
    calendar_table(new CalendarTable(date, this))
{
    ui->setupUi(this);
    ui->widget_empty->hide();
    ui->gridLayout->addWidget(calendar_table, 1, 0, 1, 3);
    ui->label_date->setText(date.toString("MMMM yyyy"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_left_clicked()
{
    date = date.addMonths(-1);
    calendar_table->DecreaseMonth();
    ui->label_date->setText(date.toString("MMMM yyyy"));
}

void MainWindow::on_pushButton_right_clicked()
{
    date = date.addMonths(1);
    calendar_table->IncreaseMonth();
    ui->label_date->setText(date.toString("MMMM yyyy"));
}
