#ifndef SETTING_H
#define SETTING_H

#include <QFont>
#include <QColor>

class Setting
{
    Setting();

public:
    static bool EnableDragsAndDrops;
    static bool ShowWeekNumber;
    static int WeekFirstDay;
    static int Language;
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
