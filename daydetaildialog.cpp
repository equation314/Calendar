#include "setting.h"
#include "mainwindow.h"
#include "addeventdialog.h"
#include "recurrentevent.h"
#include "continuousevent.h"
#include "daydetaildialog.h"
#include "ui_daydetaildialog.h"

#include <QSignalMapper>

DayDetailDialog::DayDetailDialog(DayWidget* date, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DayDetailDialog),
    date(date), parent(static_cast<MainWindow*>(parent)), spacer(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding))
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

    QFont font = ui->label_day->font();
    font.setFamily(Setting::InterfaceFont.family());
    font.setPointSize(Setting::InterfaceFont.pointSize() * 2);
    ui->label_day->setFont(font);
    ui->label_day->setText(QString("%1").arg(Translator::Locale(Setting::Language).toString(date->Date(), QLocale::LongFormat)));

    loadLabels();
}

DayDetailDialog::~DayDetailDialog()
{
    delete ui;
}

void DayDetailDialog::loadLabels()
{
    for (auto i : label_list)
    {
        ui->verticalLayout->removeWidget(i);
        i->deleteLater();
    }
    ui->verticalLayout->removeItem(spacer);
    label_list.clear();

    if (!date->EventCount())
    {
        QLabel* label = new QLabel(tr("You Have No Events Today~"), this);
        label->setFixedHeight(40);
        label->setAlignment(Qt::AlignCenter);
        ui->verticalLayout->addWidget(label);
        return;
    }

    for (int i = 0; i < date->EventCount(); i++)
    {
        AbstractEvent* event = date->EventAt(i);
        EventLabelButton* label = new EventLabelButton(event->Title(), event, this->parent);
        label->setFixedHeight(40);
        label_list.push_back(label);
        label->setAcceptDrops(Setting::EnableDragsAndDrops);
        ui->verticalLayout->addWidget(label);
    }
    ui->verticalLayout->addItem(spacer);
}
