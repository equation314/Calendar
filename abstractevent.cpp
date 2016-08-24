#include "abstractevent.h"

#include <QDebug>

AbstractEvent::AbstractEvent(const QDate& begin, const QDate& end) :
    begin(begin), end(end), color(QColor(34, 177, 76))
{

}

AbstractEvent::~AbstractEvent()
{

}
