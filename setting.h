#ifndef SETTING_H
#define SETTING_H

#include "translator.h"

#include <QFont>
#include <QColor>

class Setting
{
    Setting();

public:
    static bool Movable;
    static bool EnableDragsAndDrops;
    static bool ShowWeekNumber;
    static int WeekFirstDay;
    static Translator::Language Language;
    static int Opacity;
    static int CellSpace;
    static QFont InterfaceFont;
    static QFont EventFont;
    static QColor CellColor;
    static QColor ContinuousEventColor;
    static QColor RecurrentEventColor;

    static void LoadSetting(const QString& fileName);
    static void SaveSetting(const QString& fileName);
};

#endif // SETTING_H
