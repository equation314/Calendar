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
#include <QDesktopWidget>

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
    Setting::LoadSetting(Const::SETTING_FILE);
    QGuiApplication::setFont(Setting::InterfaceFont);
    translator.InstallToApplication(Setting::Language);

    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);// | Qt::WindowTransparentForInput);
    this->setAttribute(Qt::WA_TranslucentBackground);

    this->setFont(Setting::InterfaceFont);
    this->move((QApplication::desktop()->width()  - this->width())  / 2,
               (QApplication::desktop()->height() - this->height()) / 2);

    corner_label = new QLabel(this);
    corner_label->setFixedWidth(40);
    corner_label->setStyleSheet("QLabel{background:white;}");
    ui->layout_table->addWidget(corner_label, 0, 0, 1, 1);

    for (int i = 0; i < Const::WEEK_DAYS; i++)
    {
        horizontal_header[i] = new LabelButton(this);
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
    createActions();
    importData(QDir::currentPath() + "/" + Const::DEFAULT_DATA_FILE);
}

MainWindow::~MainWindow()
{
    clearAll();
    delete ui;
}

static QPoint dragPosition;
static bool isDrag;
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isDrag = true;
        dragPosition = event->globalPos() - this->pos();
    }
    QWidget::mousePressEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (!Setting::Movable) { event->ignore(); return; }
    if (isDrag && event->buttons() == Qt::LeftButton && ui->toolBar->geometry().contains(event->pos()))
        move(event->globalPos() - dragPosition);
    QWidget::mouseMoveEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    isDrag = false;
    QWidget::mouseMoveEvent(event);
}

void MainWindow::createActions()
{
    ui->toolBar->addAction(ui->action_menu);
    ui->toolBar->addAction(ui->action_date);
    ui->toolBar->addAction(ui->action_left);
    ui->toolBar->addAction(ui->action_right);
    ui->toolBar->addAction(ui->action_add);
    ui->toolBar->addAction(ui->action_movable);
    ui->toolBar->addWidget(ui->label_date);

    action_add_event = new QAction(this);
    action_delete_event = new QAction(this);
    action_delete_one_event = new QAction(this);

    connect(action_add_event, &QAction::triggered, this, &MainWindow::onAddEvent);
    connect(action_delete_event, &QAction::triggered, this, &MainWindow::onDeleteEvent);
    connect(action_delete_one_event, &QAction::triggered, this, &MainWindow::onDeleteOneEvent);
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
    translator.InstallToApplication(Setting::Language);
    ui->retranslateUi(this);
    this->setWindowOpacity(Setting::Opacity / 10.0);

    QGuiApplication::setFont(Setting::InterfaceFont);
    QFont font = ui->label_date->font();
    font.setFamily(Setting::InterfaceFont.family());
    font.setPointSize(Setting::InterfaceFont.pointSize() * 2);
    ui->label_date->setFont(font);
    ui->label_date->setText(Translator::Locale(Setting::Language).toString(current_date, "MMMM yyyy"));

    ui->action_movable->setToolTip(Setting::Movable ? tr("Fix") : tr("Move"));
    ui->action_dragDrop->setChecked(Setting::EnableDragsAndDrops);
    ui->layout_table->setSpacing(Setting::CellSpace);
    ui->toolBar->setStyleSheet(QString("QToolBar{background:%1;}").arg(Setting::CellColor.light(140).name()));

    for (int i = 0; i < Const::WEEK_DAYS; i++)
    {
        horizontal_header[i]->setText(Translator::Locale(Setting::Language).dayName(!i ? 7 : i, QLocale::ShortFormat));
        horizontal_header[i]->SetBackgroundColor(Setting::CellColor.darker(135));
        if (Const::IsWeekend(i))
            horizontal_header[i]->SetTextColor(Qt::red);
        else
            horizontal_header[i]->SetTextColor(Qt::black);
    }
    if (Setting::ShowWeekNumber)
    {
        corner_label->setFixedWidth(40);
        corner_label->setStyleSheet(QString("background:%1;").arg(Setting::CellColor.darker(140).name()));
        for (int i = 0; i < Const::MONTH_WEEKS; i++)
        {
            vertical_header[i]->show();
            vertical_header[i]->SetBackgroundColor(Setting::CellColor.darker(140));
            vertical_header[i]->SetTextColor(Qt::black);
        }
    }
    else
    {
        corner_label->setFixedWidth(0);
        for (int i = 0; i < Const::MONTH_WEEKS; i++) vertical_header[i]->hide();
    }

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
        if (showMessageBox) QMessageBox::critical(0, tr("Fail to Import Data"), QString(tr("Cannot import data from \"%1\"!")).arg(fileName));
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
        if (showMessageBox) QMessageBox::critical(0, tr("Fail to Export Data"), QString(tr("Cannot export data to \"%1\"!")).arg(fileName));
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
    if (!Setting::Movable) return;
    dayWidgetByAction = static_cast<DayWidget*>(QObject::sender());
    if (!dayWidgetByAction->rect().contains(pos)) return;

    eventByAction = nullptr;
    dateByAction = dayWidgetByAction->Date();

    action_add_event->setText(tr("&Add Event..."));

    ColorMenu color_menu(tr("Backgruond &Color"), this);
    color_menu.SetDefaultColor(dayWidgetByAction->ThemeColor());

    QMenu menu(this);
    menu.addAction(action_add_event);
    menu.addSeparator();
    menu.addMenu(&color_menu);

    menu.exec(QCursor::pos());
    if (color_menu.ColorSelected())
    {
        day_color[dateByAction] = color_menu.SelectedColor();
        loadTable();
    }
}

