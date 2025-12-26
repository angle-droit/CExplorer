#ifndef FILEEXPLORER_H
#define FILEEXPLORER_H

#include <QWidget>

class QTreeView;
class QFileSystemModel;
class QVBoxLayout;

class FileExplorer : public QWidget
{
    Q_OBJECT

public:
    explicit FileExplorer(QWidget *parent = nullptr);
    ~FileExplorer();

public slots:
    void navigateToPath(const QString &path);
    void refresh();
    void goToRoot();

    signals:
        void fileOpened(const QString &path);
    void fileDeleted(const QString &path);

private:
    QTreeView *treeView;
    QFileSystemModel *model;

    void setupUI();
    void setupModel();
    void setupContextMenu();
    QString getRootPath() const;

private slots:
    void showContextMenu(const QPoint &pos);
    void openFile(const QString &path);

    void fileProperties(const QString &path);

    void deleteFile(const QString &path);
};

#endif // FILEEXPLORER_H