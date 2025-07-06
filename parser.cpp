#include "parser.h"

Parser::Parser() {}

std::vector<ChatMessage> Parser::parseXML(const QString &rawXMLData) {

    // Initialise the index
    int rawXMLIndex = 0;

    // The message start and end tags, for searching the raw data
    QString messageStartString = "<Message";
    QString messageEndString = "</Message>";

    // Create the message list
    std::vector<ChatMessage> messageList;

    // Loop through the raw data
    while (rawXMLIndex < rawXMLData.length()) {

        // Get the start index of the next message
        int messageStartIndex = rawXMLData.indexOf(messageStartString, rawXMLIndex);

        // Check if there are no more messages left to parse
        if (messageStartIndex == -1) {
            break;
        }

        // Offset the messageStartIndex to remove "<Message"
        messageStartIndex += messageStartString.length();

        // Get the end index of the message and it's length
        int messageEndIndex = rawXMLData.indexOf("</Message>", messageStartIndex) - 1;
        int messageLength = messageEndIndex - messageStartIndex;

        // Retrieve the contents of the tag (unless it's a closing tag)
        QString rawMessageContent = rawXMLData.mid(messageStartIndex, messageLength).trimmed();

        // // PLACEHOLDER Output the text content to the viewer
        // if (!rawMessageContent.isEmpty()) {
        //     ui->xmlViewer->append("NEXT MESSAGE:\n\n" + rawMessageContent + "\n");
        // }

        // Build the message and add it to the list
        ChatMessage chatMessage = Parser::buildMessage(rawMessageContent);
        messageList.push_back(chatMessage);

        // Update the index to search for the next message
        rawXMLIndex = messageEndIndex + messageEndString.length();

    }

    return messageList;

}

ChatMessage Parser::buildMessage(QString rawMessageData) {

    ChatMessage chatMessage;

    chatMessage.dateTime = Parser::parseDateTime(rawMessageData);
    chatMessage.fromUser = Parser::parseFromUser(rawMessageData);
    chatMessage.toUser = Parser::parseToUser(rawMessageData);
    chatMessage.text = Parser::parseText(rawMessageData);
    chatMessage.textStyle = Parser::parseTextStyle(rawMessageData);

    return chatMessage;

}

QDateTime Parser::parseDateTime(QString rawMessageData) {

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

QString Parser::parseUsername(QString rawMessageData, QString fromToTag) {

    // The username start and end tags, for searching the raw data
    QString userStartString = "FriendlyName=\"";
    QString userEndTag = "\"/>";

    // Get the start and end indexes of the username
    int fromTagIndex = rawMessageData.indexOf(fromToTag, 0);
    int userStartIndex = rawMessageData.indexOf(userStartString, fromTagIndex) + userStartString.length();
    int userEndIndex = rawMessageData.indexOf(userEndTag, userStartIndex);

    // Extract and return the username
    return rawMessageData.mid(userStartIndex, userEndIndex - userStartIndex);

}

QString Parser::parseFromUser(QString rawMessageData) {

    // The <From> tag, for passing to parseUsername() so correct username can be retrieved
    QString fromTag = "<From>";

    // Return the From username
    return Parser::parseUsername(rawMessageData, fromTag);

}

QString Parser::parseToUser(QString rawMessageData) {

    // The <To> tag, for passing to parseUsername() so correct username can be retrieved
    QString toTag = "<To>";

    // Return the To username
    return Parser::parseUsername(rawMessageData, toTag);

}

QString Parser::parseText(QString rawMessageData) {

    // The various elements of the Text start and end tags, for searching the raw data
    QString textStartTagStartString = "<Text ";
    QString textStartTagEndString = ">";
    QString textEndTagString = "</Text>";

    // Get the start and end indexes of the text
    int textTagIndex = rawMessageData.indexOf(textStartTagStartString, 0);
    int textStartIndex = rawMessageData.indexOf(textStartTagEndString, textTagIndex) + 1;
    int textEndIndex = rawMessageData.indexOf(textEndTagString, textStartIndex);

    // Extract and return the message text
    return rawMessageData.mid(textStartIndex, textEndIndex - textStartIndex);;

}

QString Parser::parseTextStyle(QString rawMessageData) {

    // The start and end of the text style tag, for searching the raw data
    QString textStyleTagStartString = "<Text Style=\"";
    int textStyleTagLength = textStyleTagStartString.length();
    QString textStyleTagEndString = "\">";

    // Get the start and end indexes of the text style
    int textStyleStartIndex = rawMessageData.indexOf(textStyleTagStartString, 0) + textStyleTagLength;
    int textStyleEndIndex = rawMessageData.indexOf(textStyleTagEndString, textStyleStartIndex);

    // Extract and return the text style
    return rawMessageData.mid(textStyleStartIndex, textStyleEndIndex - textStyleStartIndex);

}