void MainWindow::onEventLabelContextMenu(const QPoint& pos)
{
    if (!Setting::Movable) return;
    EventLabelButton* label = static_cast<EventLabelButton*>(QObject::sender());
    AbstractEvent* event = label->Event();
    QPoint mousePos = label->pos() + pos;
    if (!label->rect().contains(pos)) return;

    eventLabelByAction = label;
    eventByAction = event;

    ColorMenu color_menu(tr("Backgruond &Color"), this);
    color_menu.SetDefaultColor(label->BackgroundColor());

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
        action_delete_event->setText(tr("&Remove Event"));
        menu.addAction(action_delete_event);
        menu.addSeparator();
        menu.addMenu(&color_menu);
    }
    else if (event->Type() == AbstractEvent::RecurrentEvent)
    {
        action_delete_event->setText(tr("Remove the Whole Event &Sequence"));
        action_delete_one_event->setText(tr("Remove &Single Event"));
        menu.addAction(action_delete_event);
        menu.addAction(action_delete_one_event);
        menu.addSeparator();
        menu.addMenu(&color_menu);
    }

    menu.exec(QCursor::pos());

    if (color_menu.ColorSelected())
    {
        eventByAction->SetLabelColor(color_menu.SelectedColor());
        loadTable();
    }
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
    if (QMessageBox::question(this, tr("Remove Event"), QString(tr("Are you sure you want to remove the event \"%1\" and it's all attachments?")).arg(eventByAction->Title())) != QMessageBox::Yes)
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
    event->SetTitle(QString(tr("File \"%2\"")).arg(QFileInfo(filePath).fileName()));
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

void MainWindow::on_action_movable_triggered(bool checked)
{
    Setting::Movable = checked;
    ui->action_movable->setToolTip(checked ? tr("Fix") : tr("Move"));
}

void MainWindow::on_action_import_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import Data File"),
                                                          QDir::currentPath() + "/" + Const::DEFAULT_DATA_FILE,
                                                          tr("Calendar Data File (*.cdat)"));
    if (!fileName.isEmpty()) importData(fileName, true);
}

void MainWindow::on_action_export_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export Data File"),
                                                          QDir::homePath(),
                                                          tr("Calendar Data File (*.cdat)"));
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
    msgBox.setWindowTitle(tr("About Calendar"));
    msgBox.setText(QString(tr(
                      "<h2>Calendar Program<br/></h2>"
                      "<p>Programing and Training Project 1</p>"
                      "<p>Based on Qt 5.7.0<br/></p>"
                      "<p>Version: %1</p>"
                      "<p>Built time: %2 - %3<br/></p>"
                      "<p>Copyright © 2016 Yuekai Jia, CST, Tsinghua University</p>"
                      "<p>All Right Reserved.<br/></p>"
                      "<p>Project Index: <a href=\"https://github.com/equation314/Calendar\">https://github.com/equation314/Calendar</a></p>"
                      )).arg("1.0.0").arg(__DATE__).arg(__TIME__));
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
