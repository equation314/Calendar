#include "setting.h"
#include "filelistwidget.h"

#include <QDrag>
#include <QMenu>
#include <QAction>
#include <QMimeData>
#include <QMouseEvent>
#include <QFileIconProvider>
#include <QDesktopServices>
#include <QCoreApplication>

static int rowByAction;

FileListWidget::FileListWidget(QWidget* parent) :
    QListWidget(parent)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setTextElideMode(Qt::ElideLeft);
    this->setResizeMode(QListView::Adjust);
    this->setIconSize(QSize(32, 32));
    this->setDragEnabled(true);
    this->hide();

    action_open_file = new QAction(tr("&Open File..."), this);
    action_delete_file = new QAction(tr("&Remove File"), this);

    connect(action_open_file, &QAction::triggered, this, &FileListWidget::onOpenFile);
    connect(action_delete_file, &QAction::triggered, this, &FileListWidget::onDeleteFile);

    connect(this, &QWidget::customContextMenuRequested, this, &FileListWidget::onContextMenuEvent);
    connect(this, &QListWidget::doubleClicked, this, [this](const QModelIndex& index)
    {
        rowByAction = index.row();
        onOpenFile();
    });
}

void FileListWidget::AddFile(const QString& filePath)
{
    QFileInfo info(filePath);
    QFileIconProvider iconProvider;
    QIcon icon = iconProvider.icon(info);
    QListWidgetItem* item = new QListWidgetItem(icon, info.fileName());
    this->addItem(item);
    file_url_list.push_back(QUrl::fromLocalFile(filePath));
    this->show();
}

void FileListWidget::startDrag(Qt::DropActions supportedActions)
{
    if (!Setting::EnableDragsAndDrops)
    {
        QListWidget::startDrag(supportedActions);
        return;
    }
    Setting::EnableDragsAndDrops = false;

    QDrag* drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;
    QFileInfo info(file_url_list[this->currentRow()].toLocalFile());
    QFileIconProvider iconProvider;

    mimeData->setUrls({file_url_list[this->currentRow()]});
    drag->setMimeData(mimeData);
    drag->setPixmap(iconProvider.icon(info).pixmap(this->iconSize()));
    drag->exec(Qt::CopyAction);

    Setting::EnableDragsAndDrops = true;
}

void FileListWidget::onContextMenuEvent(const QPoint& pos)
{
    QListWidgetItem* item = this->itemAt(pos);
    if (item)
    {
        rowByAction = this->row(item);
        QMenu menu(this);
        menu.addAction(action_open_file);
        menu.addAction(action_delete_file);
        menu.exec(QCursor::pos());
    }
}

void FileListWidget::onOpenFile()
{
    QDesktopServices::openUrl(file_url_list[rowByAction]);
}

void FileListWidget::onDeleteFile()
{
    QString filePath = file_url_list[rowByAction].toLocalFile();
    delete this->takeItem(rowByAction);
    file_url_list.erase(file_url_list.begin() + rowByAction);
    if (!this->count()) this->hide();
    emit fileRemoved(QFileInfo(filePath).fileName());
}
