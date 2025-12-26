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
#include <QDesktopServices>
#include <QDialog>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QMimeDatabase>


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

    QString path = model->filePath(index);

    QMenu menu;
    QAction *openAction = menu.addAction("Ouvrir");
    QAction *propertiesAction = menu.addAction("Propriétés");
    menu.addSeparator();
    QAction *deleteAction = menu.addAction("Supprimer");

    QAction *selectedAction = menu.exec(treeView->viewport()->mapToGlobal(pos));
    if (!selectedAction)
        return;

    // Débug pour voir quelle action est sélectionnée
    qDebug() << "Action sélectionnée:" << selectedAction->text();
    qDebug() << "openAction:" << openAction;
    qDebug() << "propertiesAction:" << propertiesAction;
    qDebug() << "deleteAction:" << deleteAction;
    qDebug() << "selectedAction:" << selectedAction;

    // Comparaison par texte au lieu de pointeur (plus sûr)
    if (selectedAction->text() == "Ouvrir") {
        openFile(path);
    } else if (selectedAction->text() == "Propriétés") {
        fileProperties(path);
    } else if (selectedAction->text() == "Supprimer") {
        deleteFile(path);
    }
}


void FileExplorer::openFile(const QString &path)
{
    qDebug() << "Ouvrir :" << path;
    emit fileOpened(path);

    // TODO: Implémenter l'ouverture réelle du fichier
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void FileExplorer::fileProperties(const QString &path)
{
    QFileInfo info(path);
    if (!info.exists()) {
        QMessageBox::warning(this, "Erreur", "Le fichier n'existe pas.");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Propriétés - " + info.fileName());
    dialog.setFixedSize(500, 400);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    QFormLayout *form = new QFormLayout();

    // Style pour les labels de valeurs (sélectionnables)
    auto createValueLabel = [](const QString &text) {
        QLabel *label = new QLabel(text);
        label->setTextInteractionFlags(Qt::TextSelectableByMouse);
        label->setWordWrap(true);
        return label;
    };

    QMimeDatabase mimeDb;
    QMimeType mime = mimeDb.mimeTypeForFile(info);

    // Informations de base
    form->addRow("<b>Nom :</b>", createValueLabel(info.fileName()));
    form->addRow("<b>Chemin :</b>", createValueLabel(info.absoluteFilePath()));
    form->addRow("<b>Type :</b>", createValueLabel(
        info.isDir() ? "Dossier" : mime.comment().isEmpty() ? mime.name() : mime.comment()
    ));

    // Taille formatée
    QString sizeStr;
    if (info.isDir()) {
        sizeStr = "Dossier";
    } else {
        qint64 size = info.size();
        if (size < 1024)
            sizeStr = QString::number(size) + " octets";
        else if (size < 1024 * 1024)
            sizeStr = QString::number(size / 1024.0, 'f', 2) + " Ko";
        else if (size < 1024 * 1024 * 1024)
            sizeStr = QString::number(size / (1024.0 * 1024.0), 'f', 2) + " Mo";
        else
            sizeStr = QString::number(size / (1024.0 * 1024.0 * 1024.0), 'f', 2) + " Go";
    }
    form->addRow("<b>Taille :</b>", createValueLabel(sizeStr));

    // Dates
    form->addRow("<b>Créé le :</b>", createValueLabel(
        info.birthTime().isValid()
            ? info.birthTime().toString("dd/MM/yyyy HH:mm:ss")
            : "Non disponible"
    ));
    form->addRow("<b>Modifié le :</b>", createValueLabel(
        info.lastModified().toString("dd/MM/yyyy HH:mm:ss")
    ));
    form->addRow("<b>Dernier accès :</b>", createValueLabel(
        info.lastRead().toString("dd/MM/yyyy HH:mm:ss")
    ));

    // Permissions détaillées
    QFile::Permissions p = info.permissions();
    QString perms;

    // Propriétaire
    perms += "Propriétaire : ";
    perms += (p & QFile::ReadOwner)  ? "r" : "-";
    perms += (p & QFile::WriteOwner) ? "w" : "-";
    perms += (p & QFile::ExeOwner)   ? "x" : "-";
    perms += "\n";

    // Groupe
    perms += "Groupe : ";
    perms += (p & QFile::ReadGroup)  ? "r" : "-";
    perms += (p & QFile::WriteGroup) ? "w" : "-";
    perms += (p & QFile::ExeGroup)   ? "x" : "-";
    perms += "\n";

    // Autres
    perms += "Autres : ";
    perms += (p & QFile::ReadOther)  ? "r" : "-";
    perms += (p & QFile::WriteOther) ? "w" : "-";
    perms += (p & QFile::ExeOther)   ? "x" : "-";

    form->addRow("<b>Permissions :</b>", createValueLabel(perms));

    // Attributs supplémentaires
    QStringList attributes;
    if (info.isHidden()) attributes << "Caché";
    if (info.isSymLink()) attributes << "Lien symbolique";
    if (info.isReadable()) attributes << "Lecture";
    if (info.isWritable()) attributes << "Écriture";
    if (info.isExecutable()) attributes << "Exécutable";

    if (!attributes.isEmpty()) {
        form->addRow("<b>Attributs :</b>", createValueLabel(attributes.join(", ")));
    }

    mainLayout->addLayout(form);
    mainLayout->addStretch();

    // Boutons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    QPushButton *closeBtn = new QPushButton("Fermer");
    closeBtn->setDefault(true);
    connect(closeBtn, &QPushButton::clicked, &dialog, &QDialog::accept);

    buttonLayout->addWidget(closeBtn);
    mainLayout->addLayout(buttonLayout);

    dialog.exec();
}

void FileExplorer::deleteFile(const QString &path)
{
    // Demander confirmation
    const QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation",
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