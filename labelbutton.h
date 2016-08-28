#ifndef LABELBUTTON_H
#define LABELBUTTON_H

#include "abstractevent.h"

#include <QLabel>
#include <QMouseEvent>

class LabelButton : public QLabel
{
    Q_OBJECT

public:
    explicit LabelButton(QWidget *parent = nullptr);
    explicit LabelButton(const QString& text, QWidget *parent = nullptr);

    QColor BackgroundColor() const { return background_color; }

    void SetMouseEnterColorShow(bool show) { show_enter_color = show;}
    void SetTextColor(const QColor& color)
    {
        text_color = color;
        this->setStyleSheet(QString("QLabel{color:%1;background:%2;}").arg(text_color.name()).arg(background_color.name(QColor::HexArgb)));
    }
    void SetBackgroundColor(const QColor& color)
    {
        background_color = color;
        this->setStyleSheet(QString("QLabel{color:%1;background:%2;}").arg(text_color.name()).arg(background_color.name(QColor::HexArgb)));
    }


public slots:
    void ShowEnterStyle();
    void ShowPressedStyle();
    void ShowReleasedStyle();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    QString text;
    QColor text_color;
    QColor background_color;
    bool show_enter_color;

    void setup();

signals:
    void mouseLeave();
    void mouseEnter();
    void clicked();
    void pressed();
    void released();
    void dropIn(const QString& filePath);
};

class EventLabelButton : public LabelButton
{
public:
    explicit EventLabelButton(const QString& text, AbstractEvent* event, QWidget *parent = nullptr);

    AbstractEvent* Event() const { return event; }
    void SetEvent(AbstractEvent* event) { this->event = event; }
    void SetAcceptDrops(bool on) { this->setAcceptDrops(on); }

private:
    AbstractEvent* event;
};

#endif // LABELBUTTON_H
