//
// Created by axel on 26/12/2025.
//

#include "fileexplorer.h"
#include <QTreeView>
#include <QFileSystemModel>
#include <QVBoxLayout>
#include <QMenu>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QMessageBox>

FileExplorer::FileExplorer(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    setupModel();
    setupContextMenu();
}

FileExplorer::~FileExplorer()
{
}

void FileExplorer::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    treeView = new QTreeView(this);
    layout->addWidget(treeView);
}

void FileExplorer::setupModel()
{
    model = new QFileSystemModel(this);
    model->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
    model->setRootPath("");

    treeView->setModel(model);

    // Naviguer vers la racine par défaut
    goToRoot();
}

void FileExplorer::setupContextMenu()
{
    treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(treeView, &QTreeView::customContextMenuRequested,
            this, &FileExplorer::showContextMenu);
}

QString FileExplorer::getRootPath() const
{
#if defined(Q_OS_WIN)
    return "C:/";
#else
    return "/";
#endif
}

void FileExplorer::navigateToPath(const QString &path)
{
    QModelIndex index = model->index(path);
    if (index.isValid()) {
        treeView->setRootIndex(index);
    }
}

void FileExplorer::refresh()
{
    // Récupérer l'index actuel
    QModelIndex currentIndex = treeView->rootIndex();
    QString currentPath = model->filePath(currentIndex);

    // Forcer le rechargement du modèle
    model->setRootPath("");
    model->setRootPath(currentPath);

    // Restaurer la vue
    treeView->setRootIndex(model->index(currentPath));
}

void FileExplorer::goToRoot()
{
    navigateToPath(getRootPath());
}

void FileExplorer::showContextMenu(const QPoint &pos)
{
    QModelIndex index = treeView->indexAt(pos);
    if (!index.isValid())
        return;

    QMenu menu;
    QAction *openAction = menu.addAction("Ouvrir");
    QAction *deleteAction = menu.addAction("Supprimer");

    QAction *selectedAction = menu.exec(treeView->viewport()->mapToGlobal(pos));

    QString path = model->filePath(index);

    if (selectedAction == openAction) {
        openFile(path);
    } else if (selectedAction == deleteAction) {
        deleteFile(path);
    }
}

void FileExplorer::openFile(const QString &path)
{
    qDebug() << "Ouvrir :" << path;
    emit fileOpened(path);

    // TODO: Implémenter l'ouverture réelle du fichier
    // QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void FileExplorer::deleteFile(const QString &path)
{
    // Demander confirmation
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Voulez-vous vraiment supprimer : " + path + " ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes)
        return;

    // Supprimer le fichier ou dossier
    QFileInfo fileInfo(path);
    bool success = false;

    if (fileInfo.isDir()) {
        QDir dir(path);
        success = dir.removeRecursively();
    } else {
        QFile file(path);
        success = file.remove();
    }

    if (success) {
        qDebug() << "Supprimé :" << path;
        emit fileDeleted(path);
    } else {
        QMessageBox::warning(this, "Erreur",
                           "Impossible de supprimer : " + path);
    }
}