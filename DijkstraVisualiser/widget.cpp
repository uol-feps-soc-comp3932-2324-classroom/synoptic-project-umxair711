#include "widget.h"
#include "QtWidgets/qradiobutton.h"
#include "edge.h"
#include "node.h"
#include "ui_widget.h"
#include <QGraphicsScene>
#include <QThread>
#include <QRadioButton>
#include <QRandomGenerator>
#include <QWheelEvent>
#include <QScrollBar>
#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include <QTimer>
#include <QList>
#include <queue>
#include <map>
#include <stack>
#include <algorithm>


// Constructor for the Widget class
Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this); // Set up the user interface as defined in the .ui file

    // Initialize the label array with alphabet characters for node labels
    for (int i = 0; i < 26; i++){
        char letter = 'A' + i;
        labels[i] = letter;
    }

    // Create a graphics scene for displaying the graph
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, sceneWidth, sceneHeight); // Set the dimensions of the scene
    ui->graphicsView->setScene(scene); // Assign the scene to the graphics view widget

    ui->helpText->setHidden(true); // Initially hide the help text

    // Connect UI elements to corresponding event handlers
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Widget::on_nextGraphButton_clicked);
    connect(ui->directedCheckBox, QOverload<int>::of(&QCheckBox::stateChanged), this, &Widget::on_nextGraphButton_clicked);

    // Generate the initial graph based on the current selection in the combo box
    generateGraph(ui->comboBox->currentIndex());
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_helpButton_clicked()
{
    if (ui->helpText->isHidden()){
        ui->helpText->setHidden(false);
    }
    else {
        ui->helpText->setHidden(true);
    }
}


// Function to handle the submit button click event
void Widget::on_submitButton_clicked() {
    // Loop through all items in the vertical layout
    for (int i = 0; i < ui->verticalLayout->count(); i++) {
        QLayoutItem *item = ui->verticalLayout->itemAt(i); // Get the layout item at index i
        // Check if the item is a widget and it's a radio button
        if (item && item->widget() && item->widget()->isWidgetType()) {
            QRadioButton *answer = qobject_cast<QRadioButton *>(item->widget()); // Cast the widget to a radio button
            // Check if the radio button exists and is checked
            if (answer && answer->isChecked()) {
                // Check if the selected answer is correct
                if (correctAnswer == answer->text()) {
                    // Correct answer
                    ui->resultLabel->setText("Correct!"); // Set the result label text to "Correct!"
                    ui->submitButton->setDisabled(true); // Disable the submit button
                    ui->verticalLayout->setEnabled(false); // Disable the vertical layout
                    ui->nextGraphButton->setEnabled(true); // Enable the next graph button
                    questionsCorrect++; // Increment the number of correct answers
                    questionsAttempted++; // Increment the number of attempted questions
                    // Update the score label with the number of correct and attempted questions
                    ui->scoreLabel->setText(QString("%1/%2").arg(questionsCorrect).arg(questionsAttempted));
                    highlightShortestPath(QColor("#53A548")); // Highlight the shortest path in green
                }
                else {
                    // Incorrect answer
                    // Set the result label text to indicate the correct answer
                    ui->resultLabel->setText(QString("Incorrect! The answer is %1").arg(correctAnswer));
                    ui->submitButton->setDisabled(true); // Disable the submit button
                    ui->verticalLayout->setEnabled(false); // Disable the vertical layout
                    ui->nextGraphButton->setEnabled(true); // Enable the next graph button
                    questionsAttempted++; // Increment the number of attempted questions
                    // Update the score label with the number of correct and attempted questions
                    ui->scoreLabel->setText(QString("%1/%2").arg(questionsCorrect).arg(questionsAttempted));
                    highlightShortestPath(Qt::red); // Highlight the shortest path in red
                }
            }
        }
    }
}


// Function to handle the next graph button click event
void Widget::on_nextGraphButton_clicked() {
    try {
        resetScreen(); // Reset the screen layout
        ui->submitButton->setDisabled(false); // Enable the submit button
        ui->nextGraphButton->setDisabled(true); // Disable the next graph button until the next graph is generated
        generateGraph(ui->comboBox->currentIndex()); // Generate the next graph based on the selected index in the combo box
    }
    catch (const std::exception &e) {
        // Handle any exceptions that occur during graph generation
        qDebug() << "Exception occurred: " << e.what(); // Log the exception message
    }
}


