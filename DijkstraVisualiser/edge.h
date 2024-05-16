#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>

class Node; // Forward declaration of the Node class

class Edge : public QGraphicsItem {
public:
    Edge(Node *sourceNode, Node *destNode, bool dir=false, int weight=0);
    ~Edge();

    Node *sourceNode() const; // Getter for the source node
    Node *destNode() const; // Getter for the destination node
    QString getName(); // Getter for the edge name
    int getWeight(); // Getter for the edge weight
    bool isDirected(); // Check if the edge is directed
    void setEdgeColour(const QColor &colour); // Setter for the edge colour
    QColor getEdgeColour(); // Getter for the edge colour
    QPointF getSourcePoint(); // Getter for the source point of the edge
    QPointF getDestPoint(); // Getter for the destination point of the edge
    bool intersects(const Edge& other) const; // Check if the edge intersects with another edge

protected:
    void findPoints(); // Helper function to find source and destination points
    void paint(QPainter *painter, const QStyleOptionGraphicsItem * = nullptr, QWidget * = nullptr) override; // Overridden paint function
    QRectF boundingRect() const override; // Overridden boundingRect function

private:
    QString name; // Edge name
    Node *source, *dest; // Source and destination nodes
    QColor edgeColour = Qt::black; // Edge colour
    bool directed; // Flag indicating if the edge is directed
    int weight; // Edge weight
    QPointF sourcePoint; // Source point of the edge
    QPointF destPoint; // Destination point of the edge
};

#endif // EDGE_H
