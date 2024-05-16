#include <gtest/gtest.h>
#include <QList>
#include <QStack>
#include <QApplication>
#include <QColor>
#include "node.h"
#include "edge.h"


// Test fixture for Node class
class NodeTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize common objects before each test
        nodeA = new Node('A', 0);
        nodeB = new Node('B', 1);
    }

    void TearDown() override {
        // Clean up common objects after each test
        delete nodeA;
        delete nodeB;
    }

    // Test objects
    Node* nodeA;
    Node* nodeB;
};

// Test constructor and getters
TEST_F(NodeTest, ConstructorAndGetters) {
    ASSERT_EQ(nodeA->getName(), 'A');
    ASSERT_EQ(nodeA->getCol(), 0);

    ASSERT_EQ(nodeB->getName(), 'B');
    ASSERT_EQ(nodeB->getCol(), 1);
}

// Test node colour setter
TEST_F(NodeTest, SetNodeColour) {
    nodeA->setNodeColour(Qt::red);
    ASSERT_EQ(nodeA->getNodeColour(), Qt::red);

    nodeB->setNodeColour(Qt::blue);
    ASSERT_EQ(nodeB->getNodeColour(), Qt::blue);
}

// Test shape function
TEST_F(NodeTest, Shape) {
    QPainterPath expectedPath;
    expectedPath.addEllipse(-15, -15, 30, 30);

    ASSERT_EQ(nodeA->shape(), expectedPath);
    ASSERT_EQ(nodeB->shape(), expectedPath);
}

// Test boundingRect function
TEST_F(NodeTest, BoundingRect) {
    QRectF expectedRect(-12, -12, 25, 25);

    ASSERT_EQ(nodeA->boundingRect(), expectedRect);
    ASSERT_EQ(nodeB->boundingRect(), expectedRect);
}