// Function to reset the screen and clear all displayed content
void Widget::resetScreen() {
    // Create a new graphics scene for rendering the graph
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, sceneWidth, sceneHeight); // Set the dimensions of the scene
    ui->graphicsView->setScene(scene); // Assign the scene to the graphics view widget

    // Clear all items from the vertical layout
    QLayoutItem *child;
    while ((child = ui->verticalLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            delete child->widget(); // Delete any widgets contained in the layout
        }
        delete child; // Delete the layout item itself
    }

    ui->verticalLayout->setEnabled(true); // Enable the vertical layout
    ui->resultLabel->clear(); // Clear the result label text
    ui->textBrowser->clear(); // Clear the text browser content
}


// Function to generate a new graph based on the selected graph type
void Widget::generateGraph(int graphType) {
    int numOfColumns; // Variable to store the number of columns in the graph
    QList<Node *> allNodes; // List to store all nodes in the graph
    QList<Edge *> allEdges; // List to store all edges in the graph

    do {
        // Determine the number of columns based on the graph type
        numOfColumns = graphType == 0 ? QRandomGenerator::global()->bounded(3, 5) : QRandomGenerator::global()->bounded(4, 7);

        // Generate nodes for the graph
        allNodes = generateNodes(graphType, numOfColumns);

        // Generate edges for the graph
        allEdges = generateEdges(allNodes, graphType);

        // Remove edges with high intersections to improve graph readability
        removeEdgesWithHighIntersections(allEdges, 2);

        // Remove nodes intersecting with edges to improve graph layout
        removeNodeIntersectingEdges(allNodes, allEdges);

        // Find the shortest path in the graph using Dijkstra's algorithm
        shortestPath = dijkstrasAlgorithm(allNodes.first(), allNodes.last(), allNodes, allEdges);
    } while (shortestPath.size() < 2); // Repeat until a valid shortest path is found

    // Generate a question based on the shortest path
    generateQuestion(shortestPath, allNodes, allEdges);

    // Add nodes and edges to the graphics scene
    for (Node *n : allNodes) {
        ui->graphicsView->scene()->addItem(n);
    }
    for (Edge *e : allEdges) {
        ui->graphicsView->scene()->addItem(e);
    }
}


// Function to generate nodes for the graph
QList<Node *> Widget::generateNodes(int graphType, const int numOfColumns) {
    int numOfNodes = 0; // Variable to keep track of the total number of nodes generated
    QList<Node *> allNodes; // List to store all generated nodes
    qreal xBase = (sceneWidth - 20) / (numOfColumns + 1); // Calculate the base x-coordinate for node placement

    // Loop through each column to generate nodes
    for (int i = 0; i < numOfColumns; i++) {
        qreal x = xBase * (i + 1); // Calculate the x-coordinate for the current column

        // Determine the minimum and maximum number of nodes for the current column
        int minNodes = (i == 0 || i == (numOfColumns - 1)) ? 1 : 2;
        int maxNodes = (i == 0 || i == (numOfColumns - 1)) ?
                           (graphType == 0 ? 3 : 4) : (graphType == 0 ? 4 : 6);

        // Generate a random number of nodes for the current column within the specified range
        int nodesInColumn = QRandomGenerator::global()->bounded(minNodes, maxNodes);

        // Calculate the base y-coordinate for node placement in the current column
        qreal yBase = (sceneHeight - 20) / (nodesInColumn + 1);

        // Loop through each node in the current column
        for (int j = 0; j < nodesInColumn; j++) {
            qreal y = yBase * (j + 1); // Calculate the y-coordinate for the current node
            Node *newNode = new Node(labels[numOfNodes], i); // Create a new node with a label and column index
            allNodes.append(newNode); // Add the new node to the list of all nodes
            numOfNodes++; // Increment the total number of nodes generated

            qreal perturb = QRandomGenerator::global()->bounded(-15, 15); // Generate a random perturbation value for node positioning
            newNode->setPos(x + perturb, y + perturb); // Set the position of the new node with perturbation
        }
    }

    // Set the color of the start and end nodes
    QColor startEndNodeColour = QColor("#09814A");
    allNodes.first()->setNodeColour(startEndNodeColour); // Set color for the first node
    allNodes.last()->setNodeColour(startEndNodeColour); // Set color for the last node

    return allNodes; // Return the list of all generated nodes
}


