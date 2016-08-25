#ifndef RECURRENTEVENT_H
#define RECURRENTEVENT_H

#include "const.h"
#include "abstractevent.h"

#include <set>
#include <QDebug>

class RecurrentEvent : public AbstractEvent
{
public:
    enum RecurrentType { Day, Week, Month, Year };

    explicit RecurrentEvent();
    explicit RecurrentEvent(const QDate& begin, const QDate& end);
    ~RecurrentEvent() {}

    EventType Type() const override { return EventType::RecurrentEvent; }
    bool InList(const QDate& date) const override;

    // Getter member functions
    RecurrentType GetRecurrentType() const { return type; }
    int Interval() const { return interval; }
    int DayMark() const { return day_mark; }
    int MonthDay() const { return month_day; }
    int MonthWeekday() const { return month_weekday; }
    int MonthWeekdayNum() const { return month_weekday_num; }
    int YearMonth() const { return year_month; }
    int EndType() const { return end_type; }
    int RepeatTimes() const { return repeat_times; }

    // Setter member functions
    void SetRecurrentType(RecurrentType x) { type = x; }
    void SetInterval(int x) { interval = x; }
    void SetDayMark(int x) { day_mark = x; }
    void SetMonthDay(int x) { month_day = x; }
    void SetMonthWeekday(int x) { month_weekday = x; }
    void SetMonthWeekdayNum(int x) { month_weekday_num = x; }
    void SetYearMonth(int x) { year_month = x; }
    void SetEndType(int x)
    {
        end_type = x;
        if (end_type == 2) repeat_end = end; else repeat_end = Const::MAX_DATE;
    }
    void SetRepeatTimes(int x);

    void AddSkip(const QDate& date) { skip_set.insert(date); }

private:
    RecurrentType type;
    int interval, day_mark, end_times;
    int month_day, month_weekday, month_weekday_num;
    int year_month;
    int end_type, repeat_times;
    QDate repeat_end;
    std::set<QDate> skip_set;

    virtual void save(QDataStream &dataStream) const override;
    virtual void load(QDataStream &dataStream) override;
};

#endif // RECURRENTEVENT_H
