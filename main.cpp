#include <QApplication>
#include <QMainWindow>
#include <QTreeView>
#include <QFileSystemModel>
#include <QToolBar>
#include <QDockWidget>
#include <QListWidget>
#include <QAction>

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
    model->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
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
    // SHOW
    // =====================
    mainWindow.show();

    return a.exec();
}
