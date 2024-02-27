#include "graphvisualiserwidget.h"
#include "edge.h"
#include "node.h"

#include <algorithm>

#include <QKeyEvent>
#include <QRandomGenerator>
#include <QPushButton>

GraphVisualiserWidget::GraphVisualiserWidget(QWidget *parent)
    : QGraphicsView(parent)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    setScene(scene);
    setMinimumSize(500, 400);

    randomise();

    QPushButton *button = new QPushButton(this);
    scene->addItem(qobject_cast<QGraphicsItem *>(button));
    // button->setGeometry(50, 50, 70, 30);
    button->setText("Random");
    connect(button, &QPushButton::clicked, this, &GraphVisualiserWidget::randomise);

}


void GraphVisualiserWidget::randomise()
{
    scene()->clear();
    nodesList.clear();
    for (int i = 0; i <= QRandomGenerator::global()->bounded(15); i++){
        qreal x = QRandomGenerator::global()->bounded(300);
        qreal y = QRandomGenerator::global()->bounded(300);
        Node *node = new Node(this);
        node->setPos(x, y);
        nodesList.append(node);
        scene()->addItem(node);

        if (i > 0){
            scene()->addItem(new Edge(nodesList[i], nodesList[i-1]));
        }
    }

    // auto rd = std::random_device {};
    // auto rng = std::default_random_engine { rd() };
    // std::shuffle(nodesList.begin(), nodesList.end(), rng);
}

QVector <Node *> GraphVisualiserWidget::nodes()
{
    return nodesList;
}

