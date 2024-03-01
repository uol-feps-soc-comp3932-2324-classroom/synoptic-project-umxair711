#ifndef GRAPHVISUALISERWIDGET_H
#define GRAPHVISUALISERWIDGET_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QSpinBox>
#include <QLabel>

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
    // int maxNodes;
    QSpinBox *maxNodesSelecter = new QSpinBox(this);
    QSpinBox *minNodesSelecter = new QSpinBox(this);
    QLabel *numOfNodesLabel = new QLabel(this);
    QLabel *numOfEdgesLabel = new QLabel(this);
};
#endif // GRAPHVISUALISERWIDGET_H









