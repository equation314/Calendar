#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addeventdialog.h"
#include "recurrentevent.h"
#include "continuousevent.h"
#include "daydetaildialog.h"

#include <QDir>
#include <QDebug>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QColorDialog>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_date(QDate::currentDate()),
    week_first_day(1)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->label_date->setText(m_date.toString("MMMM yyyy"));
    QLabel* corner = new QLabel(this);
    corner->setStyleSheet("QLabel{background:white;}");
    ui->layout_table->addWidget(corner, 0, 0, 1, 1);

    for (int i = 0; i < Const::WEEK_DAYS; i++)
    {
        horizontal_header[i] = new LabelButton(QDate::shortDayName(!i ? 7 : i), this);
        horizontal_header[i]->setAlignment(Qt::AlignCenter);
        horizontal_header[i]->setFixedHeight(30);
        if (Const::IsWeekend(i)) horizontal_header[i]->SetTextColor(QColor(255, 0, 0));
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
    ui->toolBar->addAction(new QAction(QIcon(":/icons/menu.png"), ""));
    ui->toolBar->addAction(new QAction(QIcon(":/icons/left.png"), ""));
    ui->toolBar->addAction(new QAction(QIcon(":/icons/right.png"), ""));
    ui->toolBar->addAction(new QAction(QIcon(":/icons/date.png"), ""));

    menu = new QMenu(this);
    menu_color = new ColorMenu("设置背景颜色(&C)", this);
    action_add_event = new QAction("添加事件(&A)", this);
    action_delete_event = new QAction("删除事件(&D)", this);
    action_delete_one_event = new QAction("删除单个事件(&S)", this);

    connect(action_add_event, &QAction::triggered, this, &MainWindow::onAddEvent);
    connect(action_delete_event, &QAction::triggered, this, &MainWindow::onDeleteEvent);
    connect(action_delete_one_event, &QAction::triggered, this, &MainWindow::onDeleteOneEvent);
    connect(menu_color, &ColorMenu::colorSelected, this, &MainWindow::onSelectColor);
    connect(menu_color, &ColorMenu::otherColorSelected, this, &MainWindow::onSelectOtherColor);
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
    for (int i = 0; i < Const::WEEK_DAYS; i++)
    {
        int day = dayFromColumn(i);
        ui->layout_table->addWidget(horizontal_header[day], 0, i + 1, 1, 1);
    }

    QDate first = m_date, day;
    first.setDate(m_date.year(), m_date.month(), 1);
    day = first.addDays(-first.dayOfWeek() + week_first_day);
    for (int i = 0; i < Const::MONTH_WEEKS; i++)
    {
        if (week_first_day == 1)
            vertical_header[i]->setText(QString::number(day.weekNumber()));
        else
            vertical_header[i]->setText("");
        ui->layout_table->addWidget(vertical_header[i], i * 4 + 1, 0, 4, 1);

        for (int j = 0; j < Const::WEEK_DAYS; j++)
        {
            day_table[i][j]->ClearEvents();
            day_table[i][j]->SetDate(day);

            bool isDark = day.month() != first.month();

            if (day_color.find(day) != day_color.end())
                day_table[i][j]->SetBackgroundThemeColor(day_color[day], isDark);
            else
                day_table[i][j]->SetBackgroundThemeColor(QColor(47, 101, 188), isDark);

            if (Const::IsWeekend(day.dayOfWeek()))
                day_table[i][j]->SetTitleTextColor(QColor(255, 0, 0));
            else
                day_table[i][j]->SetTitleTextColor(QColor(0, 0, 0));

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

                    LabelButtonWithEvent* label = new LabelButtonWithEvent(event->Title(), event, this);
                    label->setMaximumHeight(20);
                    event_labels.push_back(label);
                    ui->layout_table->addWidget(label, i * 4 + num + 2, j + 1, 1, span);

                    connect(label, &LabelButton::mouseLeave, this, [this]()
                    {
                        LabelButtonWithEvent* sender = static_cast<LabelButtonWithEvent*>(QObject::sender());
                        for (auto i : event_labels)
                            if (i->Event() == sender->Event()) i->ShowReleasedStyle();
                    });
                    connect(label, &LabelButton::mouseEnter, this, [this]()
                    {
                        LabelButtonWithEvent* sender = static_cast<LabelButtonWithEvent*>(QObject::sender());
                        for (auto i : event_labels)
                            if (i->Event() == sender->Event()) i->ShowEnterStyle();
                    });
                    connect(label, &LabelButton::pressed, this, [this]()
                    {
                        LabelButtonWithEvent* sender = static_cast<LabelButtonWithEvent*>(QObject::sender());
                        for (auto i : event_labels)
                            if (i->Event() == sender->Event()) i->ShowPressedStyle();
                    });
                    connect(label, &LabelButton::released, this, [this]()
                    {
                        LabelButtonWithEvent* sender = static_cast<LabelButtonWithEvent*>(QObject::sender());
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



static AbstractEvent* eventByAction;
static LabelButton* eventLabelByAction;
static QDate dateByAction;
static DayWidget* dayWidgetByAction;

void MainWindow::onDayWidgetContextMenu(const QPoint& pos)
{
    dayWidgetByAction = static_cast<DayWidget*>(QObject::sender());
    if (!dayWidgetByAction->rect().contains(pos)) return;

    menu->clear();
    menu->addAction(action_add_event);
    menu->addSeparator();
    menu->addMenu(menu_color);

    menu->popup(QCursor::pos());
}

void MainWindow::onEventLabelContextMenu(const QPoint& pos)
{
    LabelButtonWithEvent* label = static_cast<LabelButtonWithEvent*>(QObject::sender());
    AbstractEvent* event = label->Event();
    QPoint mousePos = label->pos() + pos;
    if (!label->rect().contains(pos)) return;

    dayWidgetByAction = nullptr;
    eventLabelByAction = label;
    eventByAction = event;

    if (label->parent() == ui->centralWidget)
    {
        for (int i = 0; i < Const::MONTH_WEEKS; i++)
            for (int j = 0; j < Const::WEEK_DAYS; j++)
                if (day_table[i][j]->geometry().contains(mousePos))
                    dateByAction = day_table[i][j]->Date();
    }

    menu->clear();
    if (event->Type() == AbstractEvent::ContinuousEvent)
    {
        action_delete_event->setText("删除事件(&D)");
        menu->addAction(action_delete_event);
        menu->addSeparator();
        menu->addMenu(menu_color);
    }
    else if (event->Type() == AbstractEvent::RecurrentEvent)
    {
        action_delete_event->setText("删除整个事件序列(&D)");
        menu->addAction(action_delete_event);
        menu->addAction(action_delete_one_event);
        menu->addSeparator();
        menu->addMenu(menu_color);
    }

    menu->popup(QCursor::pos());
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

void MainWindow::on_pushButton_import_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("导入数据文件"),
                                                          QDir::currentPath() + "/" + Const::DEFAULT_DATA_FILE,
                                                          tr("日历数据文件 (*.dat)"));
    if (!fileName.isEmpty()) importData(fileName, true);
}

void MainWindow::on_pushButton_export_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("导出数据文件"),
                                                          QDir::homePath(),
                                                          tr("日历数据文件 (*.dat)"));
    if (!fileName.isEmpty()) exportData(fileName, true);
}

void MainWindow::onAddEvent()
{
    AddEventDialog dialog(dayWidgetByAction->Date(), this);
    if (dialog.exec() == QDialog::Accepted)
    {
        event_list.push_back(dialog.GetEvent());
        loadTable();
    }
}

void MainWindow::onEditEvent()
{
    LabelButtonWithEvent* label = static_cast<LabelButtonWithEvent*>(QObject::sender());
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

void MainWindow::onSelectOtherColor()
{
    QColor color;
    if (dayWidgetByAction)
        color = QColorDialog::getColor(dayWidgetByAction->ThemeColor(), this);
    else
        color = QColorDialog::getColor(eventLabelByAction->BackgroundColor(), this);
    if (color.isValid())
    {
        if (dayWidgetByAction)
            day_color[dayWidgetByAction->Date()] = color;
        else
            eventByAction->SetLabelColor(color);
        loadTable();
    }
}

void MainWindow::onSelectColor(int colorId)
{
    if (dayWidgetByAction)
        day_color[dayWidgetByAction->Date()] = Const::COLOR_LIST[colorId];
    else
        eventByAction->SetLabelColor(Const::COLOR_LIST[colorId]);
    loadTable();
}

void MainWindow::onShowDayDetail()
{
    DayWidget* sender = static_cast<DayWidget*>(QObject::sender());
    DayDetailDialog dialog(sender, this);

    dayWidgetByAction = nullptr;
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
    LabelButtonWithEvent* sender = static_cast<LabelButtonWithEvent*>(QObject::sender());
    sender->Event()->AddFile(filePath);
    loadTable();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    exportData(QDir::currentPath() + "/" + Const::DEFAULT_DATA_FILE);
    QMainWindow::closeEvent(event);
}
