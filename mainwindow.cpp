#include "setting.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addeventdialog.h"
#include "recurrentevent.h"
#include "continuousevent.h"
#include "daydetaildialog.h"
#include "preferencedialog.h"
#include "dateselectdialog.h"

#include <QDir>
#include <QDebug>
#include <QFileInfo>
#include <QDateEdit>
#include <QFileDialog>
#include <QMessageBox>

using namespace std;

static AbstractEvent* eventByAction;
static LabelButton* eventLabelByAction;
static QDate dateByAction;
static DayWidget* dayWidgetByAction;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    current_date(QDate::currentDate())
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);

    Setting::LoadSetting(Const::SETTING_FILE);
    QGuiApplication::setFont(Setting::InterfaceFont);
    this->setFont(Setting::InterfaceFont);

    corner_label = new QLabel(this);
    corner_label->setFixedWidth(40);
    corner_label->setStyleSheet("QLabel{background:white;}");
    ui->layout_table->addWidget(corner_label, 0, 0, 1, 1);

    for (int i = 0; i < Const::WEEK_DAYS; i++)
    {
        horizontal_header[i] = new LabelButton(QDate::shortDayName(!i ? 7 : i), this);
        horizontal_header[i]->setAlignment(Qt::AlignCenter);
        horizontal_header[i]->setFixedHeight(30);
    }
    for (int i = 0; i < Const::MONTH_WEEKS; i++)
    {
        vertical_header[i] = new LabelButton(this);
        vertical_header[i]->setFixedWidth(40);
        vertical_header[i]->setAlignment(Qt::AlignCenter);

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
            connect(day_table[i][j], &DayWidget::clicked, this, &MainWindow::onShowDayDetail);
            connect(day_table[i][j], &DayWidget::dropIn, this, &MainWindow::onAddFile);
            connect(day_table[i][j], &QWidget::customContextMenuRequested, this, &MainWindow::onDayWidgetContextMenu);
        }
    importData(QDir::currentPath() + "/" + Const::DEFAULT_DATA_FILE);
    createActions();
    loadTable();
}

MainWindow::~MainWindow()
{
    clearAll();
    delete ui;
}

void MainWindow::createActions()
{
    ui->toolBar->addAction(ui->action_menu);
    ui->toolBar->addAction(ui->action_date);
    ui->toolBar->addAction(ui->action_left);
    ui->toolBar->addAction(ui->action_right);
    ui->toolBar->addAction(ui->action_add);

    color_menu = new ColorMenu("设置背景颜色(&C)", this);
    action_add_event = new QAction("添加事件(&A)", this);
    action_delete_event = new QAction("删除事件(&D)", this);
    action_delete_one_event = new QAction("删除单个事件(&S)", this);

    connect(action_add_event, &QAction::triggered, this, &MainWindow::onAddEvent);
    connect(action_delete_event, &QAction::triggered, this, &MainWindow::onDeleteEvent);
    connect(action_delete_one_event, &QAction::triggered, this, &MainWindow::onDeleteOneEvent);
    connect(color_menu, &ColorMenu::colorSelected, this, &MainWindow::onColorSelected);
}

void MainWindow::clearAll()
{
    for (auto i : event_labels) i->deleteLater();
    for (auto i : event_list) i->deleteLater();
    event_labels.clear();
    event_list.clear();
    day_color.clear();
}

