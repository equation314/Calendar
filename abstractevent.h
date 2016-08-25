#ifndef ABSTRACTEVENT_H
#define ABSTRACTEVENT_H

#include <QDate>
#include <QLabel>
#include <QObject>

class AbstractEvent : public QObject
{
    Q_OBJECT
public:
    enum EventType { ContinuousEvent, RecurrentEvent };

    explicit AbstractEvent();
    explicit AbstractEvent(const QDate& begin, const QDate& end);
    virtual ~AbstractEvent();

    QDate Begin() const { return begin; }
    QDate End() const { return end; }
    QString Title() const { return title; }
    QString Place() const { return place; }
    QString Detail() const { return detail; }
    QColor LabelColor() const { return color; }
    int FileCount() const { return file_name_list.size(); }
    QString FilePathAt(int i) const;

    void ResetBeginEnd(const QDate& begin, const QDate& end)
    {
        this->begin = begin;
        this->end = end;
    }
    void SetTitle(const QString& text) { title = text; }
    void SetPlace(const QString& text) { place = text; }
    void SetDetail(const QString& text) { detail = text; }
    void SetLabelColor(const QColor& col) { color = col; }

    void AddFile(const QString& filePath);
    void RemoveFile(const QString &fileName);
    void RemoveAllFiles();
    void Clone(AbstractEvent* event);

    virtual EventType Type() const = 0;
    virtual bool InList(const QDate& date) const = 0;

    friend QDataStream& operator <<(QDataStream& dataStream, AbstractEvent* event);
    friend QDataStream& operator >>(QDataStream& dataStream, AbstractEvent** event);

protected:
    QDate begin, end;
    QString title, place, detail, dir_name;
    QColor color;
    QStringList file_name_list;

    virtual void save(QDataStream &dataStream) const = 0;
    virtual void load(QDataStream &dataStream) = 0;
};

#endif // ABSTRACTEVENT_H
