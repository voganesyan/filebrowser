#include "directoryscanner.h"
#include <QStandardItemModel>

#include <QLineEdit>
#include <QTreeView>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QDir>

#include <QDebug>
#include <QPushButton>
#include <QLabel>
#include <QHeaderView>
#include <iostream>
#include "filecleaner.h"

using namespace std;
DirectoryScanner::DirectoryScanner(QWidget *parent) :
    QWidget(parent)
{
    QStringList labels;
    labels << "File"  << "Size";
    model = new QStandardItemModel();
    model->setHorizontalHeaderLabels (labels);

    list = new QTreeView();

    lineeditPath = new QLineEdit;
    lineeditFilter = new QLineEdit;
    connect(lineeditPath, SIGNAL(returnPressed()),
                     this,   SLOT(on_lineedit_returnPressed()));
    connect(lineeditFilter, SIGNAL(returnPressed()),
                     this,   SLOT(on_lineedit_returnPressed()));

    statusbar = new QStatusBar();
    clearButton = new QPushButton("Clear");
    QObject::connect(clearButton, &QPushButton::clicked,
                         this, &DirectoryScanner::on_clearButton_clicked);
    statusbar->addPermanentWidget(clearButton);

    QHBoxLayout* hbox = new QHBoxLayout();
    hbox->addWidget(new QLabel("Directory"));
    hbox->addWidget(lineeditPath);
    hbox->addWidget(new QLabel("Filter"));
    hbox->addWidget(lineeditFilter);

    QVBoxLayout* layout = new QVBoxLayout();

    layout->addLayout(hbox);
    layout->addWidget(list);
    layout->addWidget(statusbar);
    setLayout( layout );
}



void DirectoryScanner::scanDirectory(const QString& path)
{
   lineeditPath->setText(path);
   emit lineeditPath->returnPressed();
}


void DirectoryScanner::on_lineedit_returnPressed()
{
    QString path = lineeditPath->text();
    QString extension = lineeditFilter->text();
    //!!! validate path
    vector<FC_FileInfo> files;

    FC_GetFilesInDirectory(files, path.toStdString(), extension.toStdString());

    list->setModel (0);
    //warning: on_tree_selectionChanged() has disconnected!
    //         do not forget to reconnect it

    model->removeRows(0, model->rowCount());

    for (vector<FC_FileInfo>::iterator it = files.begin() ; it != files.end(); ++it)
    {
        QList<QStandardItem *> items;
        FC_FileInfo info = *it;
        items << new QStandardItem (QString::fromStdString(info.name))
              << new QStandardItem (size2string(info.size));

        model->appendRow (items);
    }

    list->setModel (model);
    connect(list->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &DirectoryScanner::on_tree_selectionChanged);

}

void DirectoryScanner::on_tree_selectionChanged(const QItemSelection & /*newSelection*/, const QItemSelection & /*oldSelection*/)
{
/*    QItemSelectionModel *selectionModel = qobject_cast<QItemSelectionModel *>(QObject::sender());
    const QModelIndex index = selectionModel->currentIndex();

    QModelIndex sibling = index.sibling(index.row(),0);
    QStandardItem *item=model->itemFromIndex(sibling);

    if (item)
      clearingFilePath = QDir(lineeditPath->text()).filePath(item->text());
    else
      clearingFilePath.clear();

    statusbar->showMessage(clearingFilePath);
    //qDebug() << "DDDD" << item->text();
*/
}

void DirectoryScanner::on_clearButton_clicked()
{
    const QModelIndex index = list->selectionModel()->currentIndex();

    QModelIndex sibling = index.sibling(index.row(),0);
    QStandardItem *item_name=model->itemFromIndex(sibling);


   if (item_name == NULL) return;

   QString clearingFilePath = QDir(lineeditPath->text()).filePath(item_name->text());



   sibling = index.sibling(index.row(),1);
   QStandardItem *item_size=model->itemFromIndex(sibling);

   string error_message;
   if (FC_ClearFile(clearingFilePath.toStdString(), error_message))
     item_size->setText(size2string(0));
   else
     statusbar->showMessage(QString::fromStdString(error_message));
}

#include <QFileSystemModel>
QString DirectoryScanner::size2string (quint64 bytes)
{
    // According to the Si standard KB is 1000 bytes, KiB is 1024
    // but on windows sizes are calulated by dividing by 1024 so we do what they do.
    const float kb = 1024;
    const float mb = 1024 * kb;
    const float gb = 1024 * mb;
    const float tb = 1024 * gb;

    if (bytes >= tb)
        return QFileSystemModel::tr("%1 TB").arg(QLocale().toString(bytes / tb, 'f', 3));
    if (bytes >= gb)
        return QFileSystemModel::tr("%1 GB").arg(QLocale().toString(bytes / gb, 'f', 2));
    if (bytes >= mb)
        return QFileSystemModel::tr("%1 MB").arg(QLocale().toString(bytes / mb, 'f', 1));
    if (bytes >= kb)
        return QFileSystemModel::tr("%1 KB").arg(QLocale().toString(bytes / kb));
    return QFileSystemModel::tr("%1 byte(s)").arg(QLocale().toString(bytes));
}
