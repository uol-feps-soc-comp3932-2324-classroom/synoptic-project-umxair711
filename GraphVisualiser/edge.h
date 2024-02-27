#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>

class Node;

class Edge : public QGraphicsItem {
public:
    Edge(Node *sourceNode, Node *destNode);

    Node *sourceNode() const;
    Node *destNode() const;

    enum { Type = UserType + 2 };
    int type() const override { return Type; }

protected:
    // QRectF boundingRect() const override;

private:
    Node *source, *dest;

    QPointF sourcePoint;
    QPointF destPoint;
    // qreal arrowSize = 10;
};

#endif // EDGE_H
