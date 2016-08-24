#include "addeventdialog.h"
#include "recurrentevent.h"
#include "continuousevent.h"
#include "daydetaildialog.h"
#include "ui_daydetaildialog.h"
#include "mainwindow.h"

#include <QSignalMapper>

DayDetailDialog::DayDetailDialog(DayWidget* date, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DayDetailDialog),
    date(date), parent(static_cast<MainWindow*>(parent)), spacer(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding))
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

    ui->label_day->setText(QString("%1 %2").arg(date->Date().toString(Qt::SystemLocaleLongDate))
                                           .arg(QDate::longDayName(date->Date().dayOfWeek())));

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
        QLabel* label = new QLabel("今天没有事件~", this);
        label->setFixedHeight(40);
        label->setAlignment(Qt::AlignCenter);
        ui->verticalLayout->addWidget(label);
        return;
    }

    for (int i = 0; i < date->EventCount(); i++)
    {
        AbstractEvent* event = date->EventAt(i);
        LabelButtonWithEvent* label = new LabelButtonWithEvent(event->Title(), event, this);
        label_list.push_back(label);

        qDebug()<<i<<' '<<event->Title()<<' '<<date->Date()<<event->InList(date->Date());

        label->setFixedHeight(40);
        label->SetMouseEnterColorShow(true);
        label->setContextMenuPolicy(Qt::CustomContextMenu);
        label->SetBackgroundColor(event->LabelColor());
        ui->verticalLayout->addWidget(label);

        connect(label, &QWidget::customContextMenuRequested, this->parent, &MainWindow::onEventLabelContextMenu);
        connect(label, &LabelButton::clicked, this->parent, &MainWindow::onEditEvent);
    }
    ui->verticalLayout->addItem(spacer);
}
