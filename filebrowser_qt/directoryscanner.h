#ifndef DIRECTORYSCANNER_H
#define DIRECTORYSCANNER_H

#include <QWidget>

class QStandardItemModel;
class QTreeView;
class QLineEdit;
class QStatusBar;

class QPushButton;
class QItemSelection;


class DirectoryScanner : public QWidget
{
    Q_OBJECT
public:
    explicit DirectoryScanner(QWidget *parent = 0);

signals:

private slots:
    void on_lineedit_returnPressed();
    void on_tree_selectionChanged(const QItemSelection & newSelection, const QItemSelection & oldSelection);
    void on_clearButton_clicked ();
public slots:
    void scanDirectory(const QString &path);

private:

    QString size2string(quint64 bytes);

QStandardItemModel *model;
QTreeView* list;
QLineEdit* lineeditPath;
QLineEdit* lineeditFilter;
QStatusBar* statusbar;
QPushButton* clearButton;
};


#endif // DIRECTORYSCANNER_H
