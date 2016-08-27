#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QLocale>
#include <QTranslator>

class Translator : public QTranslator
{
public:
    enum Language
    {
        SimplifiedChinese,
        TraditionalChinese,
        English
    };

    Translator() {}

    static QLocale Locale(Language lang) { return LANGUAGE_LOCALE[(int)lang]; }

    void InstallToApplication(Language lang);

private:
    Language language;
    std::vector<QTranslator*> trans_list;

    static const QLocale LANGUAGE_LOCALE[3];

};

#endif // TRANSLATOR_H
