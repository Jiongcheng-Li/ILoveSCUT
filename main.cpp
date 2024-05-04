#include "mainwindow.h"

#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(scut);

    QApplication app(argc, argv);
    MainWindow window;
    window.setFixedSize(1020, 600);
    window.show();
    return app.exec();
}