void MainWindow::loadTable()
{
    this->setWindowOpacity(Setting::Opacity / 10.0);
    QGuiApplication::setFont(Setting::InterfaceFont);
    QFont font = ui->label_date->font();
    font.setFamily(Setting::InterfaceFont.family());
    ui->action_dragDrop->setChecked(Setting::EnableDragsAndDrops);
    ui->label_date->setFont(font);
    ui->layout_table->setSpacing(Setting::CellSpace);
    for (int i = 0; i < Const::WEEK_DAYS; i++)
    {
        if (Const::IsWeekend(i))
            horizontal_header[i]->SetTextColor(Qt::red);
        else
            horizontal_header[i]->SetTextColor(Qt::black);
    }
    if (Setting::ShowWeekNumber)
    {
        corner_label->setFixedWidth(40);
        for (int i = 0; i < Const::MONTH_WEEKS; i++)
        {
            vertical_header[i]->show();
            vertical_header[i]->SetTextColor(Qt::black);
        }
    }
    else
    {
        corner_label->setFixedWidth(0);
        for (int i = 0; i < Const::MONTH_WEEKS; i++) vertical_header[i]->hide();
    }

    ui->label_date->setText(current_date.toString("MMMM yyyy"));
    for (int i = 0; i < Const::WEEK_DAYS; i++)
    {
        int day = dayFromColumn(i);
        ui->layout_table->addWidget(horizontal_header[day], 0, i + 1, 1, 1);
    }

    QDate first = current_date, day;
    first.setDate(current_date.year(), current_date.month(), 1);
    day = first.addDays(-first.dayOfWeek() + Setting::WeekFirstDay);
    for (int i = 0; i < Const::MONTH_WEEKS; i++)
    {
        vertical_header[i]->setText(QString::number(day.addDays(6).weekNumber()));
        ui->layout_table->addWidget(vertical_header[i], i * 4 + 1, 0, 4, 1);

        for (int j = 0; j < Const::WEEK_DAYS; j++)
        {
            day_table[i][j]->ClearEvents();
            day_table[i][j]->SetDate(day);
            day_table[i][j]->setAcceptDrops(Setting::EnableDragsAndDrops);

            bool isDark = day.month() != first.month();

            if (day_color.find(day) != day_color.end())
                day_table[i][j]->SetBackgroundThemeColor(day_color[day], isDark);
            else
                day_table[i][j]->SetBackgroundThemeColor(Setting::CellColor, isDark);

            if (Const::IsWeekend(day.dayOfWeek()))
                day_table[i][j]->SetTitleTextColor(Qt::red);
            else
                day_table[i][j]->SetTitleTextColor(Qt::black);

            ui->layout_table->addWidget(day_table[i][j], i * 4 + 1, j + 1, 4, 1);
            day = day.addDays(1);
        }
    }
    loadEvents();
    emit tableUpdated();
}

void MainWindow::loadEvents()
{
    int eventCount[Const::MONTH_WEEKS][Const::WEEK_DAYS] = {0};
    for (auto i : event_labels) i->deleteLater();
    event_labels.clear();

    for (auto i = event_list.rbegin(); i != event_list.rend(); i++)
    {
        AbstractEvent* event = *i;
        for (int i = 0; i < Const::MONTH_WEEKS; i++)
            for (int j = 0, span; span = 1, j < Const::WEEK_DAYS; j += span) if (event->InList(day_table[i][j]->Date()))
            {
                if (event->Type() == AbstractEvent::ContinuousEvent)
                {
                    for (span = 1; j + span < Const::WEEK_DAYS && event->InList(day_table[i][j + span]->Date()); span++);
                }
                else if (event->Type() == AbstractEvent::RecurrentEvent)
                    span = 1;

                int num = 0;
                for (int k = 0; k < span; k++)
                {
                    num = max(num, eventCount[i][j + k]);
                    day_table[i][j + k]->AddEvent(event);
                }

                if (num < 3)
                {
                    for (int k = 0; k < span; k++) eventCount[i][j + k] = num + 1;

                    EventLabelButton* label = new EventLabelButton(event->Title(), event, this);
                    label->setMaximumHeight(20);
                    event_labels.push_back(label);
                    label->setAcceptDrops(Setting::EnableDragsAndDrops);
                    ui->layout_table->addWidget(label, i * 4 + num + 2, j + 1, 1, span);

                    connect(label, &LabelButton::mouseLeave, this, [this]()
                    {
                        EventLabelButton* sender = static_cast<EventLabelButton*>(QObject::sender());
                        for (auto i : event_labels)
                            if (i->Event() == sender->Event()) i->ShowReleasedStyle();
                    });
                    connect(label, &LabelButton::mouseEnter, this, [this]()
                    {
                        EventLabelButton* sender = static_cast<EventLabelButton*>(QObject::sender());
                        for (auto i : event_labels)
                            if (i->Event() == sender->Event()) i->ShowEnterStyle();
                    });
                    connect(label, &LabelButton::pressed, this, [this]()
                    {
                        EventLabelButton* sender = static_cast<EventLabelButton*>(QObject::sender());
                        for (auto i : event_labels)
                            if (i->Event() == sender->Event()) i->ShowPressedStyle();
                    });
                    connect(label, &LabelButton::released, this, [this]()
                    {
                        EventLabelButton* sender = static_cast<EventLabelButton*>(QObject::sender());
                        for (auto i : event_labels)
                            if (i->Event() == sender->Event()) i->ShowReleasedStyle();
                    });
                }
            }
    }
}

