#include "abstractevent.h"
#include "recurrentevent.h"
#include "continuousevent.h"

#include <QDir>
#include <QMessageBox>

AbstractEvent::AbstractEvent(const QDate& begin, const QDate& end) :
    begin(begin), end(end)
{

}

AbstractEvent::~AbstractEvent()
{

}

QString AbstractEvent::FilePathAt(int i) const
{
    return QDir::currentPath() + "/data/" + dir_name + "/" + file_name_list[i];
}

void AbstractEvent::RemoveFile(const QString& fileName)
{
    QFile(QDir::currentPath() + "/data/" + dir_name + "/" + fileName).remove();
    for (auto i = file_name_list.begin(); i != file_name_list.end(); i++)
        if (QFileInfo(*i).fileName() == fileName)
        {
            file_name_list.erase(i);
            break;
        }
}

void AbstractEvent::RemoveAllFiles()
{
    QDir(QDir::currentPath() + "/data/" + dir_name).removeRecursively();
    file_name_list.clear();
}

void AbstractEvent::Clone(AbstractEvent *event)
{
    begin = event->begin;
    end = event->end;
    title = event->title;
    place = event->place;
    detail = event->detail;
    dir_name = event->dir_name;
    color = event->color;
    file_name_list = event->file_name_list;
}

bool AbstractEvent::AddFile(const QString &filePath)
{
    if (dir_name.isEmpty()) dir_name = QString("event_%1%2").arg(qrand()).arg(qrand());
    if (!QDir::current().exists("data/" + dir_name)) QDir::current().mkpath("data/" + dir_name);

    QString fileName = QFileInfo(filePath).fileName();
    QString newFilePath = QString("%1/%2/%3/%4").arg(QDir::currentPath()).arg("data").arg(dir_name).arg(fileName);
    if (QFile(newFilePath).exists())
    {
        QMessageBox::critical(0, tr("Import File Failed"), QString(tr("The file \"%1\" is already in this event!")).arg(fileName));
        return false;
    }
    if (QFile::copy(filePath, newFilePath))
    {
        file_name_list.push_back(fileName);
        QMessageBox::information(0, tr("File Imported Successfully"), QString(tr("Successfully imported the file \"%1\".")).arg(fileName));
    }
    return true;
}

QDataStream& operator <<(QDataStream& dataStream, AbstractEvent* event)
{
    dataStream << (int)event->Type();
    dataStream << event->begin << event->end;
    dataStream << event->title << event->place << event->detail << event->dir_name;
    dataStream << event->color;
    dataStream << event->file_name_list;
    event->save(dataStream);
    return dataStream;
}

QDataStream& operator >>(QDataStream& dataStream, AbstractEvent** event)
{
    int type;
    QDate begin, end;
    AbstractEvent* aevent;
    dataStream >> type >> begin >> end;
    if ((AbstractEvent::EventType)type == AbstractEvent::ContinuousEvent)
        aevent = new ContinuousEvent(begin, end);
    else if ((AbstractEvent::EventType)type == AbstractEvent::RecurrentEvent)
        aevent = new RecurrentEvent(begin, end);
    else
        return dataStream;

    dataStream >> aevent->title >> aevent->place >> aevent->detail >> aevent->dir_name;
    dataStream >> aevent->color;
    dataStream >> aevent->file_name_list;
    aevent->load(dataStream);
    *event = aevent;
    return dataStream;
}
