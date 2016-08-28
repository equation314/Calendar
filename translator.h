#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QLocale>
#include <QTranslator>

class Translator
{
    Translator() {}
public:
    enum Language
    {
        SimplifiedChinese,
        TraditionalChinese,
        English,
        Invalid
    };

    static QLocale Locale(Language lang) { return LANGUAGE_LOCALE[(int)lang]; }

    static void InstallToApplication(Language lang);

private:
    static const QLocale LANGUAGE_LOCALE[3];

    static Language language;
    static std::vector<QTranslator*> trans_list;

};

#endif // TRANSLATOR_H
