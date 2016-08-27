#ifndef COLORMENU_H
#define COLORMENU_H

#include <QMenu>
#include <QColor>

class ColorMenu : public QMenu
{
    Q_OBJECT

public:
    explicit ColorMenu(QWidget* parent = nullptr);
    explicit ColorMenu(const QString& text, QWidget* parent = nullptr);

    void SetDefaultColor(const QColor& color) { default_color = selected_color = color; }
    QColor SelectedColor() const { return selected_color; }

private:
    QColor default_color, selected_color;

    void setup();

private slots:
    void onSelectColor(int id);
    void onSelectOtherColor();

signals:
    void colorSelected(const QColor& color);
};

#endif // COLORMENU_H
