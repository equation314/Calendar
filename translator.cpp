#include "const.h"
#include "translator.h"

#include <QApplication>

const QLocale Translator::LANGUAGE_LOCALE[3] =
{
   QLocale(QLocale::Chinese, QLocale::SimplifiedChineseScript, QLocale::China),
   QLocale(QLocale::Chinese, QLocale::TraditionalChineseScript, QLocale::HongKong),
   QLocale(QLocale::English)
};

Translator::Language Translator::language = Translator::Invalid;
std::vector<QTranslator*> Translator::trans_list;

void Translator::InstallToApplication(Language lang)
{
    if (lang == language) return;
    QLocale::setDefault(LANGUAGE_LOCALE[language = lang]);

    for (auto i : trans_list)
    {
        qApp->removeTranslator(i);
        i->deleteLater();
    }
    trans_list.clear();

    QTranslator* trans;

    switch (lang)
    {
    case SimplifiedChinese:
        trans = new QTranslator;
        trans->load(":/trans/trans/zh_CN.qm");
        trans_list.push_back(trans);
        qApp->installTranslator(trans);

        trans = new QTranslator;
        trans->load(":/trans/trans/qt_zh_CN.qm");
        trans_list.push_back(trans);
        qApp->installTranslator(trans);
        break;
    case TraditionalChinese:
        trans = new QTranslator;
        trans->load(":/trans/trans/zh_TW.qm");
        trans_list.push_back(trans);
        qApp->installTranslator(trans);

        trans = new QTranslator;
        trans->load(":/trans/trans/qt_zh_TW.qm");
        trans_list.push_back(trans);
        qApp->installTranslator(trans);
        break;
    }
}
