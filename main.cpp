#include "mainwindow.h"
#include "translator.h"
#include "logindialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    qsrand(QTime::currentTime().msecsSinceStartOfDay());

    QApplication a(argc, argv);

    Translator::InstallToApplication(Translator::SimplifiedChinese);
    LoginDialog dialog;
    if (dialog.exec() != QDialog::Accepted) return 0;

    MainWindow w(dialog.Username());
    w.show();

    return a.exec();
}
