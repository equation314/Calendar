#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "const.h"
#include "setting.h"
#include "colormenu.h"
#include "daywidget.h"
#include "translator.h"
#include "abstractevent.h"

#include <QMainWindow>
#include <QGridLayout>
#include <QWidgetAction>
#include <QTranslator>

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

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    Ui::MainWindow *ui;
    QDate current_date;
    std::vector<EventLabelButton*> event_labels;
    std::vector<AbstractEvent*> event_list;
    std::map<QDate, QColor> day_color;

    Translator translator;

    QLabel* corner_label;
    DayWidget* day_table[Const::MONTH_WEEKS][Const::WEEK_DAYS];
    LabelButton* vertical_header[Const::MONTH_WEEKS];
    LabelButton* horizontal_header[Const::WEEK_DAYS];

    QAction *action_delete_event, *action_delete_one_event, *action_add_event;

    int dayFromColumn(int column) const { return (column + Setting::WeekFirstDay) % Const::WEEK_DAYS; }
    int columnFromDay(int day) const { return (day - Setting::WeekFirstDay + Const::WEEK_DAYS) % Const::WEEK_DAYS; }

    void createActions();
    void clearAll();
    void loadTable();
    void loadEvents();
    void importData(const QString &fileName, bool showMessageBox = false);
    void exportData(const QString &fileName, bool showMessageBox = false);

private slots:
    void onDayWidgetContextMenu(const QPoint &pos);

    void onShowDayDetail();

    void on_action_menu_triggered();
    void on_action_date_triggered();
    void on_action_left_triggered();
    void on_action_right_triggered();
    void on_action_add_triggered();

    void on_action_import_triggered();
    void on_action_export_triggered();
    void on_action_dragDrop_triggered(bool checked);
    void on_action_about_triggered();

    void on_action_today_triggered();
    void on_action_select_date_triggered();

    void on_action_preference_triggered();

signals:
    void tableUpdated();

};

#endif // MAINWINDOW_H
