#include "setting.h"
#include "labelbutton.h"
#include "mainwindow.h"

#include <QMimeData>
#include <QFileInfo>

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

void LabelButton::resizeEvent(QResizeEvent* event)
{
    if (!text.isEmpty())
        this->setText(QFontMetrics(this->font()).elidedText(text, Qt::ElideRight, this->width() - 3));
    QLabel::resizeEvent(event);
}

void LabelButton::leaveEvent(QEvent* event)
{
    QLabel::leaveEvent(event);
    ShowReleasedStyle();
    emit mouseLeave();
}

void LabelButton::mouseMoveEvent(QMouseEvent* event)
{
    if (!Setting::Movable) { event->ignore(); return;}
    QLabel::mouseMoveEvent(event);
    ShowEnterStyle();
    emit mouseEnter();
}

void LabelButton::mousePressEvent(QMouseEvent* event)
{
    if (!Setting::Movable) { event->ignore(); return;}
    QLabel::mousePressEvent(event);
    ShowPressedStyle();
    emit pressed();
}

void LabelButton::mouseReleaseEvent(QMouseEvent* event)
{
    if (!Setting::Movable) { event->ignore(); return;}
    QLabel::mouseReleaseEvent(event);
    ShowReleasedStyle();
    emit released();
    if (event->button() == Qt::LeftButton) emit clicked();
}

void LabelButton::dragEnterEvent(QDragEnterEvent* event)
{
    auto data = event->mimeData();
    if (Setting::EnableDragsAndDrops && data->hasUrls() && data->urls().size() == 1 && QFileInfo(data->urls().at(0).toLocalFile()).isFile())
        event->acceptProposedAction();
    else
        event->ignore();
}

void LabelButton::dropEvent(QDropEvent* event)
{
    auto data = event->mimeData();
    if (data->hasUrls() && data->urls().size() == 1)
    {
        QString file = data->urls().first().toLocalFile();
        if (QFileInfo(file).isFile()) emit dropIn(file);
    }
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
    if (!Setting::Movable) return;
    if (show_enter_color)
        this->setStyleSheet(QString("QLabel{color:%1;background:%2;}").arg(text_color.name()).arg(background_color.darker(110).name(QColor::HexArgb)));
}

void LabelButton::ShowPressedStyle()
{
    if (!Setting::Movable) return;
    this->setStyleSheet(QString("QLabel{color:%1;background:%2;}").arg(text_color.name()).arg(background_color.darker(120).name(QColor::HexArgb)));
}

void LabelButton::ShowReleasedStyle()
{
    if (!Setting::Movable) return;
    this->setStyleSheet(QString("QLabel{color:%1;background:%2;}").arg(text_color.name()).arg(background_color.name(QColor::HexArgb)));
}





EventLabelButton::EventLabelButton(const QString &text, AbstractEvent *event, QWidget *parent) :
    LabelButton(text, parent), event(event)
{
    this->setAcceptDrops(true);
    this->SetMouseEnterColorShow(true);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->SetBackgroundColor(event->LabelColor());
    this->setFont(Setting::EventFont);

    connect(this, &QWidget::customContextMenuRequested, static_cast<MainWindow*>(parent), &MainWindow::onEventLabelContextMenu);
    connect(this, &LabelButton::clicked, static_cast<MainWindow*>(parent), &MainWindow::onEditEvent);
    connect(this, &LabelButton::dropIn, static_cast<MainWindow*>(parent), &MainWindow::onAddFileToEvent);
}
