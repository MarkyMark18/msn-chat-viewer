#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // File menu contents
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::handleOpenAction);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleOpenAction() {
    // Open the file browser window, restricted to only .xml files
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        QDir::homePath(),
        tr("XML Files (*.xml)")
        );

    // Once file is selected, load the contents
}
