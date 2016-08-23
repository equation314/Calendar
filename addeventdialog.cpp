#include "event.h"
#include "addeventdialog.h"
#include "ui_weekrepeatwidget.h"
#include "ui_monthrepeatwidget.h"
#include "ui_yearrepeatwidget.h"
#include "ui_addeventdialog.h"

#include <QDebug>
#include <QMessageBox>

WeekRepeatWidget::WeekRepeatWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WeekRepeatWidget)
{
    ui->setupUi(this);

    checkBox[0] = new QCheckBox(tr("工作日"), this);
    ui->gridLayout->addWidget(checkBox[0], 1, 3, 1, 1);
    for (int i = 1; i <= 7; i++) checkBox[i] = new QCheckBox(QDate::shortDayName(i), this);
    for (int i = 0; i < 4; i++) ui->gridLayout->addWidget(checkBox[i + 1], 0, i, 1, 1);
    for (int i = 0; i < 3; i++) ui->gridLayout->addWidget(checkBox[i + 5], 1, i, 1, 1);

    connect(checkBox[0], &QCheckBox::clicked, this, [this](bool checked)
    {
        if (checked)
        {
            for (int i = 1; i <= 5; i++) checkBox[i]->setChecked(true);
            checkBox[6]->setChecked(false);
            checkBox[7]->setChecked(false);
        }
    });
    for (int i = 1; i <= 5; i++)
    {
        connect(checkBox[i], &QCheckBox::clicked, this, [this](bool checked)
        {
            if (!checked) checkBox[0]->setChecked(false);
        });
    }
    for (int i = 6; i <= 7; i++)
    {
        connect(checkBox[i], &QCheckBox::clicked, this, [this](bool checked)
        {
            if (checked) checkBox[0]->setChecked(false);
        });
    }
}





MonthRepeatWidget::MonthRepeatWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MonthRepeatWidget)
{
    ui->setupUi(this);

    for (int i = 1; i <= 31; i++) ui->comboBox_day->addItem(QString("第 %1 天").arg(i));
    ui->comboBox_day->addItem("最后一天");

    for (int i = 1; i <= 7; i++) ui->comboBox_weekday_2->addItem(QDate::longDayName(i));

    connect(ui->comboBox_day, &QComboBox::currentTextChanged, this, [this]()
    {
        this->ui->radioButton_day->setChecked(true);
    });
    connect(ui->comboBox_weekday_1, &QComboBox::currentTextChanged, this, [this]()
    {
        this->ui->radioButton_weekday->setChecked(true);
    });
    connect(ui->comboBox_weekday_2, &QComboBox::currentTextChanged, this, [this]()
    {
        this->ui->radioButton_weekday->setChecked(true);
    });
}




YearRepeatWidget::YearRepeatWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::YearRepeatWidget)
{
    ui->setupUi(this);

    for (int i = 1; i <= 12; i++) ui->comboBox_month->addItem(QDate::longMonthName(i));

    for (int i = 1; i <= 31; i++) ui->comboBox_day->addItem(QString("第 %1 天").arg(i));
    ui->comboBox_day->addItem("最后一天");

    for (int i = 1; i <= 7; i++) ui->comboBox_weekday_2->addItem(QDate::longDayName(i));

    connect(ui->comboBox_day, &QComboBox::currentTextChanged, this, [this]()
    {
        this->ui->radioButton_day->setChecked(true);
    });
    connect(ui->comboBox_weekday_1, &QComboBox::currentTextChanged, this, [this]()
    {
        this->ui->radioButton_weekday->setChecked(true);
    });
    connect(ui->comboBox_weekday_2, &QComboBox::currentTextChanged, this, [this]()
    {
        this->ui->radioButton_weekday->setChecked(true);
    });
}



AddEventDialog::AddEventDialog(Event* event, const bool isEditing, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEventDialog),
    is_editing(isEditing), event(event), begin(event->Begin()), end(event->End())
{
    setup();
}

AddEventDialog::AddEventDialog(const QDate& date, bool isEditing, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEventDialog),
    is_editing(isEditing), event(nullptr), begin(date), end(date)
{
    setup();
}

AddEventDialog::~AddEventDialog()
{
    delete ui;
}

void AddEventDialog::setup()
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    if (is_editing)
        this->setWindowTitle(tr("修改事件"));
    else
    {
        this->setWindowTitle(tr("新建事件"));
        ui->pushButton_delete->hide();
    }

    ui->dateEdit_begin->setDate(begin);
    ui->dateEdit_end->setDate(end);

    if (event)
    {
        ui->lineEdit_title->setText(event->Title());
        ui->lineEdit_place->setText(event->Place());
        ui->plainTextEdit_deail->setPlainText(event->Detail());
    }

    ui->comboBox_endType->addItem(end.toString("yyyy/M/d"));
    ui->spinBox_repeatTimes->hide();
    ui->label_5->hide();

    layout_repeat = new QStackedLayout;
    layout_repeat->addWidget(new QWidget(ui->groupBox));
    ui->widget_empty->setLayout(layout_repeat);
    ui->widget_empty->hide();

    // week
    week_repeat_widget = new WeekRepeatWidget(ui->groupBox);
    layout_repeat->addWidget(week_repeat_widget);

    // month
    month_repeat_widget = new MonthRepeatWidget(ui->groupBox);
    layout_repeat->addWidget(month_repeat_widget);

    // year
    year_repeat_widget = new YearRepeatWidget(ui->groupBox);
    layout_repeat->addWidget(year_repeat_widget);

    this->ui->lineEdit_title->setFocus();
}

void AddEventDialog::accept()
{
    if (ui->lineEdit_title->text().isEmpty())
    {
        QMessageBox::critical(this, "事件无效", "请输入主题！");
        return;
    }
    if (ui->groupBox->isChecked())
    {

    }
    else
    {
        if (event == nullptr) event = new Event(begin, end, static_cast<QWidget*>(this->parent()));
        event->ResetBeginEnd(begin, end);
        event->SetTitle(ui->lineEdit_title->text());
        event->SetPlace(ui->lineEdit_place->text());
        event->SetDetail(ui->plainTextEdit_deail->toPlainText());
    }
    QDialog::accept();
}



void AddEventDialog::on_dateEdit_begin_dateChanged(const QDate &date)
{
    begin = date;
    if (begin > end) ui->dateEdit_end->setDate(date);
}

void AddEventDialog::on_dateEdit_end_dateChanged(const QDate &date)
{
    end = date;
    ui->comboBox_endType->setItemText(2, date.toString("yyyy/M/d"));
    if (begin > end) ui->dateEdit_begin->setDate(date);
}

void AddEventDialog::on_comboBox_repeatType_currentIndexChanged(int index)
{
    ui->widget_empty->show();
    switch (index)
    {
    case 0: // day
        layout_repeat->setCurrentIndex(0);
        ui->widget_empty->hide();
        break;
    case 1: // week
        layout_repeat->setCurrentIndex(1);
        break;
    case 2: // month
        layout_repeat->setCurrentIndex(2);
        break;
    case 3: // year
        layout_repeat->setCurrentIndex(3);
        break;
    }
}

void AddEventDialog::on_comboBox_endType_currentIndexChanged(int index)
{
    if (index == 1)
    {
        ui->spinBox_repeatTimes->show();
        ui->label_5->show();
    }
    else
    {
        ui->spinBox_repeatTimes->hide();
        ui->label_5->hide();
    }
}
