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
        this->setStyleSheet(QString("QLabel{color:%1;background:%2;}").arg(text_color.name()).arg(background_color.name()));
    }
    void SetBackgroundColor(const QColor& color)
    {
        background_color = color;
        this->setStyleSheet(QString("QLabel{color:%1;background:%2;}").arg(text_color.name()).arg(background_color.name()));
    }


public slots:
    void ShowEnterStyle();
    void ShowPressedStyle();
    void ShowReleasedStyle();

protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    virtual void dropEvent(QDropEvent *event) override;

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

class LabelButtonWithEvent : public LabelButton
{
public:
    explicit LabelButtonWithEvent(const QString& text, AbstractEvent* event, QWidget *parent = nullptr);

    AbstractEvent* Event() const { return event; }
    void SetEvent(AbstractEvent* event) { this->event = event; }

private:
    AbstractEvent* event;
};

#endif // LABELBUTTON_H
