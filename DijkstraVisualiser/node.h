#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QVector>

class Edge; // Forward declaration of the Edge class
class GraphVisualiserWidget; // Forward declaration of the GraphVisualiserWidget class

class Node : public QGraphicsItem
{
public:
    Node(const char name = ' ', const int col = -1);
    ~Node();

    char getName(); // Getter for the node name
    int getCol(); // Getter for the node colour index
    QColor getNodeColour(); // Getter for the node colour
    void setNodeColour(const QColor &colour); // Setter for the node colour

protected:
    QRectF boundingRect() const override; // Overridden boundingRect function
    QPainterPath shape() const override; // Overridden shape function
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override; // Overridden paint function

private:
    const char name; // Node name
    const int col; // Node column
    QColor nodeColour = QColor("#2C302E"); // Node colour
};

#endif // NODE_H