void MainWindow::importData(const QString& fileName, bool showMessageBox)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        if (showMessageBox) QMessageBox::critical(0, "导入数据失败", QString("无法从数据文件 \"%1\" 导入！").arg(fileName));
        return;
    }

    clearAll();
    QDataStream in(&file);
    int size;
    in >> size;
    for (int i = 0; i < size; i++)
    {
        AbstractEvent* event;
        in >> &event;
        event_list.push_back(event);
    }
    in >> size;
    for (int i = 0; i < size; i++)
    {
        QDate date; QColor color;
        in >> date >> color;
        day_color[date] = color;
    }
    file.close();
    loadTable();
}

void MainWindow::exportData(const QString& fileName, bool showMessageBox)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        if (showMessageBox) QMessageBox::critical(0, "导出数据失败", QString("无法导出到数据文件 \"%1\"！").arg(fileName));
        return;
    }

    QDataStream out(&file);
    out << (int)event_list.size();
    for (auto i : event_list) out << i;
    out << (int)day_color.size();
    for (auto i : day_color) out << i.first << i.second;
    file.close();
}



void MainWindow::onDayWidgetContextMenu(const QPoint& pos)
{
    dayWidgetByAction = static_cast<DayWidget*>(QObject::sender());
    if (!dayWidgetByAction->rect().contains(pos)) return;

    eventByAction = nullptr;
    dateByAction = dayWidgetByAction->Date();
    color_menu->SetDefaultColor(dayWidgetByAction->ThemeColor());

    QMenu menu(this);
    menu.addAction(action_add_event);
    menu.addSeparator();
    menu.addMenu(color_menu);

    menu.exec(QCursor::pos());
}

void MainWindow::onEventLabelContextMenu(const QPoint& pos)
{
    EventLabelButton* label = static_cast<EventLabelButton*>(QObject::sender());
    AbstractEvent* event = label->Event();
    QPoint mousePos = label->pos() + pos;
    if (!label->rect().contains(pos)) return;

    eventLabelByAction = label;
    eventByAction = event;
    color_menu->SetDefaultColor(label->BackgroundColor());

    if (label->parent() == ui->centralWidget)
    {
        for (int i = 0; i < Const::MONTH_WEEKS; i++)
            for (int j = 0; j < Const::WEEK_DAYS; j++)
                if (day_table[i][j]->geometry().contains(mousePos))
                    dateByAction = day_table[i][j]->Date();
    }

    QMenu menu(this);
    if (event->Type() == AbstractEvent::ContinuousEvent)
    {
        action_delete_event->setText("删除事件(&D)");
        menu.addAction(action_delete_event);
        menu.addSeparator();
        menu.addMenu(color_menu);
    }
    else if (event->Type() == AbstractEvent::RecurrentEvent)
    {
        action_delete_event->setText("删除整个事件序列(&D)");
        menu.addAction(action_delete_event);
        menu.addAction(action_delete_one_event);
        menu.addSeparator();
        menu.addMenu(color_menu);
    }

    menu.exec(QCursor::pos());
}

void MainWindow::onAddEvent()
{
    AddEventDialog dialog(dateByAction, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        event_list.push_back(dialog.GetEvent());
        loadTable();
    }
}

void MainWindow::onEditEvent()
{
    EventLabelButton* label = static_cast<EventLabelButton*>(QObject::sender());
    AbstractEvent* event = label->Event();

    AddEventDialog dialog(event, this);
    if (dialog.exec() == QDialog::Accepted) loadTable();
}

void MainWindow::onDeleteEvent()
{
    if (QMessageBox::question(this, "确认删除", QString("确实要删除事件 \"%1\" 和它的所有附件吗？").arg(eventByAction->Title())) != QMessageBox::Yes)
        return;
    for (auto i = event_list.begin(); i != event_list.end(); i++)
        if (*i == eventByAction)
        {
            (*i)->RemoveAllFiles();
            delete *i;
            event_list.erase(i);
            break;
        }
    loadTable();
}

void MainWindow::onDeleteOneEvent()
{
    if (eventByAction->Type() == AbstractEvent::RecurrentEvent)
    {
        RecurrentEvent* event = static_cast<RecurrentEvent*>(eventByAction);
        event->AddSkip(dateByAction);
    }
    loadTable();
}

void MainWindow::onColorSelected(const QColor& color)
{
    if (color.isValid())
    {
        if (eventByAction)
            eventByAction->SetLabelColor(color);
        else
            day_color[dateByAction] = color;
        loadTable();
    }
}

