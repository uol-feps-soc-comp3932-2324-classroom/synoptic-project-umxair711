#include "node.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

// Node constructor
Node::Node(const char name, const int col)
    : name(name), col(col)
{
}

// Node destructor
Node::~Node() {
}

// Returns the name of the node
char Node::getName() {
    return name;
}

// Returns the color of the node
int Node::getCol() {
    return col;
}

// Returns the color of the node
QColor Node::getNodeColour() {
    return nodeColour;
}

// Sets the color of the node
void Node::setNodeColour(const QColor &colour) {
    nodeColour = colour;
    update();
}

// Returns the shape of the node (ellipse)
QPainterPath Node::shape() const {
    QPainterPath path;
    path.addEllipse(-15, -15, 30, 30);
    return path;
}

// Returns the bounding rectangle of the node
QRectF Node::boundingRect() const {
    qreal adjust = 2;
    return QRectF( -10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
}

// Paints the node with its color and name
void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    // Draw the node circle with light fill color and black border
    painter->setBrush(nodeColour); // Light fill color
    QPen borderPen(Qt::black);
    borderPen.setWidth(1); // Set the width of the border
    painter->setPen(borderPen); // Black border color
    painter->drawEllipse(-15, -15, 30, 30); // Larger circle, adjust the size as needed

    // Draw the node text
    QFont font("Didot", 15, QFont::Bold); // Garamond font with size 10
    painter->setFont(font);
    QPen textPen(Qt::white, 0.5);
    painter->setPen(textPen); // Set text color to black
    painter->drawText(QRectF(-15, -14, 30, 30), Qt::AlignCenter, QString(name)); // Draw text
}
