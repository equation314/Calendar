#ifndef FILELISTWIDGET_H
#define FILELISTWIDGET_H

#include <QUrl>
#include <QListWidget>

class FileListWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit FileListWidget(QWidget* parent = nullptr);

    void AddFile(const QString &filePath);

protected:
    void startDrag(Qt::DropActions supportedActions) override;

private:
    std::vector<QUrl> file_url_list;
    QAction *action_delete_file, *action_open_file;

private slots:
    void onContextMenuEvent(const QPoint &pos);
    void onOpenFile();
    void onDeleteFile();

signals:
    void fileRemoved(const QString fileName);
};

#endif // FILELISTWIDGET_H