// Function to generate edges for the graph
QList<Edge *> Widget::generateEdges(QList<Node *> allNodes, const int graphType) {
    // Check if the graph is directed
    bool directed = ui->directedCheckBox->isChecked();

    // List to store all generated edges
    QList<Edge *> allEdges;

    // Loop through each pair of adjacent nodes to create edges between them
    for (int i = 0; i < allNodes.size() - 1; i++) {
        Node *node1 = allNodes[i];
        Node *node2 = allNodes[i + 1];

        // Determine if the edge is directed based on the graph type and user input
        bool directedProb = false;
        if (directed) {
            directedProb = graphType == 0 ? (QRandomGenerator::global()->bounded(1, 11) > 7 ? true : false)
                                          : (QRandomGenerator::global()->bounded(1, 11) > 4 ? true : false);
        }

        // Generate a random weight for the edge
        int weight = QRandomGenerator::global()->bounded(1, 15);

        // Create a new edge between the current pair of nodes
        Edge *newEdge = new Edge(node1, node2, directedProb, weight);
        allEdges.append(newEdge);
    }

    // Loop through each node to potentially create additional edges
    Node *node1;
    Node *node2;
    for (Node *node : allNodes) {
        node1 = node;

        QString potEdge;
        QString potEdgeRev;
        QList<Node *> nodesOfExistingEdges;
        bool edgeExists = false;
        bool fullGraph = false;

        // Loop until all possible edges are created or the graph is full
        do {
            potEdge.clear();
            potEdgeRev.clear();

            int count = 0;
            for (Node *n : allNodes) {
                if (n->getName() == node1->getName() || nodesOfExistingEdges.contains(n)) {
                    count++;
                }
            }
            if (count == allNodes.size()) {
                fullGraph = true;
                break;
            }

            // Generate potential edges until a new edge can be created
            do {
                node2 = allNodes[QRandomGenerator::global()->bounded(allNodes.size())];
            } while (node1->getName() == node2->getName() || nodesOfExistingEdges.contains(node2));
            qreal shortestDist = QLineF(node1->pos(), node2->pos()).length();
            for (Node *n : allNodes) {
                if (n->getName() == node1->getName() || nodesOfExistingEdges.contains(n)) {
                    continue;
                }
                qreal dist = QLineF(node1->pos(), n->pos()).length();
                if (dist < shortestDist) {
                    shortestDist = dist;
                    node2 = n;
                }
            }

            // Create the potential edge
            potEdge.append(node1->getName());
            potEdge.append(node2->getName());
            for (int i = potEdge.length() - 1; i >= 0; --i) {
                potEdgeRev.append(potEdge[i]);
            }

            // Check if the potential edge already exists
            edgeExists = false;
            for (Edge *e : allEdges) {
                if (e->getName().compare(potEdge) == 0 || e->getName().compare(potEdgeRev) == 0) {
                    edgeExists = true;
                    nodesOfExistingEdges.append(node2);
                    break;
                }
            }
        } while (edgeExists);

        // Break the loop if the graph is full
        if (fullGraph) {
            break;
        } else {
            // Determine if the edge is directed based on the graph type and user input
            bool directedProb = false;
            if (directed) {
                directedProb = graphType == 0 ? (QRandomGenerator::global()->bounded(1, 11) > 7 ? true : false)
                                              : (QRandomGenerator::global()->bounded(1, 11) > 4 ? true : false);
            }

            // Generate a random weight for the edge
            int weight = QRandomGenerator::global()->bounded(1, 15);

            // Create a new edge
            Edge *newEdge = new Edge(node1, node2, directedProb, weight);
            allEdges.append(newEdge);
        }
    }

    return allEdges; // Return the list of all generated edges
}


// Function to remove edges with intersections above a limit
void Widget::removeEdgesWithHighIntersections(QList<Edge*>& allEdges, int intersectionLimit) {
    bool edgesRemoved;
    do {
        edgesRemoved = false;

        // Create a map of edges and their intersection counts
        QHash<Edge*, int> edgeIntersections;
        for (Edge* edge : allEdges) {
            int intersections = countIntersectionsForEdge(edge, allEdges);
            edgeIntersections.insert(edge, intersections);
        }

        // Create a list of edges sorted by intersection count in descending order
        QList<Edge*> sortedEdges = edgeIntersections.keys();
        std::sort(sortedEdges.begin(), sortedEdges.end(), [&edgeIntersections](Edge* a, Edge* b) {
            return edgeIntersections.value(a) > edgeIntersections.value(b);
        });

        // Iterate through the sorted edges and remove edges with intersections above the limit
        for (Edge* edge : sortedEdges) {
            if (edgeIntersections.value(edge) >= intersectionLimit) {
                allEdges.removeOne(edge);
                delete edge;
                edgesRemoved = true;
                break; // Restart the process after removing an edge
            }
        }
    } while (edgesRemoved);
}


