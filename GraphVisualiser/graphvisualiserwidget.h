#ifndef GRAPHVISUALISERWIDGET_H
#define GRAPHVISUALISERWIDGET_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class GraphVisualiserWidget;
}
QT_END_NAMESPACE

class GraphVisualiserWidget : public QMainWindow
{
    Q_OBJECT

public:
    GraphVisualiserWidget(QWidget *parent = nullptr);
    ~GraphVisualiserWidget();

private:
    Ui::GraphVisualiserWidget *ui;
};
#endif // GRAPHVISUALISERWIDGET_H
