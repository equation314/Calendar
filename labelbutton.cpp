#include "labelbutton.h"

#include <QDebug>

LabelButton::LabelButton(QWidget *parent) :
    QLabel(parent),
    text_color(Qt::black), background_color(Qt::white)
{

}

LabelButton::LabelButton(const QString& text, QWidget *parent) :
    QLabel(text, parent),
    text_color(Qt::black), background_color(Qt::white)
{

}

void LabelButton::Press()
{
    this->setStyleSheet(QString("QLabel{color:%1;background:%2;}").arg(text_color.name()).arg(background_color.darker(110).name()));
}

void LabelButton::Release()
{
    this->setStyleSheet(QString("QLabel{color:%1;background:%2;}").arg(text_color.name()).arg(background_color.name()));
}

void LabelButton::mousePressEvent(QMouseEvent *ev)
{
    Press();
    emit pressed();
    QLabel::mousePressEvent(ev);
}

void LabelButton::mouseReleaseEvent(QMouseEvent *ev)
{
    Release();
    emit released();
    QLabel::mouseReleaseEvent(ev);
}
