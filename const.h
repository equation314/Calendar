#ifndef CONST_H
#define CONST_H

#include <QDate>
#include <QColor>
#include <QLocale>

namespace Const
{
const int WEEK_DAYS = 7;
const int MONTH_WEEKS = 6;
const QDate MAX_DATE(7999, 12, 31);
const QString DEFAULT_DATA_FILE = "user.cdat";
const QString SETTING_FILE = "setting.xml";
const QList<QColor> COLOR_LIST =
{
    QColor(255, 255, 255),
    QColor(170, 170, 170),
    QColor(120, 120, 120),
    QColor(185, 122, 87),
    QColor(136, 0, 21),
    QColor(242, 66, 74),
    QColor(255, 128, 169),
    QColor(255, 127, 39),
    QColor(255, 221, 7),
    QColor(234, 219, 149),
    QColor(181, 230, 29),
    QColor(34, 177, 76),
    QColor(132, 210, 230),
    QColor(0, 162, 232),
    QColor(47, 101, 188),
    QColor(112, 146, 190),
    QColor(163, 73, 164),
    QColor(91, 39, 131)
};

inline bool IsWeekend(int day)
{
    return day % WEEK_DAYS == 0 || day % WEEK_DAYS == 6;
}

}

#endif // CONST_H
