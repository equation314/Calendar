#include "const.h"
#include "colormenu.h"

#include <QLabel>
#include <QColorDialog>
#include <QWidgetAction>
#include <QSignalMapper>

ColorMenu::ColorMenu(QWidget* parent) :
    QMenu(parent)
{
    setup();
}

ColorMenu::ColorMenu(const QString& title, QWidget* parent) :
    QMenu(title, parent)
{
    setup();
}

void ColorMenu::setup()
{
    QAction* actionOtherColor = new QAction(tr("&Other..."), this);
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
    emit colorSelected(selected_color = Const::COLOR_LIST[id]);
}

void ColorMenu::onSelectOtherColor()
{
    QColor color= QColorDialog::getColor(default_color, static_cast<QWidget*>(this->parent()));
    if (color.isValid())
    {
        selected_color = color;
        emit colorSelected(selected_color);
    }
}
