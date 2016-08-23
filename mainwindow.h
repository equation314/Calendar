#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "const.h"
#include "event.h"
#include "daywidget.h"

#include <QMainWindow>
#include <QGridLayout>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QDate m_date;
    std::vector<Event*> event_list;
    int week_first_day;

    static bool isWeekend(int day) { return day % Const::WEEK_DAYS == 0 || day % Const::WEEK_DAYS == 6; }
    int dayFromColumn(int column) const { return (column + week_first_day) % Const::WEEK_DAYS; }
    int columnFromDay(int day) const { return (day - week_first_day + Const::WEEK_DAYS) % Const::WEEK_DAYS; }

    //CalendarTable* calendar_table;
    //QGridLayout* event_table;
    DayWidget* day_table[Const::MONTH_WEEKS][Const::WEEK_DAYS];
    LabelButton* vertical_header[Const::MONTH_WEEKS];
    LabelButton* horizontal_header[Const::WEEK_DAYS];
    void loadTable();
    void loadEvents();
    std::pair<int, int> dayPosition(const QDate &date);

private slots:
    void on_pushButton_left_clicked();
    void on_pushButton_right_clicked();
    void onDayWidgetClicked(DayWidget* sender);
};

#endif // MAINWINDOW_H
