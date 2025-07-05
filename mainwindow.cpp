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

    int rawXMLIndex = 0;
    QString messageStartText = "<Message";
    QString messageEndText = "</Message>";

    std::vector<ChatMessage> messageList;

    while (rawXMLIndex < data.length()) {

        // Get the start index of the next message
        int messageStartIndex = data.indexOf(messageStartText, rawXMLIndex);

        // Check if there are no more messages left to parse
        if (messageStartIndex == -1) {
            break;
        }

        // Offset the messageStartIndex to remove "<Message"
        messageStartIndex += messageStartText.length();

        // Get the end index of the message and it's length
        int messageEndIndex = data.indexOf("</Message>", messageStartIndex) - 1;
        int messageLength = messageEndIndex - messageStartIndex;

        // Retrieve the contents of the tag (unless it's a closing tag)
        QString rawMessageContent = data.mid(messageStartIndex, messageLength).trimmed();

        // Output the text content to the viewer
        if (!rawMessageContent.isEmpty()) {
            ui->xmlViewer->append("NEXT MESSAGE:\n\n" + rawMessageContent + "\n");
        }

        // Update the index to search for the next message
        rawXMLIndex = messageEndIndex + messageEndText.length();

    }

}

MainWindow::ChatMessage MainWindow::buildMessage(QString rawMessageData) {

    MainWindow::ChatMessage chatMessage;

    return chatMessage;

}
