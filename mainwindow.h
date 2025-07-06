#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qdatetime.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    struct ChatMessage {
        QDateTime dateTime;
        QString fromUser;
        QString toUser;
        QString text;
        QString textStyle;
    };

private slots:

    // File menu contents
    void handleOpenAction();
    void parseXML(QString data);
    MainWindow::ChatMessage buildMessage(QString rawMsgData);
    QDateTime parseDateTime(QString rawMessageData);
    QString parseFromUser(QString rawMessageData);
    QString parseToUser(QString rawMessageData);
    QString parseText(QString rawMessageData);
    QString parseTextStyle(QString rawMessageData);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
