#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "const.h"
#include "colormenu.h"
#include "daywidget.h"
#include "abstractevent.h"

#include <QMainWindow>
#include <QGridLayout>
#include <QWidgetAction>

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

public slots:
    void onEventLabelContextMenu(const QPoint &pos);
    void onAddFile(const QString &filePath);
    void onAddFileToEvent(const QString &filePath);
    void onAddEvent();
    void onEditEvent();
    void onDeleteEvent();
    void onDeleteOneEvent();

private:
    Ui::MainWindow *ui;
    QDate m_date;
    std::vector<LabelButtonWithEvent*> event_labels;
    std::vector<AbstractEvent*> event_list;
    std::map<QDate, QColor> day_color;
    int week_first_day;

    DayWidget* day_table[Const::MONTH_WEEKS][Const::WEEK_DAYS];
    LabelButton* vertical_header[Const::MONTH_WEEKS];
    LabelButton* horizontal_header[Const::WEEK_DAYS];

    QMenu* menu;
    ColorMenu* menu_color;
    QAction *action_delete_event, *action_delete_one_event, *action_add_event;

    int dayFromColumn(int column) const { return (column + week_first_day) % Const::WEEK_DAYS; }
    int columnFromDay(int day) const { return (day - week_first_day + Const::WEEK_DAYS) % Const::WEEK_DAYS; }

    void createActions();
    void loadTable();
    void loadEvents();

private slots:
    void on_pushButton_left_clicked();
    void on_pushButton_right_clicked();

    void onDayWidgetContextMenu(const QPoint &pos);
    void onSelectColor(int colorId);
    void onSelectOtherColor();
    void onShowDayDetail();

signals:
    void tableUpdated();

};

#endif // MAINWINDOW_H
