#ifndef CONTINUOUSEVENT_H
#define CONTINUOUSEVENT_H

#include "abstractevent.h"

class ContinuousEvent : public AbstractEvent
{
public:
    ContinuousEvent(const QDate& begin, const QDate& end);
    ~ContinuousEvent() {}

    virtual EventType Type() const override { return EventType::ContinuousEvent; }
    virtual bool InList(const QDate& date) const override;

private:
};

#endif // CONTINUOUSEVENT_H
