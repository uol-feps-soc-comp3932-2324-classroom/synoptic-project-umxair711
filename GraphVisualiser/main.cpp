#include "graphvisualiserwidget.h"

#include <QApplication>
#include <QTime>
#include <QMainWindow>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    GraphVisualiserWidget *widget = new GraphVisualiserWidget;

    QMainWindow mainWindow;
    mainWindow.setCentralWidget(widget);

    mainWindow.show();
    return app.exec();
}
