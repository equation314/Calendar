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


    layout->addWidget(title, 0, 0, 1, 1);
    layout->addWidget(content, 1, 0, 1, 1);

    title->setFixedHeight(25);
    title->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    title->setAlignment(Qt::AlignCenter);

    SetTitleBackgroundColor(QColor(78, 144, 216));
    SetContentBackgroundColor(QColor(47, 101, 188));

    //title->SetPressedColor(Color(64, 127, 195));
    //content->SetPressedColor();
}

void DayWidget::mousePressEvent(QMouseEvent *ev)
{
    title->SetPressedStyle();
    content->SetPressedStyle();
    QWidget::mousePressEvent(ev);
}

void DayWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    title->SetReleasedStyle();
    content->SetReleasedStyle();
    emit clicked();
    QWidget::mouseReleaseEvent(ev);
}
