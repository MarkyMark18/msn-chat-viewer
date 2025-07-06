#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "parser.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

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
    // Open the file browser popup, restricted to only .xml files
    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        QDir::homePath(),
        tr("XML Files (*.xml)")
    );

    // Once file is selected, load the contents
    if (filePath.isEmpty()) {
        return;
    }

    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    // Extract the raw XML data from the file
    QString rawXMLData = file.readAll();

    // Create an instance of the parser
    Parser parser;

    // Pass the raw XML data to the parser to get a list of ChatMessages
    std::vector<ChatMessage> messageList = parser.parseXML(rawXMLData);

}

