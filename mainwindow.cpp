#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addeventdialog.h"

#include <QDebug>
#include <QTableWidget>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_date(QDate::currentDate()),
    week_first_day(0)
{
    ui->setupUi(this);
    ui->label_date->setText(m_date.toString("MMMM yyyy"));
    QLabel* corner = new QLabel(this);
    corner->setStyleSheet("QLabel{background:white;}");
    ui->layout_table->addWidget(corner, 0, 0, 1, 1);

    for (int i = 0; i < Const::WEEK_DAYS; i++)
    {
        horizontal_header[i] = new LabelButton(QDate::shortDayName(!i ? 7 : i), this);
        horizontal_header[i]->setAlignment(Qt::AlignCenter);
        horizontal_header[i]->setFixedHeight(30);
        if (isWeekend(i))
            horizontal_header[i]->setStyleSheet("QLabel{color:red;background:white;}");
        else
            horizontal_header[i]->setStyleSheet("QLabel{background:white;}");
    }
    for (int i = 0; i < Const::MONTH_WEEKS; i++)
    {
        vertical_header[i] = new LabelButton(this);
        vertical_header[i]->setFixedWidth(40);
        vertical_header[i]->setAlignment(Qt::AlignCenter);
        vertical_header[i]->setStyleSheet("QLabel{background:white;}");

        for (int j = 0; j < 4; j++)
        {
            QWidget* empty = new QWidget(this);
            if (!j) empty->setFixedHeight(25);
            ui->layout_table->addWidget(empty, i * 4 + j + 1, 0, 1, 1);
        }
    }
    for (int i = 0; i < Const::MONTH_WEEKS; i++)
        for (int j = 0; j < Const::WEEK_DAYS; j++)
        {
            day_table[i][j] = new DayWidget(this);
            connect(day_table[i][j], &DayWidget::clicked, this, &MainWindow::onDayWidgetClicked);
        }

    event_list.push_back(new Event("hehe",m_date, m_date,"",this));
    event_list.push_back(new Event("233",m_date, m_date.addDays(15),"",this));
    event_list.push_back(new Event("fuck",m_date.addDays(-4), m_date.addDays(-1),"",this));

    loadTable();
}

MainWindow::~MainWindow()
{
    delete ui;
}

pair<int, int> MainWindow::dayPosition(const QDate& date)
{
    if (date < day_table[0][0]->Date() || date > day_table[Const::MONTH_WEEKS - 1][Const::WEEK_DAYS - 1]->Date())
        return make_pair(-1, -1);
    for (int i = 0; i < Const::MONTH_WEEKS; i++)
        for (int j = 0; j < Const::WEEK_DAYS; j++)
            if (day_table[i][j]->Date() == date) return make_pair(i, j);
    return make_pair(-1, -1);
}

void MainWindow::loadTable()
{
    for (int i = 0; i < Const::WEEK_DAYS; i++)
    {
        int day = dayFromColumn(i);
        ui->layout_table->addWidget(horizontal_header[day], 0, i + 1, 1, 1);
    }

    QDate first = m_date, day;
    first.setDate(m_date.year(), m_date.month(), 1);
    day = first.addDays(-first.dayOfWeek() + week_first_day);
    for (int i = 0, weekNum = day.weekNumber(); i < Const::MONTH_WEEKS; i++, weekNum++)
    {
        vertical_header[i]->setText(QString::number(weekNum));
        ui->layout_table->addWidget(vertical_header[i], i * 4 + 1, 0, 4, 1);

        for (int j = 0; j < Const::WEEK_DAYS; j++)
        {
            day_table[i][j]->ClearEvents();
            day_table[i][j]->SetDate(day);

            if (day.month() != first.month())
            {
                day_table[i][j]->SetTitleBackgroundColor(QColor(56, 116, 184));
                day_table[i][j]->SetContentBackgroundColor(QColor(36, 82, 156));
            }
            if (isWeekend(day.dayOfWeek()))
                day_table[i][j]->SetTitleTextColor(QColor(255, 0, 0));

            ui->layout_table->addWidget(day_table[i][j], i * 4 + 1, j + 1, 4, 1);
            day = day.addDays(1);
        }
    }
    loadEvents();
}

void MainWindow::loadEvents()
{
    int eventCount[Const::MONTH_WEEKS][Const::WEEK_DAYS] = {0};
    for (auto i : event_list)
    {
        for (int j = 0; j < i->DayCount(); j++) i->EventLabel(j)->hide();

        QDate begin = i->Begin(), end = i->End();
        if (begin < day_table[0][0]->Date() && end >= day_table[0][0]->Date())
            begin = day_table[0][0]->Date();
        auto pos = dayPosition(begin);
        if (pos.first < 0) continue;

        for (QDate date = begin; date <= end;)
        {
            int span = min(qint64(Const::WEEK_DAYS - pos.second), date.daysTo(end) + 1), num = 0;
            for (int j = 0; j < span; j++) num = max(num, eventCount[pos.first][pos.second + j]);

            i->EventLabel(date)->show();
            ui->layout_table->addWidget(i->EventLabel(date), pos.first * 4 + num + 2, pos.second + 1, 1, span);
            for (int j = 0; j < span; j++)
            {
                day_table[pos.first][pos.second + j]->AddEvent(i);
                eventCount[pos.first][pos.second + j] = num + 1;
            }

            pos.first++, pos.second = 0;
            if (pos.first == Const::MONTH_WEEKS) break;
            date = day_table[pos.first][pos.second]->Date();
        }
    }
}



void MainWindow::on_pushButton_left_clicked()
{
    m_date = m_date.addMonths(-1);
    loadTable();
    ui->label_date->setText(m_date.toString("MMMM yyyy"));
}

void MainWindow::on_pushButton_right_clicked()
{
    m_date = m_date.addMonths(1);
    loadTable();
    ui->label_date->setText(m_date.toString("MMMM yyyy"));
}

void MainWindow::onDayWidgetClicked(DayWidget *sender)
{
    AddEventDialog dialog(sender->Date(), this);
    if (dialog.exec() == QDialog::Accepted)
    {

    }
}
