#include <gtest/gtest.h>
#include <QList>
#include <QStack>
#include <QApplication>
#include <QColor>
#include <QLayoutItem>
#include <QRadioButton>
#include "widget.h"
#include "node.h"
#include "edge.h"

// Test Fixture
class WidgetTest : public ::testing::Test {
protected:
    QApplication* app; // Declare QApplication pointer
    Widget* widget;

    void SetUp() override {
        int argc = 0;
        char** argv = nullptr;
        app = new QApplication(argc, argv); // Initialize QApplication
        widget = new Widget();
    }

    void TearDown() override {
        delete widget;
        delete app; // Delete QApplication instance
    }
};

// Test generating nodes for uniqueness and constraints
TEST_F(WidgetTest, GenerateNodesUniqueAndConstraintsTest) {
    int graphType = 0; // Assuming a specific graph type for this test
    int numOfColumns = 4; // Define the number of columns for the test

    QList<Node*> nodes = widget->generateNodes(graphType, numOfColumns);

    // Check if the number of nodes is within the expected range
    EXPECT_GE(nodes.size(), numOfColumns); // Minimum number of nodes should be equal to the number of columns
    EXPECT_LE(nodes.size(), numOfColumns * 6); // Maximum number of nodes based on the provided logic

    // Check if all nodes are unique
    QSet<Node*> uniqueNodes;
    for (Node* node : nodes) {
        uniqueNodes.insert(node);
    }
    EXPECT_EQ(nodes.size(), uniqueNodes.size());

    // Check if the nodes have unique labels
    QSet<char> uniqueLabels;
    for (Node* node : nodes) {
        uniqueLabels.insert(node->getName());
    }
    EXPECT_EQ(nodes.size(), uniqueLabels.size());

    // Check if the first and last nodes have the correct color
    QColor startEndNodeColour = QColor("#09814A");
    EXPECT_EQ(nodes.first()->getNodeColour(), startEndNodeColour);
    EXPECT_EQ(nodes.last()->getNodeColour(), startEndNodeColour);
}

// Test generating edges for uniqueness and constraints
TEST_F(WidgetTest, GenerateEdgesUniqueAndConstraintsTest) {
    int graphType = 0; // Assuming a specific graph type for this test
    int numOfColumns = 4; // Define the number of columns for the test

    // Generate nodes
    QList<Node*> nodes = widget->generateNodes(graphType, numOfColumns);
    ASSERT_FALSE(nodes.isEmpty());

    // Generate edges
    QList<Edge*> edges = widget->generateEdges(nodes, graphType);

    // Check if edges are not empty
    EXPECT_FALSE(edges.isEmpty());

    // Check if all edges are unique
    QSet<Edge*> uniqueEdges;
    for (Edge* edge : edges) {
        uniqueEdges.insert(edge);
    }
    EXPECT_EQ(edges.size(), uniqueEdges.size());

    // Check if all edges have unique source and destination nodes
    QSet<QString> uniqueEdgeNames;
    for (Edge* edge : edges) {
        QString edgeName = edge->getName();
        uniqueEdgeNames.insert(edgeName);
        EXPECT_NE(edge->sourceNode(), edge->destNode());
    }
    EXPECT_EQ(edges.size(), uniqueEdgeNames.size());

    // Check if all edges have weights
    for (Edge* edge : edges) {
        EXPECT_GT(edge->getWeight(), 0);
    }

    // Check if all edges have labels
    for (Edge* edge : edges) {
        EXPECT_FALSE(edge->getName().isEmpty());
    }
}

// Test removing edges with high intersections
TEST_F(WidgetTest, RemoveEdgesWithHighIntersectionsTest) {
    int graphType = 0; // Assuming a specific graph type for this test
    int numOfColumns = 4; // Define the number of columns for the test

    // Generate nodes and edges
    QList<Node*> nodes = widget->generateNodes(graphType, numOfColumns);
    QList<Edge*> edges = widget->generateEdges(nodes, graphType);

    // Count the number of intersections for each edge before removal
    QHash<Edge*, int> initialEdgeIntersections;
    for (Edge* edge : edges) {
        int intersections = widget->countIntersectionsForEdge(edge, edges);
        initialEdgeIntersections.insert(edge, intersections);
    }

    // Set an intersection limit and remove edges with intersections above the limit
    int intersectionLimit = 2;
    widget->removeEdgesWithHighIntersections(edges, intersectionLimit);

    // Verify that edges with intersections above the limit have been removed
    for (Edge* edge : initialEdgeIntersections.keys()) {
        if (initialEdgeIntersections.value(edge) >= intersectionLimit) {
            EXPECT_FALSE(edges.contains(edge));
        }
    }

    // Verify that remaining edges have intersections below or equal to the limit
    for (Edge* edge : edges) {
        int intersections = widget->countIntersectionsForEdge(edge, edges);
        EXPECT_LE(intersections, intersectionLimit);
    }

    // Check if the remaining edges are still valid and unique
    QSet<Edge*> uniqueEdges(edges.begin(), edges.end());
    EXPECT_EQ(edges.size(), uniqueEdges.size());
}

// Test removing node-intersecting edges
TEST_F(WidgetTest, RemoveNodeIntersectingEdgesTest) {
    int graphType = 0; // Assuming a specific graph type for this test
    int numOfColumns = 4; // Define the number of columns for the test

    // Generate nodes and edges
    QList<Node*> nodes = widget->generateNodes(graphType, numOfColumns);
    QList<Edge*> edges = widget->generateEdges(nodes, graphType);

    // Store the initial list of edges
    QList<Edge*> initialEdges = edges;

    // Remove edges that intersect with nodes
    widget->removeNodeIntersectingEdges(nodes, edges);

    // Define the threshold distance for intersection
    qreal threshold = 40.0;

    // Verify that edges intersecting with nodes (except their source and destination nodes) have been removed
    for (Edge* edge : initialEdges) {
        QLineF edgeLine(edge->sourcePoint, edge->destPoint);
        bool intersectsNode = false;

        for (Node* node : nodes) {
            if (node == edge->sourceNode() || node == edge->destNode()) {
                continue; // Skip this node, as it's part of the edge
            }

            for (qreal step = 0.0; step <= 1; step += 0.1) {
                qreal dist = QLineF(node->pos(), edgeLine.pointAt(step)).length();
                if (dist < threshold) {
                    intersectsNode = true;
                    break;
                }
            }

            if (intersectsNode) {
                break;
            }
        }

        if (intersectsNode) {
            EXPECT_FALSE(edges.contains(edge));
        } else {
            EXPECT_TRUE(edges.contains(edge));
        }
    }

    // Check if the remaining edges are still valid and unique
    QSet<Edge*> uniqueEdges(edges.begin(), edges.end());
    EXPECT_EQ(edges.size(), uniqueEdges.size());
}

// Test highlighting the shortest path
TEST_F(WidgetTest, HighlightShortestPathTest) {
    QList<Node*> nodes = widget->generateNodes(0, 4);
    QList<Edge*> edges = widget->generateEdges(nodes, 0);
    std::stack<Edge*> shortestPath = widget->dijkstrasAlgorithm(nodes.first(), nodes.last(), nodes, edges);
    widget->shortestPath = shortestPath;  // Set the global shortestPath
    widget->highlightShortestPath(Qt::green);

    while (!shortestPath.empty()) {
        Edge* edge = shortestPath.top();
        shortestPath.pop();
        EXPECT_EQ(edge->getEdgeColour(), Qt::green);
    }
}
