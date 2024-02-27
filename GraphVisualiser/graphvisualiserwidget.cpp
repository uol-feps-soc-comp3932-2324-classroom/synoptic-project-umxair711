#include "graphvisualiserwidget.h"
#include "edge.h"
#include "node.h"

#include <math.h>

#include <QKeyEvent>
#include <QRandomGenerator>
#include <QPushButton>

GraphVisualiserWidget::GraphVisualiserWidget(QWidget *parent)
    : QGraphicsView(parent)
{
    QGraphicsScene *scene = new QGraphicsScene(this);

    setScene(scene);
    setMinimumSize(500, 400);

    for (int i = 0; i <= QRandomGenerator::global()->bounded(8, 15); i++){
        qreal x = QRandomGenerator::global()->bounded(300);
        qreal y = QRandomGenerator::global()->bounded(300);
        Node *node = new Node(this);
        node->setPos(x, y);
        scene->addItem(node);
    }



    QPushButton *button = new QPushButton(this);
    scene->addItem(qobject_cast<QGraphicsItem *>(button));
    // button->setGeometry(50, 50, 70, 30);
    button->setText("Random");
    connect(button, &QPushButton::clicked, this, &GraphVisualiserWidget::buttonClicked);

}
//! [1]

void GraphVisualiserWidget::buttonClicked()
{
    shuffle();
}


void GraphVisualiserWidget::shuffle()
{

}

