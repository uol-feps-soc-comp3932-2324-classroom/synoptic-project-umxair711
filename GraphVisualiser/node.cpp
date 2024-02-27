#include "edge.h"
#include "node.h"
#include "graphvisualiserwidget.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

Node::Node(GraphVisualiserWidget *graphVisualiserWidget)
    : graph(graphVisualiserWidget)
{

}

void Node::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->findPoints();
}

QVector<Edge *> Node::edges() const
{
    return edgeList;
}


QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}

QRectF Node::boundingRect() const
{
    qreal adjust = 2;
    return QRectF( -10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::gray);
    painter->drawEllipse(-10, -10, 20, 20);

    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-10, -10, 20, 20);
}




