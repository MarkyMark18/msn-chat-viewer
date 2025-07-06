#ifndef PARSER_H
#define PARSER_H

#include <qdatetime.h>

struct ChatMessage {
    QDateTime dateTime;
    QString fromUser;
    QString toUser;
    QString text;
    QString textStyle;
};

class Parser
{
public:
    Parser();
    std::vector<ChatMessage> parseXML(const QString &rawXMLData);

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
