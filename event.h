#ifndef EVENT_H
#define EVENT_H

#include "labelbutton.h"

#include <QDate>
#include <QLabel>
#include <QObject>

class Event : public QObject
{
    Q_OBJECT
public:
    explicit Event(const QDate& begin, const QDate& end, QWidget* parent = nullptr);
    explicit Event(const QString& title, const QDate& begin, const QDate& end, const QString& place, const QString& detail, QWidget* parent = nullptr);
    ~Event();

    int DayCount() const { return num; }
    QString Title() const { return title; }
    QDate Begin() const { return begin; }
    QDate End() const { return end; }
    QString Place() const { return place; }
    QString Detail() const { return detail; }


    //void SetBegin(const QDate& date) { begin = date; }
    //void SetEnd(const QDate& date) { end = date; }
    void ResetBeginEnd(const QDate& begin, const QDate& end);
    void SetTitle(const QString& text)
    {
        title = text;
        for (auto i : label_list) i->setText(text);
    }
    void SetPlace(const QString& text) { place = text; }
    void SetDetail(const QString& text) { detail = text; }


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
    QWidget* parent;
    QDate begin, end;
    QString title, place, detail;
    std::vector<LabelButton*> label_list;
    int num;

    void setup();

private slots:
    void onMouseLeft();
    void onMouseHovering();
    void onMousePressed();

signals:
    void clicked();
};

#endif // EVENT_H