// Function to remove edges intersecting with nodes
void Widget::removeNodeIntersectingEdges(QList<Node *> allNodes, QList<Edge *> &allEdges) {
    // Define the threshold distance
    qreal threshold = 40.0;

    // List to store edges to remove
    QList<Edge *> edgesToRemove;

    // Iterate through each edge
    for (Edge *edge : allEdges) {
        // Get the line representing the edge
        QLineF edgeLine(edge->getSourcePoint(), edge->getDestPoint());

        // Iterate through each node
        for (Node *node : allNodes) {
            // Check if the edge's start or end node is the current node
            if (node == edge->sourceNode() || node == edge->destNode()) {
                continue; // Skip this node, as it's part of the edge
            }

            // Iterate along the edge to check for intersection with the node
            for (qreal step = 0.0; step <= 1; step += 0.1) {
                qreal dist = QLineF(node->pos(), edgeLine.pointAt(step)).length();
                if (dist < threshold) {
                    if (!edgesToRemove.contains(edge)) {
                        edgesToRemove.append(edge);
                    }
                    break; // Move to the next edge
                }
            }
        }
    }

    // Remove edges intersecting with nodes
    if (!edgesToRemove.isEmpty()) {
        // Remove edges from allEdges and delete them
        for (Edge *edge : std::as_const(edgesToRemove)) {
            allEdges.removeOne(edge);
            delete edge;
        }
    }
}


// Dijkstra's algorithm to find the shortest path
std::stack<Edge*> Widget::dijkstrasAlgorithm(Node* startNode, Node* endNode, const QList<Node*>& allNodes, const QList<Edge*>& allEdges) {
    // Initialize distances map and predecessors map
    std::map<Node*, int> distances;
    std::map<Node*, Node*> predecessors;
    for (Node* node : allNodes) {
        distances[node] = std::numeric_limits<int>::max(); // Initialize all distances to infinity
        predecessors[node] = nullptr; // Initialize all predecessors to nullptr
    }

    // Set distance of the start node to 0
    distances[startNode] = 0;

    // Create a priority queue to store nodes based on their distances
    std::priority_queue<std::pair<int, Node*>, std::vector<std::pair<int, Node*>>, std::greater<std::pair<int, Node*>>> pq;

    // Push the start node into the priority queue
    pq.push({0, startNode});

    // Main Dijkstra's algorithm loop
    while (!pq.empty()) {
        // Get the node with the minimum distance from the priority queue
        Node* currNode = pq.top().second;
        int currDist = pq.top().first;
        pq.pop();

        // Check if the current node has already been visited
        if (currDist > distances[currNode]) {
            continue; // Skip if already visited
        }

        // Visit all neighbors of the current node
        for (Edge* edge : allEdges) {
            if (edge->sourceNode() == currNode || (!edge->isDirected() && edge->destNode() == currNode)) {
                Node* neighbor = (edge->sourceNode() == currNode) ? edge->destNode() : edge->sourceNode();
                int edgeWeight = edge->getWeight();
                int newDist = currDist + edgeWeight;

                // Update distance if a shorter path is found
                if (newDist < distances[neighbor]) {
                    distances[neighbor] = newDist;
                    predecessors[neighbor] = currNode; // Update predecessor
                    pq.push({newDist, neighbor});
                }
            }
        }
    }

    // Stack to store the shortest path
    std::stack<Edge*> shortestPath;

    // Backtrack from the end node to the start node to find the shortest path
    Node* currentNode = endNode;
    Node* prevNode = nullptr;
    while (currentNode != nullptr && predecessors[currentNode] != nullptr) {
        // Find the edge between currentNode and its predecessor
        for (Edge* edge : allEdges) {
            if ((edge->sourceNode() == currentNode && edge->destNode() == predecessors[currentNode]) ||
                (edge->sourceNode() == predecessors[currentNode] && edge->destNode() == currentNode)) {
                shortestPath.push(edge);
                break;
            }
        }
        prevNode = currentNode;
        currentNode = predecessors[currentNode];
    }

    return shortestPath;
}


