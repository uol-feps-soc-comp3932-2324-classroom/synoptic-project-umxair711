#include "edge.h"
#include "node.h"

#include <QPainter>
#include <QtMath>


Edge::Edge(Node *sourceNode, Node *destNode)
    : source(sourceNode), dest(destNode)
{
    setAcceptedMouseButtons(Qt::NoButton);
    source->addEdge(this);
    dest->addEdge(this);
}

Node *Edge::sourceNode() const
{
    return source;
}

Node *Edge::destNode() const
{
    return dest;
}




