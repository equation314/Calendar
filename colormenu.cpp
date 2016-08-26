#include "const.h"
#include "colormenu.h"

#include <QLabel>
#include <QColorDialog>
#include <QWidgetAction>
#include <QSignalMapper>

ColorMenu::ColorMenu(const QString& text, QWidget* parent) :
    QMenu(text, parent)
{
    QAction* actionOtherColor = new QAction("其他(&O)...", this);
    QSignalMapper *colorActionMap = new QSignalMapper(this);
    for (int i = 0; i < Const::COLOR_LIST.size(); i++)
    {
        QWidgetAction* action = new QWidgetAction(this);
        QLabel* label = new QLabel(this);
        label->setFixedHeight(20);
        label->setStyleSheet(QString(
                    "QLabel"
                    "{"
                    "  margin:2;"
                    "  border:1 solid black;"
                    "  background:%1;"
                    "}"
                    "QLabel:hover"
                    "{"
                    "  margin:1;"
                    "  border:2 solid black;"
                    "}").arg(Const::COLOR_LIST[i].name()));
        action->setDefaultWidget(label);
        this->addAction(action);

        connect(action, SIGNAL(triggered()), colorActionMap, SLOT(map()));
        colorActionMap->setMapping(action, i);
    }
    this->addSeparator();
    this->addAction(actionOtherColor);

    connect(colorActionMap, SIGNAL(mapped(int)), this, SLOT(onSelectColor(int)));
    connect(actionOtherColor, SIGNAL(triggered()), this, SLOT(onSelectOtherColor()));
}

void ColorMenu::onSelectColor(int id)
{
    selected_color = Const::COLOR_LIST[id];
    emit colorSelected(selected_color);
}

void ColorMenu::onSelectOtherColor()
{
    selected_color = QColorDialog::getColor(default_color, static_cast<QWidget*>(this->parent()));
    if (selected_color.isValid()) emit colorSelected(selected_color);
}
