#include "setting.h"
#include "recurrentevent.h"

RecurrentEvent::RecurrentEvent(const QDate& begin, const QDate& end) :
    AbstractEvent(begin, end),
    interval(1), month_weekday(0), repeat_end(end)
{
    color = Setting::RecurrentEventColor;
}

bool RecurrentEvent::InList(const QDate& date) const
{
    if (date < begin || date > repeat_end) return false;
    if (skip_set.find(date) != skip_set.end()) return false;
    switch (type)
    {
    case Day:
        if (begin.daysTo(date) % interval) return false;
        return true;
    case Week:
    {
        int x;
        QDate nextWeekFirstDay = begin.addDays((Setting::WeekFirstDay - begin.dayOfWeek() + Const::WEEK_DAYS) % Const::WEEK_DAYS);
        if (nextWeekFirstDay > date) x = 0; else x = nextWeekFirstDay.daysTo(date) / Const::WEEK_DAYS + 1;
        if (x % interval) return false;

        return (day_mark >> date.dayOfWeek()) & 1;
    }
    case Year:
        if ((date.year() - begin.year()) % interval) return false;
        if (date.month() != year_month) return false;
    case Month:
        if (type == Month)
        {
            int x = date.month() - begin.month() + (date.year() - begin.year()) * 12;
            if (x % interval) return false;
        }
        if (!month_weekday) // case 1
        {
            if (!month_day) return date.day() == date.daysInMonth();
            else return date.day() == month_day;
        }
        else                // case 2
        {
            if (date.dayOfWeek() != month_weekday) return false;
            if (!month_weekday_num) return date.addDays(7).month() != date.month();
            int x = 0;
            for (QDate i = date; i.month() == date.month(); i = i.addDays(-7), x++);
            return x == month_weekday_num;
        }
    default:
        return false;
    }
}

void RecurrentEvent::SetRepeatTimes(int x)
{
    repeat_times = x;
    for (repeat_end = begin; x; repeat_end = repeat_end.addDays(1))
        if (InList(repeat_end)) x--;
    repeat_end.addDays(-1);
}

void RecurrentEvent::save(QDataStream &dataStream) const
{
    dataStream << (int)type;
    dataStream << interval << day_mark << end_times;
    dataStream << month_day << month_weekday << month_weekday_num;
    dataStream << year_month;
    dataStream << end_type << repeat_times;
    dataStream << repeat_end;
    dataStream << (int)skip_set.size();
    for (auto i : skip_set) dataStream << i;
}

void RecurrentEvent::load(QDataStream &dataStream)
{
    int x;
    dataStream >> x;
    type = (RecurrentType)x;
    dataStream >> interval >> day_mark >> end_times;
    dataStream >> month_day >> month_weekday >> month_weekday_num;
    dataStream >> year_month;
    dataStream >> end_type >> repeat_times;
    dataStream >> repeat_end;
    int size;
    dataStream >> size;
    skip_set.clear();
    for (int i = 0; i < size; i++)
    {
        QDate date;
        dataStream >> date;
        skip_set.insert(date);
    }
}
