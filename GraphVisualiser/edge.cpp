#include "edge.h"
#include "node.h"

#include <QPainter>
#include <QtMath>


Edge::Edge(Node *sourceNode, Node *destNode, bool dir)
    : source(sourceNode), dest(destNode), directed(dir)
{
    source->addEdge(this);
    dest->addEdge(this);
    findPoints();
}

Node *Edge::sourceNode() const
{
    return source;
}

Node *Edge::destNode() const
{
    return dest;
}

void Edge::findPoints()
{
    QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
    sourcePoint = line.p1();
    destPoint = line.p2();
}


QRectF Edge::boundingRect() const
{
    if (!source || !dest)
        return QRectF();

    qreal penWidth = 2;
    qreal extra = (penWidth) / 2.0;

    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QLineF line(sourcePoint, destPoint);
    painter->setPen(QPen(Qt::darkGreen, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

    if (directed){
        double angle = std::atan2(-line.dy(), line.dx());

        QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + M_PI / 3) * 10,
                                                      cos(angle + M_PI / 3) * 10);
        QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + M_PI - M_PI / 3) * 10,
                                                      cos(angle + M_PI - M_PI / 3) * 10);

        painter->setBrush(Qt::black);
        painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
    }
}

