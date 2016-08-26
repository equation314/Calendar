#include "const.h"
#include "addeventdialog.h"
#include "recurrentevent.h"
#include "continuousevent.h"
#include "ui_weekrepeatwidget.h"
#include "ui_monthrepeatwidget.h"
#include "ui_yearrepeatwidget.h"
#include "ui_addeventdialog.h"

#include <QDebug>
#include <QFileDialog>
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

int MonthRepeatWidget::MonthType() { return ui->radioButton_day->isChecked() ? 0 : 1; }
int MonthRepeatWidget::MonthDay()
{
    if (ui->comboBox_day->currentIndex() == 31) return 0; // last day
    return ui->comboBox_day->currentIndex() + 1;
}
int MonthRepeatWidget::MonthWeekday() { return ui->comboBox_weekday_2->currentIndex() + 1; }
int MonthRepeatWidget::MonthWeekdayNum()
{
    if (ui->comboBox_weekday_1->currentIndex() == 5) return 0; // last week
    return ui->comboBox_weekday_1->currentIndex() + 1;
}

void MonthRepeatWidget::SetMonthDay(int x)
{
    if (!x) ui->comboBox_day->setCurrentIndex(31);
    else ui->comboBox_day->setCurrentIndex(x - 1);
}
void MonthRepeatWidget::SetMonthWeekday(int x) { ui->comboBox_weekday_2->setCurrentIndex(x - 1); }
void MonthRepeatWidget::SetMonthWeekdayNum(int x)
{
    if (!x) ui->comboBox_weekday_1->setCurrentIndex(5);
    else ui->comboBox_weekday_1->setCurrentIndex(x - 1);
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

int YearRepeatWidget::MonthType() { return ui->radioButton_day->isChecked() ? 0 : 1; }
int YearRepeatWidget::MonthDay()
{
    if (ui->comboBox_day->currentIndex() == 31) return 0; // last day
    return ui->comboBox_day->currentIndex() + 1;
}
int YearRepeatWidget::MonthWeekday() { return ui->comboBox_weekday_2->currentIndex() + 1; }
int YearRepeatWidget::MonthWeekdayNum()
{
    if (ui->comboBox_weekday_1->currentIndex() == 5) return 0; // last week
    return ui->comboBox_weekday_1->currentIndex() + 1;
}
int YearRepeatWidget::YearMonth() { return ui->comboBox_month->currentIndex() + 1; }

void YearRepeatWidget::SetMonthDay(int x)
{
    if (!x) ui->comboBox_day->setCurrentIndex(31);
    else ui->comboBox_day->setCurrentIndex(x - 1);
}
void YearRepeatWidget::SetMonthWeekday(int x) { ui->comboBox_weekday_2->setCurrentIndex(x - 1); }
void YearRepeatWidget::SetMonthWeekdayNum(int x)
{
    if (!x) ui->comboBox_weekday_1->setCurrentIndex(5);
    else ui->comboBox_weekday_1->setCurrentIndex(x - 1);
}
void YearRepeatWidget::SetYearMonth(int x) { ui->comboBox_month->setCurrentIndex(x - 1); }





AddEventDialog::AddEventDialog(AbstractEvent* event, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEventDialog),
    is_editing(true), event(event), tmp_event(event), begin(event->Begin()), end(event->End())
{
    setup();
}

AddEventDialog::AddEventDialog(const QDate& date, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEventDialog),
    is_editing(false), event(nullptr), tmp_event(nullptr), begin(date), end(date)
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

    ui->dateEdit_begin->setDate(begin);
    ui->dateEdit_end->setDate(end);

    ui->comboBox_endType->addItem(end.toString(Qt::SystemLocaleShortDate));
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

    ui->lineEdit_title->setFocus();

    file_list_widget = new FileListWidget(this);
    ui->gridLayout_file->addWidget(file_list_widget, 1, 0, 1, 3);

    if (!is_editing)
    {
        this->setWindowTitle(tr("新建事件"));
    }
    else
    {
        this->setWindowTitle(tr("修改事件"));
        ui->lineEdit_title->setText(event->Title());
        ui->lineEdit_place->setText(event->Place());
        ui->plainTextEdit_deail->setPlainText(event->Detail());
        ui->groupBox->setEnabled(false);
        if (event->Type() == AbstractEvent::RecurrentEvent)
        {
            this->setWindowTitle(tr("修改重复事件"));
            ui->dateEdit_begin->setEnabled(false);
            ui->dateEdit_end->setEnabled(false);

            RecurrentEvent* revent = static_cast<RecurrentEvent*>(event);
            ui->groupBox->setChecked(true);
            ui->spinBox_interval->setValue(revent->Interval());
            ui->comboBox_repeatType->setCurrentIndex((int)revent->GetRecurrentType());
            ui->comboBox_endType->setCurrentIndex(revent->EndType());
            if (revent->EndType() == 1) ui->spinBox_repeatTimes->setValue(revent->RepeatTimes());
            switch (revent->GetRecurrentType())
            {
            case RecurrentEvent::Day:
                break;
            case RecurrentEvent::Week:
                week_repeat_widget->SetCheckStatus(revent->DayMark());
                break;
            case RecurrentEvent::Month:
                if (!revent->MonthWeekday())
                    month_repeat_widget->SetMonthDay(revent->MonthDay());
                else
                {
                    month_repeat_widget->SetMonthWeekday(revent->MonthWeekday());
                    month_repeat_widget->SetMonthWeekdayNum(revent->MonthWeekdayNum());
                }
                break;
            case RecurrentEvent::Year:
                year_repeat_widget->SetYearMonth(revent->YearMonth());
                if (!revent->MonthWeekday())
                    year_repeat_widget->SetMonthDay(revent->MonthDay());
                else
                {
                    year_repeat_widget->SetMonthWeekday(revent->MonthWeekday());
                    year_repeat_widget->SetMonthWeekdayNum(revent->MonthWeekdayNum());
                }
                break;
            }
        }

        connect(file_list_widget, &FileListWidget::fileRemoved, event, &AbstractEvent::RemoveFile);
        for (int i = 0; i < event->FileCount(); i++)
            file_list_widget->AddFile(event->FilePathAt(i));
    }
}

