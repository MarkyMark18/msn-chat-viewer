#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "parser.h"
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

private slots:

    // File menu contents
    void handleOpenAction();
    void displayMessages(const std::vector<ChatMessage> &messageList);

private:
    Ui::MainWindow *ui;

    const QString defaultFontSize = "12";
};
#endif // MAINWINDOW_H
