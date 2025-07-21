#ifndef PARSER_H
#define PARSER_H

#include <qdatetime.h>
#include <QObject>

struct ChatMessage {
    QDateTime dateTime;
    QString fromUser;
    QString toUser;
    QString text;
    QString textStyle;
};

class Parser : public QObject
{

    Q_OBJECT

public:
    Parser();
    // std::vector<ChatMessage> parseXML(const QString &rawXMLData);

public slots:
    void parseXML(const QString &rawXMLData);

signals:
    void progressUpdate(int percentage);
    void parsingFinished(const std::vector<ChatMessage> results);

private:
    ChatMessage buildMessage(QString rawMessageData);
    QDateTime parseDateTime(QString rawMessageData);
    QString parseUsername(QString rawMessageData, QString tagType);
    QString parseFromUser(QString rawMessageData);
    QString parseToUser(QString rawMessageData);
    QString parseText(QString rawMessageData);
    QString parseTextStyle(QString rawMessageData);
};

#endif // PARSER_H
