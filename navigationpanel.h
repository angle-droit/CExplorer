#ifndef NAVIGATIONPANEL_H
#define NAVIGATIONPANEL_H

#include <QWidget>

class QListWidget;
class QListWidgetItem;

class NavigationPanel : public QWidget
{
    Q_OBJECT

public:
    explicit NavigationPanel(QWidget *parent = nullptr);
    ~NavigationPanel() override;

    signals:
        void locationSelected(const QString &path);

private:
    QListWidget *listWidget;

    void setupUI();
    void addDefaultLocations() const;

    static QString getPathForItem(const QString &itemText);

private slots:
    void onItemClicked(const QListWidgetItem *item);
};

#endif // NAVIGATIONPANEL_H