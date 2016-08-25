#ifndef FILELISTWIDGET_H
#define FILELISTWIDGET_H

#include <QUrl>
#include <QListWidget>

class FileListWidget : public QListWidget
{
    Q_OBJECT
public:
    FileListWidget(QWidget* parent = nullptr);

    void AddFile(const QString &filePath);

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;

private:
    std::vector<QUrl> file_url_list;
    QMenu* menu;
    QAction *action_delete_file, *action_open_file;
    int rowByAction;

private slots:
    void onContextMenuEvent(const QPoint &pos);
    void onOpenFile();
    void onDeleteFile();

signals:
    void fileRemoved(const QString fileName);
};

#endif // FILELISTWIDGET_H
