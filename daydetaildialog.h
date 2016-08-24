#ifndef DAYDETAILDIALOG_H
#define DAYDETAILDIALOG_H

#include "daywidget.h"
#include "mainwindow.h"

#include <QDialog>

namespace Ui
{
class DayDetailDialog;
}

class DayDetailDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DayDetailDialog(DayWidget* date, QWidget *parent = 0);
    ~DayDetailDialog();

public slots:
    void loadLabels();

private:
    Ui::DayDetailDialog *ui;
    DayWidget* date;
    std::vector<LabelButton*> label_list;
    MainWindow* parent;
    QSpacerItem* spacer;
};

#endif // DAYEVENTDIALOG_H
