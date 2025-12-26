#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    app.setApplicationName("CExplorer");
    app.setApplicationVersion("0.0.1");

    MainWindow w;
    w.show();

    return app.exec();
}
