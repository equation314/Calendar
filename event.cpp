#include "event.h"

Event::Event(const QString& title, const QDate& begin, const QDate& end, const QString& detail, QWidget* parent) :
    title(title), begin(begin), end(end), detail(detail), num(begin.daysTo(end) + 1)
{
    for (QDate date = begin; date <= end; date = date.addDays(1))
    {
        LabelButton* label = new LabelButton(title, parent);
        label->SetBackgroundColor(QColor(22,167,101));
        label->setMaximumHeight(20);
        label->hide();
        label_list.push_back(label);
    }
}

Event::~Event()
{
    for (auto i : label_list) i->deleteLater();
    label_list.clear();
}
