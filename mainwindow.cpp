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

    // Set the xmlViewer background colour to white so that messages display correctly
    // (MSN Messenger didn't have dark mode)
    QPalette xmlViewerPalette = ui->xmlViewer->palette();
    xmlViewerPalette.setColor(QPalette::Base, Qt::white);
    ui->xmlViewer->setAutoFillBackground(true);
    ui->xmlViewer->setPalette(xmlViewerPalette);

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

    // Output the messageList to the xmlViewer
    displayMessages(messageList);

}

void MainWindow::displayMessages(const std::vector<ChatMessage> &messageList) {

    // The styling to use for the header
    QString headerStyle = "font-family:Calibri; color:#111111; font-size:" +
        defaultFontSize + "pt;";

    // Output the message to the XMLViewer
    for (const ChatMessage &msg : messageList) {

        // Create the formatted header
        QString formattedHeader = QString(
                                    "<pre><span style=\"%1\">%2\nFrom: %3\nTo: %4</span></pre>"
                                    )
                                    .arg(
                                        headerStyle,
                                        msg.dateTime.toString(),
                                        msg.fromUser,
                                        msg.toUser
                                    );

        // Add the font size to the message
        QString textStyle = msg.textStyle + " font-size:" + defaultFontSize + "pt;";

        // Create the formatted message text using the original styling
        QString formattedText = QString(
                                    "<pre><span style=\"%1\">    %2\n</span></pre>"
                                    )
                                    .arg(
                                        textStyle,
                                        msg.text
                                    );

        // Output the formatted header and text to the viewer
        ui->xmlViewer->append(formattedHeader);
        ui->xmlViewer->append(formattedText);

    }

}
