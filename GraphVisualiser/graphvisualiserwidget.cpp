#include "graphvisualiserwidget.h"
#include "ui_graphvisualiserwidget.h"

GraphVisualiserWidget::GraphVisualiserWidget(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GraphVisualiserWidget)
{
    ui->setupUi(this);
}

GraphVisualiserWidget::~GraphVisualiserWidget()
{
    delete ui;
}
