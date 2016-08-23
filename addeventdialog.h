#ifndef ADDEVENTDIALOG_H
#define ADDEVENTDIALOG_H

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

private:
    Ui::MonthRepeatWidget *ui;
};



class YearRepeatWidget : public QWidget
{
    Q_OBJECT
public:
    explicit YearRepeatWidget(QWidget *parent = nullptr);
    ~YearRepeatWidget() { delete ui; }

private:
    Ui::YearRepeatWidget *ui;
};



class AddEventDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddEventDialog(const QDate& date, QWidget *parent = nullptr);
    ~AddEventDialog();

protected:
    void accept() override;

private slots:
    void on_dateEdit_begin_dateChanged(const QDate &date);
    void on_dateEdit_end_dateChanged(const QDate &date);

    void on_comboBox_repeatType_currentIndexChanged(int index);
    void on_comboBox_endType_currentIndexChanged(int index);

private:
    Ui::AddEventDialog *ui;
    QDate begin, end;

    QStackedLayout* layout_repeat;
    WeekRepeatWidget* week_repeat_widget;
    MonthRepeatWidget* month_repeat_widget;
    YearRepeatWidget* year_repeat_widget;
};

#endif // ADDEVENTDIALOG_H
