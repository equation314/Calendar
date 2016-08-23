#include "event.h"

#include <QDebug>

Event::Event(const QDate& begin, const QDate& end, QWidget* parent) :
    parent(parent), begin(begin), end(end), num(begin.daysTo(end) + 1)
{
    setup();
}

Event::Event(const QString& title, const QDate& begin, const QDate& end, const QString& place, const QString& detail, QWidget* parent) :
    parent(parent), begin(begin), end(end), title(title), place(place), detail(detail), num(begin.daysTo(end) + 1)
{
    setup();
}

Event::~Event()
{
    for (auto i : label_list) i->deleteLater();
    label_list.clear();
}

void Event::ResetBeginEnd(const QDate &begin, const QDate &end)
{
    if (begin == this->begin && end == this->end) return;
    this->begin = begin;
    this->end = end;
    if (end < begin) return;

    for (auto i : label_list) i->deleteLater();
    label_list.clear();

    setup();
}

void Event::setup()
{
    for (QDate date = begin; date <= end; date = date.addDays(1))
    {
        LabelButton* label = new LabelButton(title, parent);
        label->setMaximumHeight(20);
        label->hide();
        label->SetHover(true);
        label->SetBackgroundColor(QColor(22,167,101));
        label_list.push_back(label);

        connect(label, &LabelButton::clicked, this, &Event::clicked);
        connect(label, &LabelButton::pressed, this, &Event::onMousePressed);
        connect(label, &LabelButton::mouseHovering, this, &Event::onMouseHovering);
        connect(label, &LabelButton::mouseLeft, this, &Event::onMouseLeft);
    }
}

void Event::onMouseLeft()
{
    for (auto i : label_list) i->SetReleasedStyle();
}

void Event::onMouseHovering()
{
    for (auto i : label_list) i->SetHoveringStyle();
}

void Event::onMousePressed()
{
    for (auto i : label_list) i->SetPressedStyle();
}
