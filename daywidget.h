#ifndef DAYWIDGET_H
#define DAYWIDGET_H

#include "event.h"
#include "labelbutton.h"

#include <QWidget>
#include <QLabel>
#include <QDate>

class DayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DayWidget(QWidget *parent = nullptr);
    explicit DayWidget(const QDate& date, QWidget *parent = nullptr);
    ~DayWidget() {}

    QDate Date() const { return date; }
    void SetDate(const QDate& date)
    {
        this->date = date;
        title->setText(QString::number(date.day()));
    }

    void SetTitleTextColor(const QColor &color) { title->SetTextColor(color); }
    void SetTitleBackgroundColor(const QColor &color) { title->SetBackgroundColor(color); }
    void SetContentTextColor(const QColor &color) { content->SetBackgroundColor(color); }
    void SetContentBackgroundColor(const QColor &color) { content->SetBackgroundColor(color); }

    const int EventCount() const { return event_list.size(); }
    void ClearEvents() { event_list.clear(); }
    void AddEvent(Event* event) { event_list.push_back(event); }

public slots:

protected:
    virtual void mousePressEvent(QMouseEvent *ev) override;
    virtual void mouseReleaseEvent(QMouseEvent *ev) override;

private:
    QDate date;
    LabelButton *title, *content;
    std::vector<Event*> event_list;

    void setup();

signals:
    void clicked(DayWidget* sender);
};

#endif // DAYWIDGET_H
