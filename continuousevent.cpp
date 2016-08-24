#include "continuousevent.h"

ContinuousEvent::ContinuousEvent(const QDate& begin, const QDate& end) :
    AbstractEvent(begin, end)
{

}

bool ContinuousEvent::InList(const QDate& date) const
{
    return begin <= date && date <= end;
}
