#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

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

    QString xmlData = file.readAll();

    // Pass the XML data to the parser to format it for display
    parseXML(xmlData);

}

void MainWindow::parseXML(QString data) {

    int index = 0;

    while (index < data.length()) {

        // Get the index of the next start tag
        int tagStart = data.indexOf('<', index);

        // Check if there are no more tags left to parse
        if (tagStart == -1) {
            break;
        }

        // Get the index of the end of the tag
        int tagEnd = data.indexOf('>', tagStart);

        // // Retrieve the contents of the tag (unless it's a closing tag)
        // if (data[tagStart + 1] != '/') {

        //     int tagLength = tagEnd - (tagStart + 1);

        //     QString tag = data.mid(tagStart + 1, tagLength);

        //     ui->xmlViewer->append(tag);

        // }

        // Retrieve the text contents of the tag and trim it
        int contentLength = data.indexOf('<', tagEnd) - (tagEnd + 1);
        QString textContent = data.mid(tagEnd + 1, contentLength).trimmed();

        // Output the
        if (!textContent.isEmpty()) {
            ui->xmlViewer->append(textContent + "\n");
        }

        index = tagEnd + 1;

    }

}

void MainWindow::loadXMLToBrowser(QString data) {

}