void MainWindow::onShowDayDetail()
{
    DayWidget* sender = static_cast<DayWidget*>(QObject::sender());
    DayDetailDialog dialog(sender, this);

    dateByAction = sender->Date();
    connect(this, &MainWindow::tableUpdated, &dialog, &DayDetailDialog::loadLabels);

    dialog.exec();
}

void MainWindow::onAddFile(const QString& filePath)
{
    DayWidget* sender = static_cast<DayWidget*>(QObject::sender());

    ContinuousEvent* event = new ContinuousEvent(sender->Date(), sender->Date());
    event->SetTitle(QString("文件 \"%2\"").arg(QFileInfo(filePath).fileName()));
    event->AddFile(filePath);
    event_list.push_back(event);

    loadTable();
}

void MainWindow::onAddFileToEvent(const QString& filePath)
{
    EventLabelButton* sender = static_cast<EventLabelButton*>(QObject::sender());
    sender->Event()->AddFile(filePath);
    loadTable();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    exportData(QDir::currentPath() + "/" + Const::DEFAULT_DATA_FILE);
    QMainWindow::closeEvent(event);
}

void MainWindow::on_action_menu_triggered()
{
    QMenu menu(this);
    menu.addAction(ui->action_import);
    menu.addAction(ui->action_export);
    menu.addSeparator();
    menu.addAction(ui->action_dragDrop);
    menu.addSeparator();
    menu.addAction(ui->action_preference);
    menu.addAction(ui->action_about);
    menu.addSeparator();
    menu.addAction(ui->action_exit);

    QRect rect = ui->toolBar->actionGeometry(ui->action_menu);
    menu.exec(this->pos() + rect.bottomLeft());
}

void MainWindow::on_action_date_triggered()
{
    QMenu menu(this);
    menu.addAction(ui->action_today);
    menu.addSeparator();
    menu.addAction(ui->action_select_date);

    QRect rect = ui->toolBar->actionGeometry(ui->action_date);
    menu.exec(this->pos() + rect.bottomLeft());
}

void MainWindow::on_action_left_triggered()
{
    current_date = current_date.addMonths(-1);
    loadTable();
}

void MainWindow::on_action_right_triggered()
{
    current_date = current_date.addMonths(1);
    loadTable();
}

void MainWindow::on_action_add_triggered()
{
    if (current_date < QDate::currentDate())
        dateByAction = QDate::currentDate();
    else
    {
        dateByAction = current_date;
        if (current_date.year() != QDate::currentDate().year() || current_date.month() !=  QDate::currentDate().month())
            dateByAction.setDate(current_date.year(), current_date.month(), 1);
    }
    onAddEvent();
}

void MainWindow::on_action_import_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("导入数据文件"),
                                                          QDir::currentPath() + "/" + Const::DEFAULT_DATA_FILE,
                                                          tr("日历数据文件 (*.dat)"));
    if (!fileName.isEmpty()) importData(fileName, true);
}

void MainWindow::on_action_export_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("导出数据文件"),
                                                          QDir::homePath(),
                                                          tr("日历数据文件 (*.dat)"));
    if (!fileName.isEmpty()) exportData(fileName, true);
}

void MainWindow::on_action_dragDrop_triggered(bool checked)
{
    Setting::EnableDragsAndDrops = checked;
    loadTable();
}

void MainWindow::on_action_preference_triggered()
{
    PreferenceDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        Setting::SaveSetting(Const::SETTING_FILE);
        loadTable();
    }
}

void MainWindow::on_action_about_triggered()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("关于 Calendar");
    msgBox.setText(QString(
                      "<h2>Calendar 日历程序<br/></h2>"
                      "<p>程序设计训练 Project 1</p>"
                      "<p>基于 Qt 5.7.0<br/></p>"
                      "<p>版本: %1</p>"
                      "<p>构建时间: %2 - %3<br/></p>"
                      "<p>Copyright © 2016 清华大学 计算机系 贾越凯。</p>"
                      "<p>保留所有权利。<br/></p>"
                      "<p>项目主页: <a href=\"https://github.com/equation314/Calendar\">https://github.com/equation314/Calendar</a></p>"
                      ).arg("V1.0.0").arg(__DATE__).arg(__TIME__));
    msgBox.exec();
}

void MainWindow::on_action_today_triggered()
{
    current_date = QDate::currentDate();
    loadTable();
}

void MainWindow::on_action_select_date_triggered()
{
    DateSelectDialog dialog(current_date, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        current_date = dialog.SelectedDate();
        loadTable();
    }
}
