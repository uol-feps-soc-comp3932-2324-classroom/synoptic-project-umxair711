#include "graphvisualiserwidget.h"
#include "edge.h"
#include "node.h"

// #include <math.h>

#include <QKeyEvent>
#include <QRandomGenerator>
#include <QPushButton>

GraphVisualiserWidget::GraphVisualiserWidget(QWidget *parent)
    : QGraphicsView(parent)
{
    QGraphicsScene *scene = new QGraphicsScene(this);



    QPushButton *button = new QPushButton(this);
    scene->addItem(qobject_cast<QGraphicsItem *>(button));
    button->setGeometry(50, 50, 70, 30);
    button->setText("Shuffle");
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

