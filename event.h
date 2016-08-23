#ifndef EVENT_H
#define EVENT_H

#include "labelbutton.h"

#include <QDate>
#include <QLabel>

class Event
{
public:
    Event(const QString& title, const QDate& begin, const QDate& end, const QString& detail = "", QWidget* parent = nullptr);
    ~Event();

    int DayCount() const { return num; }
    QDate Begin() const { return begin; }
    QDate End() const { return end; }
    QString Title() const { return title; }
    QString Detail() const { return detail; }
    QLabel* EventLabel(int i) const
    {
        if (i < 0 || i >= num) return nullptr;
        return label_list[i];
    }
    QLabel* EventLabel(const QDate& date) const
    {
        if (date < begin || date > end) return nullptr;
        return label_list[begin.daysTo(date)];
    }

private:
    QDate begin, end;
    QString title, detail;
    std::vector<LabelButton*> label_list;
    int num;
};

#endif // EVENT_H
