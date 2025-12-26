#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class FileExplorer;
class NavigationPanel;
class QToolBar;
class QAction;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    // Composants
    FileExplorer *fileExplorer;
    NavigationPanel *navPanel;
    QToolBar *toolbar;

    // Actions
    QAction *refreshAction;
    QAction *rootAction;

    // Méthodes d'initialisation
    void setupUI();
    void createToolbar();
    void createActions();
    void connectSignals();

private slots:
    void onNavigate(const QString &path);
    void onRefresh();
    void onGoToRoot();
    void onFileDeleted(const QString &path);
};

#endif // MAINWINDOW_H