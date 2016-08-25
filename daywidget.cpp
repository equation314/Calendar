#include "daywidget.h"

#include <QGridLayout>
#include <QDebug>

DayWidget::DayWidget(QWidget *parent) :
    QWidget(parent),
    title(new LabelButton(this)), content(new LabelButton(this))
{
    setup();
}

DayWidget::DayWidget(const QDate& date, QWidget *parent) :
    QWidget(parent),
    date(date), title(new LabelButton(QString::number(date.day()), this)), content(new LabelButton(this))
{
    setup();
}

void DayWidget::setup()
{
    QGridLayout* layout = new QGridLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    this->setLayout(layout);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    title->setAcceptDrops(true);
    content->setAcceptDrops(true);

    layout->addWidget(title, 0, 0, 1, 1);
    layout->addWidget(content, 1, 0, 1, 1);

    title->setFixedHeight(25);
    title->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    title->setAlignment(Qt::AlignCenter);

    SetBackgroundThemeColor(Qt::white);

    connect(title,   &LabelButton::pressed, content, &LabelButton::ShowPressedStyle);
    connect(content, &LabelButton::pressed, title,   &LabelButton::ShowPressedStyle);

    connect(title,   &LabelButton::released, content, &LabelButton::ShowReleasedStyle);
    connect(content, &LabelButton::released, title,   &LabelButton::ShowReleasedStyle);

    connect(title,   &LabelButton::dropIn, this, &DayWidget::dropIn);
    connect(content, &LabelButton::dropIn, this, &DayWidget::dropIn);
}

void DayWidget::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton)
    {
        title->ShowPressedStyle();
        content->ShowPressedStyle();
    }
    QWidget::mousePressEvent(ev);
}

void DayWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton)
    {
        title->ShowReleasedStyle();
        content->ShowReleasedStyle();
        emit clicked();
    }
    QWidget::mouseReleaseEvent(ev);
}
