#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>

class Node;

class Edge : public QGraphicsItem {
public:
    Edge(Node *sourceNode, Node *destNode, bool dir);

    Node *sourceNode() const;
    Node *destNode() const;

    void findPoints();

    // enum { Type = UserType + 2 };
    // int type() const override { return Type; }

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) override;

private:
    Node *source, *dest;

    QPointF sourcePoint;
    QPointF destPoint;
    bool directed;

};

#endif // EDGE_H
