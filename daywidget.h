#ifndef DAYWIDGET_H
#define DAYWIDGET_H

#include "abstractevent.h"
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

    void SetDate(const QDate& date)
    {
        this->date = date;
        if (date.day() == 1)
            title->setText(date.toString("M/d"));
        else
            title->setText(QString::number(date.day()));
    }
    void SetBackgroundThemeColor(const QColor &color, bool isDark = false)
    {
        SetTitleBackgroundColor(isDark ? color : color.lighter(135));
        SetContentBackgroundColor(isDark ? color.darker(135) : color);
    }
    void SetTitleTextColor(const QColor &color) { title->SetTextColor(color); }
    void SetContentTextColor(const QColor &color) { content->SetBackgroundColor(color); }
    void SetTitleBackgroundColor(const QColor &color)
    {
        title_color = color;
        title->SetBackgroundColor(color);
    }
    void SetContentBackgroundColor(const QColor &color)
    {
        content_color = color;
        content->SetBackgroundColor(color);
    }

    QDate Date() const { return date; }
    QColor ThemeColor() const { return content_color; }

    void ClearEvents() { event_list.clear(); }
    int EventCount() const { return event_list.size(); }
    AbstractEvent* EventAt(int i) const { return event_list[i]; }
    void AddEvent(AbstractEvent* event) { event_list.push_back(event); }

public slots:

protected:
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;

private:
    QDate date;
    LabelButton *title, *content;
    std::vector<AbstractEvent*> event_list;
    QColor title_color, content_color;

    void setup();

signals:
    void clicked();
    void dropIn(const QString& filePath);
};

#endif // DAYWIDGET_H
