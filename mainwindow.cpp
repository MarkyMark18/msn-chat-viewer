#include "mainwindow.h"
#include "./ui_mainwindow.h"
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

    QString xmlData = file.readAll();

    // Pass the XML data to the parser to format it for display
    parseXML(xmlData);

}

void MainWindow::parseXML(QString data) {

    // Initialise the index
    int rawXMLIndex = 0;

    // The message start and end tags, for searching the raw data
    QString messageStartString = "<Message";
    QString messageEndString = "</Message>";

    // Create the message list
    std::vector<ChatMessage> messageList;

    // Loop through the raw data
    while (rawXMLIndex < data.length()) {

        // Get the start index of the next message
        int messageStartIndex = data.indexOf(messageStartString, rawXMLIndex);

        // Check if there are no more messages left to parse
        if (messageStartIndex == -1) {
            break;
        }

        // Offset the messageStartIndex to remove "<Message"
        messageStartIndex += messageStartString.length();

        // Get the end index of the message and it's length
        int messageEndIndex = data.indexOf("</Message>", messageStartIndex) - 1;
        int messageLength = messageEndIndex - messageStartIndex;

        // Retrieve the contents of the tag (unless it's a closing tag)
        QString rawMessageContent = data.mid(messageStartIndex, messageLength).trimmed();

        // // PLACEHOLDER Output the text content to the viewer
        // if (!rawMessageContent.isEmpty()) {
        //     ui->xmlViewer->append("NEXT MESSAGE:\n\n" + rawMessageContent + "\n");
        // }

        // Build the message and add it to the list
        MainWindow::ChatMessage chatMessage = MainWindow::buildMessage(rawMessageContent);
        messageList.push_back(chatMessage);

        // Update the index to search for the next message
        rawXMLIndex = messageEndIndex + messageEndString.length();

    }

}

MainWindow::ChatMessage MainWindow::buildMessage(QString rawMessageData) {

    MainWindow::ChatMessage chatMessage;

    chatMessage.dateTime = MainWindow::parseDateTime(rawMessageData);
    chatMessage.fromUser = MainWindow::parseFromUser(rawMessageData);
    chatMessage.toUser = MainWindow::parseToUser(rawMessageData);
    chatMessage.text = MainWindow::parseText(rawMessageData);
    chatMessage.textStyle = MainWindow::parseTextStyle(rawMessageData);

    return chatMessage;

}

QDateTime MainWindow::parseDateTime(QString rawMessageData) {

    // The date and time start tags, for searching the raw data
    QString dateStartString = "Date=\"";
    int dateLength = 10;
    QString timeStartString = "Time=\"";
    int timeLength = 8;

    // Get the start indexes of the date and time
    int dateIndex = rawMessageData.indexOf(dateStartString, 0) + dateStartString.length();
    int timeIndex = rawMessageData.indexOf(timeStartString, 0) + timeStartString.length();

    // Extract the date and time strings and combine them with a T separator
    QString dateString = rawMessageData.mid(dateIndex, dateLength);
    QString timeString = rawMessageData.mid(timeIndex, timeLength);
    QString dateTimeString = dateString + "T" + timeString;

    // Return a DateTime object created from the string
    return QDateTime::fromString(dateTimeString, "yyyy-MM-ddTHH:mm:ss");
}

QString MainWindow::parseUsername(QString rawMessageData, QString fromToTag) {

    // The username start and end tags, for searching the raw data
    QString userStartString = "FriendlyName=\"";
    QString userEndTag = "\"/>";

    // Get the start and end indexes of the username
    int fromTagIndex = rawMessageData.indexOf(fromToTag, 0);
    int userStartIndex = rawMessageData.indexOf(userStartString, fromTagIndex) + userStartString.length();
    int userEndIndex = rawMessageData.indexOf(userEndTag, userStartIndex);

    // Return the username
    return rawMessageData.mid(userStartIndex, userEndIndex - userStartIndex);

}

QString MainWindow::parseFromUser(QString rawMessageData) {

    // The <From> tag, for passing to parseUsername() so correct username can be retrieved
    QString fromTag = "<From>";

    // Return the From username
    return MainWindow::parseUsername(rawMessageData, fromTag);

}

QString MainWindow::parseToUser(QString rawMessageData) {

    // The <To> tag, for passing to parseUsername
    QString toTag = "<To>";

    //Return the To username
    return MainWindow::parseUsername(rawMessageData, toTag);

}

QString MainWindow::parseText(QString rawMessageData) {
    QString text = "";
    return text;
}

QString MainWindow::parseTextStyle(QString rawMessageData) {
    QString textStyle = "";
    return textStyle;
}
