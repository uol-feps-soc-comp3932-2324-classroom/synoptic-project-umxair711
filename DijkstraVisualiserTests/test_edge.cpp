#include "edge.h"
#include "node.h"
#include <gtest/gtest.h>

// Test fixture for Edge class
class EdgeTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize common objects before each test
        nodeA = new Node('A', 0);
        nodeB = new Node('B', 1);
        nodeC = new Node('C', 2);
        nodeD = new Node('D', 3);

        // Set positions for nodes to create intersecting edges
        nodeA->setPos(0, 0);
        nodeB->setPos(100, 100);
        nodeC->setPos(100, 0);
        nodeD->setPos(0, 100);

        edge1 = new Edge(nodeA, nodeB, true, 5);
        edge2 = new Edge(nodeC, nodeD, false, 8);
    }

    void TearDown() override {
        // Clean up common objects after each test
        delete edge1;
        delete edge2;
        delete nodeA;
        delete nodeB;
        delete nodeC;
        delete nodeD;
    }

    // Test objects
    Node* nodeA;
    Node* nodeB;
    Node* nodeC;
    Node* nodeD;
    Edge* edge1;
    Edge* edge2;
};

// Test constructor and getters
TEST_F(EdgeTest, ConstructorAndGetters) {
    ASSERT_EQ(edge1->sourceNode(), nodeA);
    ASSERT_EQ(edge1->destNode(), nodeB);
    ASSERT_EQ(edge1->getName(), "AB");
    ASSERT_EQ(edge1->getWeight(), 5);
    ASSERT_TRUE(edge1->isDirected());

    ASSERT_EQ(edge2->sourceNode(), nodeC);
    ASSERT_EQ(edge2->destNode(), nodeD);
    ASSERT_EQ(edge2->getName(), "CD");
    ASSERT_EQ(edge2->getWeight(), 8);
    ASSERT_FALSE(edge2->isDirected());
}

// Test edge colour setter and getter
TEST_F(EdgeTest, SetEdgeColourAndGetEdgeColour) {
    edge1->setEdgeColour(Qt::red);
    ASSERT_EQ(edge1->getEdgeColour(), Qt::red);

    edge2->setEdgeColour(Qt::blue);
    ASSERT_EQ(edge2->getEdgeColour(), Qt::blue);
}

// Test intersecting edges
TEST_F(EdgeTest, IntersectingEdges) {
    // The edges should intersect
    ASSERT_TRUE(edge1->intersects(*edge2));
    ASSERT_TRUE(edge2->intersects(*edge1));
}

