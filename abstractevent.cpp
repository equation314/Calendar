#include "abstractevent.h"
#include "recurrentevent.h"
#include "continuousevent.h"

#include <QDir>
#include <QDebug>
#include <QMessageBox>

AbstractEvent::AbstractEvent() :
    color(QColor(34, 177, 76))
{

}


AbstractEvent::AbstractEvent(const QDate& begin, const QDate& end) :
    begin(begin), end(end), color(QColor(34, 177, 76))
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

void AbstractEvent::AddFile(const QString &filePath)
{
    if (dir_name.isEmpty()) dir_name = QString("event_%1%2").arg(qrand()).arg(qrand());
    if (!QDir::current().exists("data/" + dir_name)) QDir::current().mkpath("data/" + dir_name);

    QString fileName = QFileInfo(filePath).fileName();
    QString newFilePath = QString("%1/%2/%3/%4").arg(QDir::currentPath()).arg("data").arg(dir_name).arg(fileName);
    if (QFile(newFilePath).exists())
    {
        QMessageBox::critical(0, "导入文件失败", QString("文件 \"%1\" 已在该事件中！").arg(fileName));
        return;
    }
    if (QFile::copy(filePath, newFilePath))
    {
        file_name_list.push_back(fileName);
        QMessageBox::information(0, "导入文件成功", QString("文件 \"%1\" 导入成功。").arg(fileName));
    }
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
    AbstractEvent* aevent;
    dataStream >> type;
    if ((AbstractEvent::EventType)type == AbstractEvent::ContinuousEvent)
        aevent = new ContinuousEvent;
    else if ((AbstractEvent::EventType)type == AbstractEvent::RecurrentEvent)
        aevent = new RecurrentEvent;
    else
        return dataStream;

    dataStream >> aevent->begin >> aevent->end;
    dataStream >> aevent->title >> aevent->place >> aevent->detail >> aevent->dir_name;
    dataStream >> aevent->color;
    dataStream >> aevent->file_name_list;
    aevent->load(dataStream);
    *event = aevent;
    return dataStream;
}
