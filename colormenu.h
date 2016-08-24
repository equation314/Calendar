#ifndef COLORMENU_H
#define COLORMENU_H

#include <QMenu>

class ColorMenu : public QMenu
{
    Q_OBJECT
public:
    ColorMenu(const QString& text, QWidget* parent = nullptr);

signals:
    void otherColorSelected();
    void colorSelected(int colorId);
};

#endif // COLORMENU_H
