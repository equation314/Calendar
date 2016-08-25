#include "filelistwidget.h"

#include <QDrag>
#include <QMenu>
#include <QDebug>
#include <QAction>
#include <QMimeData>
#include <QMouseEvent>
#include <QFileIconProvider>
#include <QDesktopServices>
#include <QCoreApplication>

FileListWidget::FileListWidget(QWidget* parent) :
    QListWidget(parent)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setIconSize(QSize(64, 64));
    this->setTextElideMode(Qt::ElideLeft);
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setResizeMode(QListView::Adjust);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->hide();

    menu = new QMenu(this);
    action_open_file = new QAction("打开文件(&O)", this);
    action_delete_file = new QAction("删除文件(&D)", this);
    menu->addAction(action_open_file);
    menu->addAction(action_delete_file);

    connect(action_open_file, &QAction::triggered, this, &FileListWidget::onOpenFile);
    connect(action_delete_file, &QAction::triggered, this, &FileListWidget::onDeleteFile);

    connect(this, &QWidget::customContextMenuRequested, this, &FileListWidget::onContextMenuEvent);
    connect(this, &QListWidget::doubleClicked, this, [this](const QModelIndex& index)
    {
        rowByAction = index.row();
        onOpenFile();
    });
}

void FileListWidget::AddFile(const QString& file)
{
    QFileInfo info(file);
    QFileIconProvider iconProvider;
    QIcon icon = iconProvider.icon(info);
    QListWidgetItem* item = new QListWidgetItem(QIcon(icon.pixmap(64,64)), info.fileName());
    this->addItem(item);
    file_url_list.push_back(QUrl::fromLocalFile(file));
    this->show();
}

void FileListWidget::mousePressEvent(QMouseEvent *event)
{
    QListWidgetItem* item = this->itemAt(event->pos());
    if (item && event->button() == Qt::LeftButton)
    {
        QDrag* drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;

        mimeData->setUrls({file_url_list[this->row(item)]});
        drag->setMimeData(mimeData);
        drag->exec(Qt::CopyAction | Qt::MoveAction);
    }
    QListWidget::mousePressEvent(event);
}



void FileListWidget::onContextMenuEvent(const QPoint& pos)
{
    QListWidgetItem* item = this->itemAt(pos);
    if (item)
    {
        rowByAction = this->row(item);
        menu->popup(QCursor::pos());
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
    qDebug()<<filePath<<' '<<rowByAction<<' '<<file_url_list.size();
    emit fileRemoved(QFileInfo(filePath).fileName());
}
