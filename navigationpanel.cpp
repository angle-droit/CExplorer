#include "navigationpanel.h"
#include <QListWidget>
#include <QVBoxLayout>
#include <QDir>

NavigationPanel::NavigationPanel(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    addDefaultLocations();
}

NavigationPanel::~NavigationPanel()
{
}

void NavigationPanel::setupUI()
{
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    
    listWidget = new QListWidget(this);
    layout->addWidget(listWidget);
    
    connect(listWidget, &QListWidget::itemClicked,
            this, &NavigationPanel::onItemClicked);
}

void NavigationPanel::addDefaultLocations() const {
    listWidget->addItem("Racine");
    listWidget->addItem("Home");
    listWidget->addItem("Desktop");
    listWidget->addItem("Documents");
}

QString NavigationPanel::getPathForItem(const QString &itemText) {
    if (itemText == "Racine") {
#if defined(Q_OS_WIN)
        return "C:/";
#else
        return "/";
#endif
    } else if (itemText == "Home") {
        return QDir::homePath();
    } else if (itemText == "Desktop") {
        return QDir::homePath() + "/Desktop";
    } else if (itemText == "Documents") {
        return QDir::homePath() + "/Documents";
    }
    
    return QString();
}

void NavigationPanel::onItemClicked(const QListWidgetItem *item)
{
    if (const QString path = getPathForItem(item->text()); !path.isEmpty()) {
        emit locationSelected(path);
    }
}