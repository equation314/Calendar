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
    void Press();
    void Release();

protected:
    virtual void mousePressEvent(QMouseEvent *ev) override;
    virtual void mouseReleaseEvent(QMouseEvent *ev) override;

private:
    QColor text_color;
    QColor background_color;

signals:
    void pressed();
    void released();
};

#endif // LABELBUTTON_H
