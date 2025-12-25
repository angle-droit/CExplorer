#include <QApplication>
#include <QMainWindow>
#include <QTreeView>
#include <QFileSystemModel>
#include <QToolBar>
#include <QDockWidget>
#include <QListWidget>
#include <QAction>
#include <QMenu>
#include <QMessageBox>

QString getRootPath()
{
#if defined(Q_OS_WIN)
    return "C:/";
#else
    return "/";
#endif
}




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // =====================
    // Fenêtre principale
    // =====================
    QMainWindow mainWindow;
    QWidget window;
    window.setFixedSize(400, 300);
    mainWindow.setWindowTitle("Explorateur de dossiers");
    mainWindow.resize(900, 600);

    // =====================
    // TOP BAR (ToolBar)
    // =====================
    QToolBar *topBar = new QToolBar("TopBar");
    QAction *refreshAction = new QAction("Rafraîchir");
    QAction *rootAction = new QAction("Racine");

    topBar->addAction(rootAction);
    topBar->addAction(refreshAction);
    mainWindow.addToolBar(topBar);

    // =====================
    // MODELE FILE SYSTEM
    // =====================
    QFileSystemModel *model = new QFileSystemModel;
    model->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
    model->setRootPath("");

    // =====================
    // ZONE CENTRALE (Tree)
    // =====================
    QTreeView *treeView = new QTreeView;
    treeView->setModel(model);
    treeView->hideColumn(1);
    treeView->hideColumn(2);
    treeView->hideColumn(3);

    QString rootPath = getRootPath();
    treeView->setRootIndex(model->index(rootPath));

    mainWindow.setCentralWidget(treeView);

    // =====================
    // NAV BAR (Dock)
    // =====================
    QDockWidget *navBar = new QDockWidget("Navigation");
    QListWidget *navList = new QListWidget;

    navList->addItem("Racine");
    navList->addItem("Home");
    navList->addItem("Desktop");
    navList->addItem("Documents");

    navBar->setWidget(navList);
    mainWindow.addDockWidget(Qt::LeftDockWidgetArea, navBar);

    // =====================
    // ACTIONS
    // =====================
    QObject::connect(rootAction, &QAction::triggered, [&]() {
        treeView->setRootIndex(model->index(getRootPath()));
    });

    // disabled, not implemented

    // QObject::connect(refreshAction, &QAction::triggered, [&]() {
    //     model->refresh();
    // });

    QObject::connect(navList, &QListWidget::itemClicked, [&](QListWidgetItem *item) {
        QString path;

        if (item->text() == "Racine")
            path = getRootPath();
        else if (item->text() == "Home")
            path = QDir::homePath();
        else if (item->text() == "Desktop")
            path = QDir::homePath() + "/Desktop";
        else if (item->text() == "Documents")
            path = QDir::homePath() + "/Documents";

        treeView->setRootIndex(model->index(path));
    });

    // =====================
    // CONTEXT MENU
    // =====================
    mainWindow.setContextMenuPolicy(Qt::CustomContextMenu);

    QObject::connect(&window, &QWidget::customContextMenuRequested,
                     [&window](const QPoint &pos) {
        QMenu menu(&window);
        menu.addAction("Option 1", [&window]() {
            QMessageBox::information(&window, "Menu", "Option 1 sélectionnée !");
        });
        menu.addAction("Option 2", [&window]() {
            QMessageBox::information(&window, "Menu", "Option 2 sélectionnée !");
        });
        menu.exec(window.mapToGlobal(pos));
    });;

    // =====================
    // SHOW
    // =====================
    mainWindow.show();
    return a.exec();
}