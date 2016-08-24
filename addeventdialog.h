#ifndef ADDEVENTDIALOG_H
#define ADDEVENTDIALOG_H

#include "abstractevent.h"

#include <QDate>
#include <QDialog>
#include <QCheckBox>
#include <QGridLayout>
#include <QRadioButton>
#include <QStackedLayout>

namespace Ui
{
class WeekRepeatWidget;
class MonthRepeatWidget;
class YearRepeatWidget;
class AddEventDialog;
}

class WeekRepeatWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WeekRepeatWidget(QWidget *parent = nullptr);
    ~WeekRepeatWidget() { delete ui; }

    void SetCheckStatus(int mark)
    {
        for (int i = 1; i <= 7; i++)
            checkBox[i]->setChecked((mark >> i) & 1);
    }
    int DayMark() const
    {
        int mark = 0;
        for (int i = 1; i <= 7; i++)
            if (checkBox[i]->isChecked()) mark |= 1 << i;
        return mark;
    }

private:
    Ui::WeekRepeatWidget *ui;
    QCheckBox* checkBox[8];
};



class MonthRepeatWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MonthRepeatWidget(QWidget *parent = nullptr);
    ~MonthRepeatWidget() { delete ui; }

    int MonthType();
    int MonthDay();
    int MonthWeekday();
    int MonthWeekdayNum();

    void SetMonthDay(int x);
    void SetMonthWeekday(int x);
    void SetMonthWeekdayNum(int x);

private:
    Ui::MonthRepeatWidget *ui;
};



class YearRepeatWidget : public QWidget
{
    Q_OBJECT
public:
    explicit YearRepeatWidget(QWidget *parent = nullptr);
    ~YearRepeatWidget() { delete ui; }

    int MonthType();
    int MonthDay();
    int MonthWeekday();
    int MonthWeekdayNum();
    int YearMonth();

    void SetMonthDay(int x);
    void SetMonthWeekday(int x);
    void SetMonthWeekdayNum(int x);
    void SetYearMonth(int x);

private:
    Ui::YearRepeatWidget *ui;
};



class AddEventDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddEventDialog(const QDate& date, QWidget *parent = nullptr);
    explicit AddEventDialog(AbstractEvent* event, QWidget *parent = nullptr);
    ~AddEventDialog();

    AbstractEvent* GetEvent() const { return event; }

protected:
    void accept() override;

private:
    Ui::AddEventDialog *ui;
    bool is_editing;
    AbstractEvent* event;
    QDate begin, end;

    QStackedLayout* layout_repeat;
    WeekRepeatWidget* week_repeat_widget;
    MonthRepeatWidget* month_repeat_widget;
    YearRepeatWidget* year_repeat_widget;

    void setup();

private slots:
    void on_dateEdit_begin_dateChanged(const QDate &date);
    void on_dateEdit_end_dateChanged(const QDate &date);

    void on_comboBox_repeatType_currentIndexChanged(int index);
    void on_comboBox_endType_currentIndexChanged(int index);
    void on_groupBox_clicked(bool checked);
};

#endif // ADDEVENTDIALOG_H
