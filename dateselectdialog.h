#ifndef DATESELECTDIALOG_H
#define DATESELECTDIALOG_H

#include <QDialog>

namespace Ui
{
class DateSelectDialog;
}

class DateSelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DateSelectDialog(const QDate& date, QWidget *parent = 0);
    ~DateSelectDialog();

    QDate SelectedDate() const;

private slots:
    void on_calendarWidget_activated(const QDate& date);

private:
    Ui::DateSelectDialog *ui;
};

#endif // DATESELECTDIALOG_H
