#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QVector>

class Edge;
class GraphVisualiserWidget;

class Node : public QGraphicsItem
{
public:
    Node(GraphVisualiserWidget *graphVisualiserWidget);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void addEdge(Edge *edge);
    QVector<Edge *> edges() const;


private:
    QVector<Edge *> edgeList;
    QPointF newPos;
    GraphVisualiserWidget *graph;
};

#endif // NODE_H
