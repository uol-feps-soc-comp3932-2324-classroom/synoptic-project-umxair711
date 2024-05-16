#ifndef WIDGET_H
#define WIDGET_H

#include "edge.h"
#include <QWidget>
#include <stack>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui; // Pointer to the UI object
    const int sceneWidth = 771; // Scene width constant
    const int sceneHeight = 600; // Scene height constant
    std::map<int, char> labels; // Map for labels
    std::stack<Edge*> shortestPath; // Stack for shortest path
    QString correctAnswer; // Correct answer string
    int questionsAttempted = 0; // Number of questions attempted
    int questionsCorrect = 0; // Number of questions answered correctly

    // Private functions
    void resetScreen();
    void generateGraph(int graphType);
    QList<Node *> generateNodes(int graphType, const int numOfColumns);
    QList<Edge *> generateEdges(QList<Node *> allNodes, const int graphType);
    void removeEdgesWithHighIntersections(QList<Edge*>& allEdges, int intersectionLimit);
    int countIntersectionsForEdge(const Edge* edgeToCheck, const QList<Edge*>& allEdges);
    void removeNodeIntersectingEdges(QList<Node *> allNodes, QList<Edge *> &allEdges);
    std::stack<Edge *> dijkstrasAlgorithm(Node* startNode, Node* endNode, const QList<Node*>& allNodes, const QList<Edge*>& allEdges);
    void generateQuestion(std::stack<Edge *> shortestPath, const QList<Node *> allNodes, const QList<Edge *> &allEdges);
    QList<QString> findAllPaths(const QString& shortestPath, Node* startNode, Node* endNode, const QList<Edge*>& allEdges);
    QList<QList<Node*>> dfs(Node* startNode, Node* endNode, const QList<Edge*>& allEdges);
    void printGraphRepresentation(const QList<Node*>& allNodes, const QList<Edge*>& allEdges);
    void highlightShortestPath(QColor colour);

private slots:
    // Private slots
    void on_nextGraphButton_clicked();
    void on_submitButton_clicked();
    void wheelEvent(QWheelEvent *event);
    void on_helpButton_clicked();
};
#endif // WIDGET_H
