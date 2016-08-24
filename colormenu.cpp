#include "const.h"
#include "colormenu.h"

#include <QLabel>
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

    connect(colorActionMap, SIGNAL(mapped(int)), this, SIGNAL(colorSelected(int)));
    connect(actionOtherColor, SIGNAL(triggered()), this, SIGNAL(otherColorSelected()));
}