// Function that handles the generation of the question components
void Widget::generateQuestion(std::stack<Edge *> shortestPath, const QList<Node *> allNodes, const QList<Edge *> &allEdges) {
    // Print the graph representation in the text browser
    printGraphRepresentation(allNodes, allEdges);

    // Construct the correct answer
    QString rightAnswer = "A";
    char prevNode = 'A';
    while (!shortestPath.empty()) {
        char nextNode = shortestPath.top()->destNode()->getName() != prevNode ?
                            shortestPath.top()->destNode()->getName() : shortestPath.top()->sourceNode()->getName();
        rightAnswer.append(nextNode);
        shortestPath.pop();
        prevNode = nextNode;
    }
    correctAnswer = rightAnswer;

    // Find alternative paths and shuffle them
    QList<QString> alternativePaths = findAllPaths(rightAnswer, allNodes.first(), allNodes.last(), allEdges);
    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(alternativePaths.begin(), alternativePaths.end(), rng);

    // Determine the number of choices
    int numberOfChoices = alternativePaths.size() <= 3 ? alternativePaths.size() + 1 : 5;
    int answerButtonIndex = QRandomGenerator::global()->bounded(0, numberOfChoices - 1);
    bool answerButtonCreated = false;

    // Add answer options to the layout
    for (int i = 0; i < numberOfChoices - 1; i+=0) {
        if (i == answerButtonIndex && !answerButtonCreated) {
            QRadioButton *radioButton = new QRadioButton(rightAnswer, this);
            radioButton->setFont(QFont("Didot", 15));
            ui->verticalLayout->addWidget(radioButton);
            // Connect the correct answer button
            connect(radioButton, &QRadioButton::clicked, this, [=]() {
                correctAnswer = rightAnswer;
            });
            answerButtonCreated = true;
        } else {
            QRadioButton *radioButton = new QRadioButton(alternativePaths[i], this);
            radioButton->setFont(QFont("Didot", 15));
            ui->verticalLayout->addWidget(radioButton);
            i++;
        }
    }
}


// Function that generates the adjacency matrix representation of the graph
void Widget::printGraphRepresentation(const QList<Node*>& allNodes, const QList<Edge*>& allEdges) {
    int numNodes = allNodes.size();
    QVector<QVector<int>> adjacencyMatrix(numNodes, QVector<int>(numNodes, 0));

    // Fill the adjacency matrix with edge weights
    for (Edge* edge : allEdges) {
        int sourceIndex = allNodes.indexOf(edge->sourceNode());
        int destIndex = allNodes.indexOf(edge->destNode());
        adjacencyMatrix[sourceIndex][destIndex] = edge->getWeight();
        if (!edge->isDirected()) {
            adjacencyMatrix[destIndex][sourceIndex] = edge->getWeight();
        }
    }

    // Create the HTML table for the adjacency matrix
    QString graphRepresentation = "<html><head><style>"
                                  "table { border-collapse: collapse; }"
                                  "th, td { border: 1px solid black; padding: 5px; text-align: center; }"
                                  "th { background-color: #f2f2f2; }"
                                  ".red-text { color: red; }"
                                  "</style></head><body>";

    // Add introductory text
    graphRepresentation += "<p>The adjacency matrix for the graph G = (V, E) is shown below:</p>";

    // Add table headers
    graphRepresentation += "<table><tr><th></th>";
    for (Node* node : allNodes) {
        graphRepresentation += "<th>" + QString(node->getName()) + "</th>";
    }
    graphRepresentation += "</tr>";

    // Add table rows
    for (int i = 0; i < numNodes; ++i) {
        graphRepresentation += "<tr><th>" + QString(allNodes[i]->getName()) + "</th>";
        for (int j = 0; j < numNodes; ++j) {
            if (adjacencyMatrix[i][j] != 0) {
                graphRepresentation += "<td class='red-text'>" + QString::number(adjacencyMatrix[i][j]) + "</td>";
            } else {
                graphRepresentation += "<td>" + QString::number(adjacencyMatrix[i][j]) + "</td>";
            }
        }
        graphRepresentation += "</tr>";
    }

    graphRepresentation += "</table></body></html>";

    // Display the adjacency matrix in the QTextBrowser
    ui->textBrowser->setHtml(graphRepresentation);
}


// Function that finds all the alternate paths for distractors using DFS
QList<QString> Widget::findAllPaths(const QString& shortestPath, Node* startNode, Node* endNode, const QList<Edge*>& allEdges) {
    // Perform DFS to find all paths from startNode to endNode
    QList<QList<Node*>> allPaths = dfs(startNode, endNode, allEdges);

    // Convert paths to QStrings and filter out paths based on length
    QList<QString> alternatePaths;
    int shortestPathLength = shortestPath.length();

    for (const auto& path : allPaths) {
        // Check if the path length matches the criteria
        int pathLength = path.size();
        if (pathLength == shortestPathLength || pathLength == shortestPathLength - 1 || pathLength == shortestPathLength + 1) {
            QString pathString;
            for (Node* node : path) {
                pathString += node->getName();
            }
            alternatePaths.append(pathString);
        }
    }

    // Remove the shortest path from the list of alternative paths
    alternatePaths.removeAll(shortestPath);

    return alternatePaths;
}


