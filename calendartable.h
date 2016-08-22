#ifndef CALENDARTABLE_H
#define CALENDARTABLE_H

#include <QTableWidget>
#include <QDate>

class CalendarTable : public QTableWidget
{
    Q_OBJECT
public:
    explicit CalendarTable(QDate date, QWidget* parent = nullptr);
    ~CalendarTable() {}

    void DecreaseMonth()
    {
        date = date.addMonths(-1);
        showCalendar();
    }
    void IncreaseMonth()
    {
        date = date.addMonths(1);
        showCalendar();
    }

    void setWeekFirstDay(int day) { week_first_day = day; }

private:
    static const int WEEK_DAYS;
    static const int MONTH_WEEKS;
    static const QStringList DAY_NAME;

    int week_first_day;
    QDate date;

    static bool isWeekend(int day) { return day % WEEK_DAYS == 0 || day % WEEK_DAYS == 6; }
    int dayFromColumn(int column) const { return (column + week_first_day) % WEEK_DAYS; }
    int columnFromDay(int day) const { return (day - week_first_day + WEEK_DAYS) % WEEK_DAYS; }
    void showCalendar();
};

#endif // CALENDARTABLE_H
