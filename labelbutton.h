#ifndef LABELBUTTON_H
#define LABELBUTTON_H

#include <QLabel>
#include <QMouseEvent>

class LabelButton : public QLabel
{
    Q_OBJECT
public:
    explicit LabelButton(QWidget *parent = nullptr);
    explicit LabelButton(const QString& text, QWidget *parent = nullptr);

    void SetHover(bool hover) { show_hover = hover;}

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
    void SetHoveringStyle();
    void SetPressedStyle();
    void SetReleasedStyle();

protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QString text;
    QColor text_color;
    QColor background_color;
    bool show_hover;

    void setup();

signals:
    void mouseLeft();
    void mouseHovering();
    void clicked();
    void pressed();
    void released();
};

#endif // LABELBUTTON_H
