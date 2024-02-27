#ifndef GRAPHVISUALISERWIDGET_H
#define GRAPHVISUALISERWIDGET_H

#include <QMainWindow>
#include <QGraphicsView>

class Node;

class GraphVisualiserWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GraphVisualiserWidget(QWidget *parent = nullptr);
    QVector <Node *> nodes();


public slots:
    void randomise();



private:
    int timerId = 0;
    QVector <Node *> nodesList;
};
#endif // GRAPHVISUALISERWIDGET_H









