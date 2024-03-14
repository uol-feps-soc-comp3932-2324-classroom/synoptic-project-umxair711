#include "graphvisualiserwidget.h"
#include "edge.h"
#include "node.h"

#include <QKeyEvent>
#include <QRandomGenerator>
#include <QPushButton>
#include <QSpinBox>
#include <QMessageBox>
#include <QCheckBox>

GraphVisualiserWidget::GraphVisualiserWidget(QWidget *parent)
    : QGraphicsView(parent)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    setScene(scene);
    setMinimumSize(700, 500);


    QPushButton *button = new QPushButton(this);
    // scene->addItem(qobject_cast<QGraphicsItem *>(button)); // not needed
    // button->setGeometry(50, 50, 70, 30);
    button->setText("Random");
    connect(button, &QPushButton::clicked, this, &GraphVisualiserWidget::randomise);


    directedCheckBox->setText("Directed");
    directedCheckBox->setGeometry(79, 30, 80, 20);

    minNodesSelecter->setGeometry(80, 5, 50, 20);
    minNodesSelecter->setRange(5, 15);
    minNodesSelecter->setValue(5);

    maxNodesSelecter->setGeometry(140, 5, 50, 20);
    maxNodesSelecter->setRange(5, 15);
    maxNodesSelecter->setValue(5);
    maxNodesSelecter->setStatusTip("Max nodes");
    // scene->addItem(qobject_cast<QGraphicsItem *>(maxNodesSelecter)); // not needed


    numOfNodesLabel->setGeometry(200, 5, 250, 20);
    numOfEdgesLabel->setGeometry(330, 5, 250, 20);

    randomise();
}


void GraphVisualiserWidget::randomise()
{
    scene()->clear();
    nodesList.clear();
    int max = maxNodesSelecter->value();
    int min = minNodesSelecter->value() - 1;

    if (min < max){
        for (int i = 0; i <= QRandomGenerator::global()->bounded(min, max); i++){
            qreal x = QRandomGenerator::global()->bounded(400);
            qreal y = QRandomGenerator::global()->bounded(400);
            Node *node = new Node(this);
            node->setPos(x, y);
            nodesList.append(node);
            scene()->addItem(node);
        }

        int maxNumOfEdges = QRandomGenerator::global()->bounded((nodesList.count() * (nodesList.count() - 1)) / 2);
        for (int i = 0; i < maxNumOfEdges; i++){
            int node1;
            int node2;
            bool edgeExists = true;
            while (edgeExists){
                edgeExists = false;
                do {
                    node1 = QRandomGenerator::global()->bounded(0, nodesList.count());
                    node2 = QRandomGenerator::global()->bounded(0, nodesList.count());
                } while (node1 == node2);

                if (nodesList[node1]->edges().isEmpty() || nodesList[node2]->edges().isEmpty()){
                    break;
                }

                for (Edge *edge1 : nodesList[node1]->edges()){
                    for (Edge *edge2 : nodesList[node2]->edges()){
                        if (edge1 == edge2){
                            edgeExists = true;
                        }
                    }
                }
            }
            bool directed = directedCheckBox->isChecked();
            scene()->addItem(new Edge(nodesList[node1], nodesList[node2], directed));
        }
        numOfNodesLabel->setText("Number of nodes: " + QString::number(nodesList.count()));
        numOfEdgesLabel->setText("Number of edges: " + QString::number(maxNumOfEdges));
    }
    else if (min >= max){
        QMessageBox *rangeError = new QMessageBox(this);
        rangeError->setText("Minimum nodes cannot be more than maximum nodes.");
        rangeError->show();
        minNodesSelecter->setValue(maxNodesSelecter->value());
    }

}

QVector <Node *> GraphVisualiserWidget::nodes()
{
    return nodesList;
}

