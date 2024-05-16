#include "edge.h"
#include "node.h"
#include <QPainter>
#include <QtMath>
#include <QThread>
#include <QLineF>

// Edge constructor initializes an edge with source, destination nodes, directionality, and weight
Edge::Edge(Node *sourceNode, Node *destNode, bool dir, int weight)
    : source(sourceNode), dest(destNode), directed(dir), weight(weight)
{
    name.append(source->getName());
    name.append(dest->getName());
    findPoints();
}

// Edge destructor
Edge::~Edge() {}

// Returns the source node of the edge
Node *Edge::sourceNode() const {
    return source;
}

// Returns the destination node of the edge
Node *Edge::destNode() const {
    return dest;
}

// Returns the name of the edge
QString Edge::getName() {
    return name;
}

// Returns the weight of the edge
int Edge::getWeight() {
    return weight;
}

// Returns whether the edge is directed
bool Edge::isDirected() {
    return directed;
}

// Sets the edge color and updates the display
void Edge::setEdgeColour(const QColor &colour) {
    edgeColour = colour;
    update();
}

// Returns the current edge color
QColor Edge::getEdgeColour() {
    return edgeColour;
}

// Returns the source point of the edge
QPointF Edge::getSourcePoint() {
    return sourcePoint;
}

// Returns the destination point of the edge
QPointF Edge::getDestPoint() {
    return destPoint;
}

// Checks if this edge intersects with another edge
bool Edge::intersects(const Edge &other) const {
    QLineF thisLine(sourcePoint, destPoint);
    QLineF otherLine(other.sourcePoint, other.destPoint);

    QPointF intersectionPoint;
    if (thisLine.intersects(otherLine, &intersectionPoint) == QLineF::BoundedIntersection) {
        // Lines intersect
        return true;
    }
    return false;
}

// Calculates the source and destination points of the edge based on the source and destination nodes
void Edge::findPoints() {
    QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(30.)) {
        QPointF edgeOffset((line.dx() * 15) / length, (line.dy() * 15) / length);
        sourcePoint = line.p1() + edgeOffset;
        destPoint = line.p2() - edgeOffset;
    } else {
        sourcePoint = destPoint = line.p1();
    }
}

// Returns the bounding rectangle for the edge
QRectF Edge::boundingRect() const {
    qreal penWidth = 1;
    qreal extra = (penWidth + 10) / 2.0;

    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

// Paints the edge, including arrows if directed and weight if specified
void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

    // Draw the line itself
    painter->setPen(QPen(edgeColour, 1.8, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

    // Draw the arrows
    if (directed) {
        double angle = std::atan2(-line.dy(), line.dx());

        QPointF destArrowP1 = destPoint + QPointF(sin(angle - M_PI / 3) * 10,
                                                  cos(angle - M_PI / 3) * 10);
        QPointF destArrowP2 = destPoint + QPointF(sin(angle - M_PI + M_PI / 3) * 10,
                                                  cos(angle - M_PI + M_PI / 3) * 10);

        painter->setBrush(edgeColour);
        painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
    }

    // Draw the weight
    if (weight != 0) {
        QFont font("Didot", 14);
        font.setWeight(QFont::ExtraBold);
        painter->setFont(font);
        QPointF textPos = (line.pointAt(0.6));

        // Draw the background text
        painter->setPen(Qt::white);
        painter->drawText(textPos, QString::number(weight));

        painter->setPen(QPen(QColor("#E71D36"), 1.8, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawText(textPos, QString::number(weight));
    }
}