// DFS algorithm to find all paths between start and end node
QList<QList<Node*>> Widget::dfs(Node* startNode, Node* endNode, const QList<Edge*>& allEdges) {
    QList<QList<Node*>> allPaths;
    QList<Node*> currentPath;
    QSet<Node*> visited;

    // Helper lambda function for DFS recursion
    std::function<void(Node*)> dfsRecursive = [&](Node* currentNode) {
        // Mark the current node as visited
        visited.insert(currentNode);

        // Add current node to the current path
        currentPath.append(currentNode);

        // If the current node is the end node, add the current path to allPaths
        if (currentNode == endNode) {
            allPaths.append(currentPath);
        } else {
            // Continue DFS to its neighbors
            for (Edge* edge : allEdges) {
                if (edge->sourceNode() == currentNode || (!edge->isDirected() && edge->destNode() == currentNode)) {
                    Node* neighbor = (edge->sourceNode() == currentNode) ? edge->destNode() : edge->sourceNode();
                    if (!visited.contains(neighbor)) {
                        dfsRecursive(neighbor);
                    }
                }
            }
        }

        // Backtrack: remove the current node from the current path and mark it as unvisited
        currentPath.removeLast();
        visited.remove(currentNode);
    };

    // Start DFS from the start node
    dfsRecursive(startNode);

    return allPaths;
}


// Function that counts all the intersections that a given edge has
int Widget::countIntersectionsForEdge(const Edge* edgeToCheck, const QList<Edge*>& allEdges) {
    int intersectionCount = 0;

    // Check the provided edge against all other edges
    for (Edge* otherEdge : allEdges) {
        if (edgeToCheck != otherEdge) {
            // Check for intersection with the provided edge
            if (edgeToCheck->intersects(*otherEdge)) {
                intersectionCount++;
            }
        }
    }
    // Return the intersection count for the provided edge
    return intersectionCount;
}


// Function that iterates through edges and ndes part of the shortest path and changes their colour to highlight
void Widget::highlightShortestPath(QColor colour) {
    // Create a set to store visited nodes
    QSet<Node*> visitedNodes;

    // Iterate through the shortest path and highlight edges and nodes
    std::stack<Edge*> path = shortestPath;
    while (!path.empty()) {
        Edge* edge = path.top();
        path.pop();

        // Highlight edge
        edge->setEdgeColour(colour);

        // Highlight source and destination nodes
        visitedNodes.insert(edge->sourceNode());
        visitedNodes.insert(edge->destNode());
    }

    // Highlight nodes that are part of the shortest path
    for (Node* node : visitedNodes) {
        node->setNodeColour(colour);
    }
}


// Wheel event function that controls zoom and traversal of graph display area
void Widget::wheelEvent(QWheelEvent *event) {
    // Get the cursor position in the scene coordinates
    QPointF oldPos = ui->graphicsView->mapToScene(event->position().toPoint());

    // Get the angle delta
    int angle = event->angleDelta().y();

    // Define minimum and maximum scale factors
    qreal minScale = 0.5;
    qreal maxScale = 2.0;

    // Calculate the scaling factor
    qreal scaleFactor;
    if (angle > 0) {
        // Zoom in
        scaleFactor = 1.0 + angle / 1000.0;
        if (ui->graphicsView->transform().m11() * scaleFactor > maxScale) {
            scaleFactor = maxScale / ui->graphicsView->transform().m11();
        }
    } else {
        // Zoom out
        scaleFactor = 1.0 / (1.0 - angle / 1000.0);
        if (ui->graphicsView->transform().m11() * scaleFactor < minScale) {
            scaleFactor = minScale / ui->graphicsView->transform().m11();
        }
    }

    // Apply the scaling factor
    ui->graphicsView->scale(scaleFactor, scaleFactor);

    // Get the new position after scaling
    QPointF newPos = ui->graphicsView->mapToScene(event->position().toPoint());

    // Adjust the view to keep the cursor fixed
    ui->graphicsView->translate(newPos.x() - oldPos.x(), newPos.y() - oldPos.y());

    event->accept();
}


