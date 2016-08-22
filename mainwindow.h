#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "calendartable.h"

#include <QMainWindow>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_left_clicked();

    void on_pushButton_right_clicked();

private:
    Ui::MainWindow *ui;
    QDate date;
    CalendarTable* calendar_table;
};

#endif // MAINWINDOW_H
