#include "calendartable.h"

#include <QHeaderView>
#include <QScrollBar>
#include <QDebug>


const int CalendarTable::WEEK_DAYS = 7;
const int CalendarTable::MONTH_WEEKS = 6;
const QStringList CalendarTable::DAY_NAME =
{
    "星期日",
    "星期一",
    "星期二",
    "星期三",
    "星期四",
    "星期五",
    "星期六"
};


CalendarTable::CalendarTable(QDate date, QWidget* parent) :
    QTableWidget(parent), date(date), week_first_day(0)
{
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionMode(QAbstractItemView::NoSelection);

    this->horizontalHeader()->setHighlightSections(false);
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->verticalHeader()->setMinimumWidth(40);
    this->verticalHeader()->setHighlightSections(false);
    this->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    showCalendar();
}

void CalendarTable::showCalendar()
{
    this->clear();
    this->setRowCount(MONTH_WEEKS);
    this->setColumnCount(WEEK_DAYS);

    for (int i = 0; i < WEEK_DAYS; i++)
    {
        int day = dayFromColumn(i);
        QTableWidgetItem* item = new QTableWidgetItem(DAY_NAME[day]);
        if (isWeekend(day)) item->setTextColor(QColor(255, 0, 0));
        item->setTextAlignment(Qt::AlignCenter);
        this->setHorizontalHeaderItem(i, item);
    }

    QDate first = date, day;
    first.setDate(date.year(), date.month(), 1);
    day = first.addDays(-first.dayOfWeek() + week_first_day);
    for (int i = 0, weekNum = day.weekNumber(); i < MONTH_WEEKS; i++, weekNum++)
    {
        QTableWidgetItem* item = new QTableWidgetItem(QString::number(weekNum));
        item->setTextAlignment(Qt::AlignCenter);
        this->setVerticalHeaderItem(i, item);

        for (int j = 0; j < WEEK_DAYS; j++)
        {
            item = new QTableWidgetItem(QString::number(day.day()));
            item->setTextAlignment(Qt::AlignCenter);
            if (day.month() != first.month())
                item->setTextColor(QColor(150, 150, 150));
            else if (isWeekend(day.dayOfWeek()))
                item->setTextColor(QColor(255, 0, 0));
            this->setItem(i, j, item);
            day = day.addDays(1);
        }
    }
}
