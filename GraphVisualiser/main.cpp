#include "graphvisualiserwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GraphVisualiserWidget w;
    w.show();
    return a.exec();
}
