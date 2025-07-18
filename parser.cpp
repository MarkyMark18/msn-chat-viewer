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

        // Get the end index of the message
        int messageEndIndex = rawXMLData.indexOf("</Message>", messageStartIndex);

        // Handle a malformed message end tag (skip to next message)
        if (messageEndIndex == -1) {
            rawXMLIndex = messageStartIndex + messageStartString.length();
            continue;
        }

        // Get the length of the message
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

    // Get the start indexes of the date and time tags
    int dateStartIndex = rawMessageData.indexOf(dateStartString, 0);
    int timeStartIndex = rawMessageData.indexOf(timeStartString, 0);

    // Handle malformed date or time tags (return a blank datetime)
    if (dateStartIndex == -1 || timeStartIndex == -1) {
        return QDateTime();
    }

    // Calculate the start indexes of the actual date and time
    int dateIndex = dateStartIndex + dateStartString.length();
    int timeIndex = timeStartIndex + timeStartString.length();


    // Extract the date and time strings and combine them with a T separator
    QString dateString = rawMessageData.mid(dateIndex, dateLength);
    QString timeString = rawMessageData.mid(timeIndex, timeLength);
    QString dateTimeString = dateString + "T" + timeString;

    // Return a DateTime object created from the string
    return QDateTime::fromString(dateTimeString, "dd/MM/yyyyTHH:mm:ss");
}

QString Parser::parseUsername(QString rawMessageData, QString fromToTag) {

    // The username start and end tags, for searching the raw data
    QString userStartString = "FriendlyName=\"";
    QString userEndTag = "\"/>";

    // Get the index of the from/to tag
    int fromToTagIndex = rawMessageData.indexOf(fromToTag, 0);

    // Handle a malformed from/to tag (return an empty string)
    if (fromToTagIndex == -1) {
        return "";
    }

    // Get the index of the start string
    int userStartStringIndex = rawMessageData.indexOf(userStartString, fromToTagIndex);

    // Handle a malformed start string (return an empty string)
    if (userStartStringIndex == -1) {
        return "";
    }

    // Get the start and end index of the username
    int userStartIndex = userStartStringIndex + userStartString.length();
    int userEndIndex = rawMessageData.indexOf(userEndTag, userStartIndex);

    // Handle a malformed username end tag (return an empty string)
    if (userEndIndex == -1) {
        return "";
    }

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

    // Get the starting text tag index
    int textTagIndex = rawMessageData.indexOf(textStartTagStartString, 0);

    // Handle malformed/missing text tag (return empty string)
    if (textTagIndex == -1) {
        return "";
    }

    // Get the start index of the text
    int textStartIndex = rawMessageData.indexOf(textStartTagEndString, textTagIndex) + 1;

    // Handle missing closing angle bracket on text start tag (return empty string)
    // NOTE: validates as 0 due to the +1 in textStartIndex declaration
    if (textStartIndex == 0) {
        return "";
    }

    // Get the end index of the text
    int textEndIndex = rawMessageData.indexOf(textEndTagString, textStartIndex);

    // Handle malformed/missing text end tag (Return empty string)
    if (textEndIndex == -1) {
        return "";
    }

    // Extract and return the message text
    return rawMessageData.mid(textStartIndex, textEndIndex - textStartIndex);

}

QString Parser::parseTextStyle(QString rawMessageData) {

    // The start and end of the text style tag, for searching the raw data
    QString textStyleTagStartString = "<Text Style=\"";
    int textStyleTagLength = textStyleTagStartString.length();
    QString textStyleTagEndString = "\">";

    // Get the index of the text style start tag
    int textStyleStartTagIndex = rawMessageData.indexOf(textStyleTagStartString, 0);

    // Handle malformed/missing text style start tag (return empty string)
    if (textStyleStartTagIndex == -1) {
        return "";
    }

    // Get the start index of the text style
    int textStyleStartIndex = textStyleStartTagIndex + textStyleTagLength;

    // Get the index of the text style end tag
    int textStyleEndIndex = rawMessageData.indexOf(textStyleTagEndString, textStyleStartIndex);

    // Handle malformed/missing text style end tag (Return empty string)
    if (textStyleEndIndex == -1) {
        return "";
    }

    // Extract and return the text style
    return rawMessageData.mid(textStyleStartIndex, textStyleEndIndex - textStyleStartIndex);

}

