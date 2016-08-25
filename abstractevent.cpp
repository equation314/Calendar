#include "abstractevent.h"

#include <QDir>
#include <QMessageBox>

AbstractEvent::AbstractEvent(const QDate& begin, const QDate& end) :
    begin(begin), end(end), color(QColor(34, 177, 76))
{

}

AbstractEvent::~AbstractEvent()
{

}

void AbstractEvent::RemoveFile(const QString& fileName)
{
    QFile(QDir::currentPath() + "/data/" + dir_name + "/" + fileName).remove();
    for (auto i = file_list.begin(); i != file_list.end(); i++)
        if (QFileInfo(*i).fileName() == fileName)
        {
            file_list.erase(i);
            break;
        }
}

void AbstractEvent::RemoveAllFiles()
{
    QDir(QDir::currentPath() + "/data/" + dir_name).removeRecursively();
    file_list.clear();
}

void AbstractEvent::AddFile(const QString &filePath)
{
    QFile file(filePath);
    if (dir_name.isEmpty()) dir_name = QString("event_%1%2").arg(qrand()).arg(qrand());
    if (!QDir::current().exists("data/" + dir_name)) QDir::current().mkpath("data/" + dir_name);

    QString newFile = QString("%1/%2/%3/%4").arg(QDir::currentPath()).arg("data").arg(dir_name).arg(QFileInfo(file).fileName());
    if (QFile(newFile).exists())
    {
        QMessageBox::critical(0, "导入文件失败", QString("文件 \"%1\" 已在该事件中！").arg(QFileInfo(file).fileName()));
        return;
    }
    if (file.copy(newFile))
    {
        file_list.push_back(newFile);
        QMessageBox::information(0, "导入文件成功", QString("文件 \"%1\" 导入成功。").arg(QFileInfo(file).fileName()));
    }
}
