//
// Created by axel on 26/12/2025.
//

#include "mainwindow.h"

#include "mainwindow.h"
#include "fileexplorer.h"
#include "navigationpanel.h"
#include <QToolBar>
#include <QAction>
#include <QDockWidget>
#include <QSystemTrayIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("CExplorer");
    setWindowIcon(QIcon(":/images/images/icon.png"));
    resize(900, 600);

    setupUI();
    createActions();
    createToolbar();
    connectSignals();
}

MainWindow::~MainWindow()
{
    // Qt gère automatiquement la suppression des enfants
}

void MainWindow::setupUI()
{
    // Créer l'explorateur de fichiers (widget central)
    fileExplorer = new FileExplorer(this);
    setCentralWidget(fileExplorer);

    // Créer le panneau de navigation (dock)
    navPanel = new NavigationPanel(this);
    QDockWidget *dock = new QDockWidget("Navigation", this);
    dock->setWidget(navPanel);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
}

void MainWindow::createActions()
{
    refreshAction = new QAction("Rafraîchir", this);
    rootAction = new QAction("Racine", this);
}

void MainWindow::createToolbar()
{
    toolbar = addToolBar("TopBar");
    toolbar->addAction(rootAction);
    toolbar->addAction(refreshAction);
}

void MainWindow::connectSignals()
{
    // Navigation panel -> File explorer
    connect(navPanel, &NavigationPanel::locationSelected,
            this, &MainWindow::onNavigate);

    // Actions toolbar
    connect(refreshAction, &QAction::triggered,
            this, &MainWindow::onRefresh);
    connect(rootAction, &QAction::triggered,
            this, &MainWindow::onGoToRoot);

    // File explorer -> MainWindow (pour notifications)
    connect(fileExplorer, &FileExplorer::fileDeleted,
            this, &MainWindow::onFileDeleted);
}

void MainWindow::onNavigate(const QString &path)
{
    fileExplorer->navigateToPath(path);
}

void MainWindow::onRefresh()
{
    fileExplorer->refresh();
}

void MainWindow::onGoToRoot()
{
    fileExplorer->goToRoot();
}

void MainWindow::onFileDeleted(const QString &path)
{
    // Afficher une notification
    QSystemTrayIcon trayIcon(this);
    trayIcon.show();
    trayIcon.showMessage("Élément supprimé",
                         path + " a été supprimé !",
                         QSystemTrayIcon::Information,
                         5000);
}