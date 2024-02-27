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


public slots:
    void shuffle();
    void buttonClicked();



private:
    int timerId = 0;
    Node *centerNode;
    // Ui::GraphVisualiserWidget *ui;
};
#endif // GRAPHVISUALISERWIDGET_H