void AddEventDialog::accept()
{
    if (ui->lineEdit_title->text().isEmpty())
    {
        QMessageBox::critical(this, "事件无效", "请输入主题！");
        ui->lineEdit_title->setFocus();
        return;
    }
    if (ui->groupBox->isEnabled() && ui->groupBox->isChecked())
    {
        RecurrentEvent* revent;
        if (!event)
        {
            revent = new RecurrentEvent(begin, end);
            if (tmp_event) revent->Clone(tmp_event);
        }
        revent->SetTitle(ui->lineEdit_title->text());
        revent->SetPlace(ui->lineEdit_place->text());
        revent->SetDetail(ui->plainTextEdit_deail->toPlainText());
        revent->SetInterval(ui->spinBox_interval->value());
        switch (ui->comboBox_repeatType->currentIndex())
        {
        case 0: // day
            revent->SetRecurrentType(RecurrentEvent::Day);
            break;
        case 1: // week
            revent->SetRecurrentType(RecurrentEvent::Week);
            revent->SetDayMark(week_repeat_widget->DayMark());
            if (!revent->DayMark())
            {
                if (event == nullptr) delete revent;
                QMessageBox::critical(this, "事件无效", "请选择一周中的至少一天！");
                return;
            }
            break;
        case 2: // month
            revent->SetRecurrentType(RecurrentEvent::Month);
            if (!month_repeat_widget->MonthType())
                revent->SetMonthDay(month_repeat_widget->MonthDay());
            else
            {
                revent->SetMonthWeekday(month_repeat_widget->MonthWeekday());
                revent->SetMonthWeekdayNum(month_repeat_widget->MonthWeekdayNum());
            }
            break;
        case 3: // year
            revent->SetRecurrentType(RecurrentEvent::Year);
            revent->SetYearMonth(year_repeat_widget->YearMonth());
            if (!year_repeat_widget->MonthType())
                revent->SetMonthDay(year_repeat_widget->MonthDay());
            else
            {
                revent->SetMonthWeekday(year_repeat_widget->MonthWeekday());
                revent->SetMonthWeekdayNum(year_repeat_widget->MonthWeekdayNum());
            }
            break;
        }
        revent->SetEndType(ui->comboBox_endType->currentIndex());
        if (revent->EndType() == 1) revent->SetRepeatTimes(ui->spinBox_repeatTimes->value());
        event = revent;
    }
    else
    {
        if (!event)
        {
            event = new ContinuousEvent(begin, end);
            if (tmp_event) event->Clone(tmp_event);
        }
        event->ResetBeginEnd(begin, end);
        event->SetTitle(ui->lineEdit_title->text());
        event->SetPlace(ui->lineEdit_place->text());
        event->SetDetail(ui->plainTextEdit_deail->toPlainText());
    }
    QDialog::accept();
}

void AddEventDialog::reject()
{
    if (!is_editing && tmp_event)
    {
        tmp_event->RemoveAllFiles();
        tmp_event->deleteLater();
    }
    QDialog::reject();
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
    ui->dateEdit_end->setEnabled(index == 2);
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

void AddEventDialog::on_groupBox_clicked(bool checked)
{
    if (checked)
    {
        on_comboBox_endType_currentIndexChanged(ui->comboBox_endType->currentIndex());
    }
    else
    {
        ui->dateEdit_begin->setEnabled(true);
        ui->dateEdit_end->setEnabled(true);
    }
}

void AddEventDialog::on_pushButton_addFile_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, "选择附件", QDir::homePath(), "所有文件 (*)");
    if (!file.isEmpty())
    {
        if (!tmp_event)
        {
            tmp_event = new ContinuousEvent(begin, end);
            connect(file_list_widget, &FileListWidget::fileRemoved, tmp_event, &AbstractEvent::RemoveFile);
        }
        tmp_event->AddFile(file);
        file_list_widget->AddFile(tmp_event->FilePathAt(tmp_event->FileCount() - 1));
    }
}
