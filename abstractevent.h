#ifndef ABSTRACTEVENT_H
#define ABSTRACTEVENT_H

#include <QDate>
#include <QLabel>
#include <QObject>

class AbstractEvent : public QObject
{
    Q_OBJECT
public:
    enum EventType { ContinuousEvent, RecurrentEvent };

    explicit AbstractEvent(const QDate& begin, const QDate& end);//QWidget* parent = nullptr);
     virtual ~AbstractEvent();

    QDate Begin() const { return begin; }
    QDate End() const { return end; }
    QString Title() const { return title; }
    QString Place() const { return place; }
    QString Detail() const { return detail; }
    QColor LabelColor() const { return color; }


    void ResetBeginEnd(const QDate& begin, const QDate& end)
    {
        this->begin = begin;
        this->end = end;
    }
    void SetTitle(const QString& text) { title = text; }
    void SetPlace(const QString& text) { place = text; }
    void SetDetail(const QString& text) { detail = text; }
    void SetLabelColor(const QColor& col) { color = col; }

    virtual EventType Type() const = 0;
    virtual bool InList(const QDate& date) const = 0;

protected:
    QDate begin, end;

private:
    QString title, place, detail;
    QColor color;
};

#endif // ABSTRACTEVENT_H
