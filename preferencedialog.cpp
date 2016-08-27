#include "const.h"
#include "setting.h"
#include "preferencedialog.h"
#include "ui_preferencedialog.h"

#include <QDate>
#include <QPainter>
#include <QPushButton>
#include <QFileDialog>
#include <QFontDialog>

static QPushButton* buttonByAction;

PreferenceDialog::PreferenceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferenceDialog),
    color_menu(new ColorMenu("", this))
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

    for (int i = 1; i <= 7; i++)
        ui->comboBox_weekFirstDay->addItem(Translator::Locale(Setting::Language).dayName(i, QLocale::LongFormat));

    loadSetting();
}

PreferenceDialog::~PreferenceDialog()
{
    delete ui;
}

void PreferenceDialog::setButtonFont(QPushButton* button, const QFont& font)
{
    button_font[button] = font;

    QFontMetrics fm(font);
    QSize size = fm.size(Qt::TextSingleLine, font.family());
    if (size.height() < 24) size.setHeight(24);
    if (size.width() < 64) size.setWidth(64);
    int offset = size.height() / 4;

    button->setFixedSize(size + QSize(offset, offset));
    button->setIconSize(size);
    QPixmap pixmap(button->iconSize());
    pixmap.fill(Qt::white);
    QPainter painter(&pixmap);
    painter.setFont(font);
    painter.drawText(QRectF(QPoint(0, 0), pixmap.size()), Qt::AlignCenter, font.family());
    button->setIcon(pixmap);
}

void PreferenceDialog::setButtonColor(QPushButton* button, const QColor& color)
{
    button_color[button] = color;

    QFontMetrics fm(Setting::InterfaceFont);
    QSize size = fm.size(Qt::TextSingleLine, Setting::InterfaceFont.family());
    if (size.height() < 24) size.setHeight(24);
    if (size.width() < 64) size.setWidth(64);
    int offset = size.height() / 4;

    button->setFixedSize(size);
    button->setIconSize(size - QSize(offset, offset));
    QPixmap pixmap(button->iconSize());
    pixmap.fill(color);
    button->setIcon(pixmap);
}

void PreferenceDialog::loadSetting()
{
    ui->checkBox_dragDrop->setChecked(Setting::EnableDragsAndDrops);
    ui->checkBox_showWeek->setChecked(Setting::ShowWeekNumber);
    ui->comboBox_weekFirstDay->setCurrentIndex(Setting::WeekFirstDay - 1);
    setButtonFont(ui->pushButton_interfaceFont, Setting::InterfaceFont);
    ui->comboBox_language->setCurrentIndex(Setting::Language);
    ui->comboBox_opacity->setCurrentIndex(10 - Setting::Opacity);

    ui->comboBox_cellSpace->setCurrentIndex(Setting::CellSpace);
    setButtonColor(ui->pushButton_cellColor, Setting::CellColor);

    setButtonFont(ui->pushButton_eventFont, Setting::EventFont);
    setButtonColor(ui->pushButton_eventColor_1, Setting::ContinuousEventColor);
    setButtonColor(ui->pushButton_eventColor_2, Setting::RecurrentEventColor);
}

void PreferenceDialog::accept()
{
    Setting::EnableDragsAndDrops = ui->checkBox_dragDrop->isChecked();
    Setting::ShowWeekNumber = ui->checkBox_showWeek->isChecked();
    Setting::WeekFirstDay = ui->comboBox_weekFirstDay->currentIndex() + 1;
    Setting::Language = (Translator::Language)ui->comboBox_language->currentIndex();
    Setting::Opacity = 10 - ui->comboBox_opacity->currentIndex();
    Setting::CellSpace = ui->comboBox_cellSpace->currentIndex();

    Setting::InterfaceFont = button_font[ui->pushButton_interfaceFont];
    Setting::EventFont = button_font[ui->pushButton_eventFont];
    Setting::CellColor = button_color[ui->pushButton_cellColor];
    Setting::ContinuousEventColor = button_color[ui->pushButton_eventColor_1];
    Setting::RecurrentEventColor = button_color[ui->pushButton_eventColor_2];

    QDialog::accept();
}



void PreferenceDialog::on_pushButton_import_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import Configuration File"),
                                                          QDir::currentPath() + "/" + Const::SETTING_FILE,
                                                          tr("Configuration File (*.xml)"));
    if (!fileName.isEmpty()) Setting::LoadSetting(fileName);
    loadSetting();
}

void PreferenceDialog::on_pushButton_export_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export Configuration File"),
                                                          QDir::homePath() + "/" + Const::SETTING_FILE,
                                                          tr("Configuration File (*.xml)"));
    if (!fileName.isEmpty()) Setting::SaveSetting(fileName);
}

void PreferenceDialog::on_pushButton_interfaceFont_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, button_font[ui->pushButton_interfaceFont], this);
    if (ok) setButtonFont(ui->pushButton_interfaceFont, font);
}

void PreferenceDialog::on_pushButton_eventFont_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, button_font[ui->pushButton_eventFont], this);
    if (ok) setButtonFont(ui->pushButton_eventFont, font);
}

void PreferenceDialog::on_pushButton_cellColor_clicked()
{
    color_menu->SetDefaultColor(button_color[buttonByAction = ui->pushButton_cellColor]);
    QPoint pos = ui->groupBox_3->mapToGlobal(ui->pushButton_cellColor->geometry().bottomLeft());
    color_menu->exec(pos);
    if (color_menu->ColorSelected())
        setButtonColor(ui->pushButton_cellColor, color_menu->SelectedColor());
}

void PreferenceDialog::on_pushButton_eventColor_1_clicked()
{
    color_menu->SetDefaultColor(button_color[buttonByAction = ui->pushButton_eventColor_1]);
    QPoint pos = ui->groupBox_4->mapToGlobal(ui->pushButton_eventColor_1->geometry().bottomLeft());
    color_menu->exec(pos);
    if (color_menu->ColorSelected())
        setButtonColor(ui->pushButton_eventColor_1, color_menu->SelectedColor());
}

void PreferenceDialog::on_pushButton_eventColor_2_clicked()
{
    color_menu->SetDefaultColor(button_color[buttonByAction = ui->pushButton_eventColor_2]);
    QPoint pos = ui->groupBox_4->mapToGlobal(ui->pushButton_eventColor_2->geometry().bottomLeft());
    color_menu->exec(pos);
    if (color_menu->ColorSelected())
        setButtonColor(ui->pushButton_eventColor_2, color_menu->SelectedColor());
}

void PreferenceDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if (button == ui->buttonBox->buttons().last())
    {
        ui->checkBox_dragDrop->setChecked(true);
        ui->checkBox_showWeek->setChecked(true);
        ui->comboBox_weekFirstDay->setCurrentIndex(0);
        ui->comboBox_language->setCurrentIndex(0);
        ui->comboBox_opacity->setCurrentIndex(0);
        setButtonFont(ui->pushButton_interfaceFont, QFont("微软雅黑", 9));

        ui->comboBox_cellSpace->setCurrentIndex(2);
        setButtonColor(ui->pushButton_cellColor, Const::COLOR_LIST[14]);

        setButtonColor(ui->pushButton_eventColor_1, Const::COLOR_LIST[11]);
        setButtonColor(ui->pushButton_eventColor_2, Const::COLOR_LIST[10]);
        setButtonFont(ui->pushButton_eventFont, QFont("微软雅黑", 9));
    }
}
