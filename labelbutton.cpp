#include "labelbutton.h"

#include <QDebug>

LabelButton::LabelButton(QWidget *parent) :
    QLabel(parent),
    text_color(Qt::black), background_color(Qt::white), show_enter_color(false)
{
    setup();
}

LabelButton::LabelButton(const QString& text, QWidget *parent) :
    QLabel(text, parent),
    text(text), text_color(Qt::black), background_color(Qt::white), show_enter_color(false)
{
    setup();
}

void LabelButton::setup()
{
    this->setIndent(3);
    this->setMouseTracking(true);
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    ShowReleasedStyle();
}

void LabelButton::ShowEnterStyle()
{
    if (show_enter_color)
        this->setStyleSheet(QString("QLabel{color:%1;background:%2;}").arg(text_color.name()).arg(background_color.darker(110).name()));
}

void LabelButton::ShowPressedStyle()
{
    this->setStyleSheet(QString("QLabel{color:%1;background:%2;}").arg(text_color.name()).arg(background_color.darker(120).name()));
}

void LabelButton::ShowReleasedStyle()
{
    this->setStyleSheet(QString("QLabel{color:%1;background:%2;}").arg(text_color.name()).arg(background_color.name()));
}

void LabelButton::resizeEvent(QResizeEvent *event)
{
    if (!text.isEmpty())
        this->setText(QFontMetrics(this->font()).elidedText(text, Qt::ElideRight, this->width() - 3));
    QLabel::resizeEvent(event);
}

void LabelButton::leaveEvent(QEvent *event)
{
    ShowReleasedStyle();
    emit mouseLeave();
    QLabel::leaveEvent(event);
}

void LabelButton::mouseMoveEvent(QMouseEvent *event)
{
    ShowEnterStyle();
    emit mouseEnter();
    QLabel::mouseMoveEvent(event);
}

void LabelButton::mousePressEvent(QMouseEvent *event)
{
    ShowPressedStyle();
    emit pressed();
    QLabel::mousePressEvent(event);
}

void LabelButton::mouseReleaseEvent(QMouseEvent *event)
{
    ShowReleasedStyle();
    emit released();
    if (event->button() == Qt::LeftButton) emit clicked();
    QLabel::mouseReleaseEvent(event);
}
