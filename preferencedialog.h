#ifndef PREFERENCEDIALOG_H
#define PREFERENCEDIALOG_H

#include "colormenu.h"

#include <QDialog>
#include <QAbstractButton>

namespace Ui
{
class PreferenceDialog;
}

class PreferenceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferenceDialog(QWidget *parent = 0);
    ~PreferenceDialog();

protected:
    void accept() override;

private slots:
    void on_pushButton_import_clicked();
    void on_pushButton_export_clicked();

    void on_pushButton_interfaceFont_clicked();
    void on_pushButton_eventFont_clicked();

    void on_pushButton_cellColor_clicked();
    void on_pushButton_eventColor_1_clicked();
    void on_pushButton_eventColor_2_clicked();

    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::PreferenceDialog *ui;
    ColorMenu* color_menu;
    int clicked_button_id;
    std::map<QPushButton*, QColor> button_color;
    std::map<QPushButton*, QFont> button_font;

    void setButtonFont(QPushButton *button, const QFont &font);
    void setButtonColor(QPushButton* button, const QColor &color);
    void loadSetting();
};

#endif // PREFERENCEDIALOG_H
