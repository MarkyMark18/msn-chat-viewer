#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "parser.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QProgressDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Setup the thread for the parser
    parser.moveToThread(&parserThread);
    connect(this, &MainWindow::startParsing, &parser, &Parser::parseXML);
    connect(&parser, &Parser::parsingFinished, this, &MainWindow::handleParsingFinished);
    connect(this, &MainWindow::destroyed, &parserThread, &QThread::quit);
    parserThread.start();

    // Set the xmlViewer background colour to white so that messages display correctly
    // (MSN Messenger didn't have dark mode)
    QPalette xmlViewerPalette = ui->xmlViewer->palette();
    xmlViewerPalette.setColor(QPalette::Base, Qt::white);
    ui->xmlViewer->setAutoFillBackground(true);
    ui->xmlViewer->setPalette(xmlViewerPalette);

    // File menu contents
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::handleOpenAction);
    connect(ui->actionClose, &QAction::triggered, this, &QWidget::close);

    // Buttons
    connect(ui->openButton, &QPushButton::clicked, this, &MainWindow::handleOpenAction);
}

MainWindow::~MainWindow()
{
    parserThread.quit();
    parserThread.wait();
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

    // Set up the dialog for the loading progress bar and connect it to get progress updates
    QProgressDialog progressDialog;
    connect(&parser, &Parser::progressUpdate, &progressDialog, &QProgressDialog::setValue);
    progressDialog.setWindowTitle("Loading...");

    emit startParsing(rawXMLData);

    progressDialog.exec();

}

void MainWindow::handleParsingFinished(const std::vector<ChatMessage> &messageList) {

    displayMessages(messageList);

}

void MainWindow::displayMessages(const std::vector<ChatMessage> &messageList) {

    // The styling to use for the header
    QString headerStyle = "font-family:Calibri; background-color:#dddddd; color:#111111; font-size:" +
        defaultFontSize + "pt;";

    // Initialise the QStringList to add each message to
    QStringList htmlMessageList;

    // Create each message and add them to the htmlMessageList
    for (const ChatMessage &msg : messageList) {

        // The font size and colour for the message
        QString textStyle = msg.textStyle + " font-size:" + defaultFontSize + "pt;";

        // Create the message
        QString singleFormattedMessage = QString(
            "<div style=\"%1\">"
            "%2<br>"
            "From: %3<br>"
            "To: %4"
            "</div>"
            "<div style=\"%5\">"
            "<br>&nbsp;&nbsp;&nbsp;&nbsp;%6<br>"
            "</div>"
            ).arg(
                headerStyle,
                msg.dateTime.toString(),
                msg.fromUser,
                msg.toUser,
                textStyle,
                msg.text
                );

        // Add the message to the message list
        htmlMessageList.append(singleFormattedMessage);

    }

    ui->xmlViewer->setHtml(htmlMessageList.join(""));

}
