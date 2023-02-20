#include "XML_Validator.h"

bool XmlValidator::ValidateFile(const std::string& fileName)
{
    std::ifstream fileToValidate(fileName.c_str());
    
    if (!fileToValidate.is_open())
    {
        LOG("Failed to open file: " + fileName);
        return false;
    }

    std::string fileLine;
    std::vector<std::string> fileVec;

    while (getline(fileToValidate, fileLine))
        fileVec.push_back(fileLine);
    
    return ValidateVectorOfString(fileVec);
}

bool XmlValidator::ValidateVectorOfString(std::vector<std::string>& vectorToValidate)
{
    // String to store all chars in tags
    std::string tagString;
    // String to store all chars between tags
    std::string valueString;
    // String to store comments
    std::string commentString;

    std::stack<std::string> tagStringsStack;
    bool isTagStringOpened = false;
    int lineNumber = 0;
    bool isComment = false;
    bool wasAtLeastOneTag = false;

    for (std::string xmlString : vectorToValidate)
    {
        // Ignore line with xml specification
        if (lineNumber == 0)
        {
            std::string xmlSpecString = TrimString(xmlString, false);
            if(xmlString[0] == '<' && xmlString[1] == '?' &&
                xmlSpecString[xmlSpecString.length() - 1] == '>' && 
                xmlSpecString[xmlSpecString.length() - 2] == '?')
                continue;
        }

        lineNumber++;
        for (auto stringChar = xmlString.begin(); stringChar != xmlString.end(); stringChar++)
        {
            // Check if it is opening comment
            if ((stringChar + 1) != xmlString.end() && (stringChar + 2) != xmlString.end() && (stringChar + 3) != xmlString.end() &&
            *stringChar == '<' && *(stringChar + 1) == '!' && *(stringChar + 2) == '-' && *(stringChar + 3) == '-')
            {
                isComment = true;
                continue;
            }

            // Check if it is closing comment
            if (*stringChar == '>' && *(stringChar - 1) == '-' && *(stringChar - 2) == '-')
            {
                isComment = false;

                if (commentString.substr(3, commentString.length() - 5).find("--") != -1)
                {
                    LOG("The string \"--\" is not valid inside comments");
                    return false;
                }

                commentString.clear();
                continue;
            }

            // Skip all logic if it is a comment
            if (isComment)
            {
                commentString += *stringChar;
                continue;
            }

            // Check opening tag string
            if (*stringChar == '<')
            {
                if (tagStringsStack.size() == 0 && wasAtLeastOneTag)
                {
                    LOG("There can be only one root element in a file. New root element on line: " + std::to_string(lineNumber));
                    return false;
                }

                // '<' symbol without closing tag
                if (isTagStringOpened)
                {
                    LOG("Opened new tag without closing previous on line: " + std::to_string(lineNumber));
                    return false;
                }

                isTagStringOpened = true;

                std::string trimmedValueString = TrimString(valueString);
                if (!CheckString(trimmedValueString))
                {
                    LOG("Invalid symbol on line: " + std::to_string(lineNumber));
                    return false;
                }
                
                valueString.clear();

                continue;
            }

            // Check closing tag string
            if (*stringChar == '>')
            {
                isTagStringOpened = false;

                // Check if it is closing tag
                if(tagString[0] == '/')
                {
                    // Check if closing tag equals or not opening tag
                    if (tagStringsStack.size() != 0 && TrimString(tagString.substr(1)) == tagStringsStack.top())
                        tagStringsStack.pop();
                    else 
                    {
                        LOG("Closing wrong tag on line: " + std::to_string(lineNumber) + " Expected: " + tagStringsStack.top());
                        return false;
                    }
                }
                else
                {
                    std::string tagName;
                    std::map<std::string, std::string> paramsAndValues;

                    // Check if it is not one line tag
                    if (tagString[tagString.length() - 1] != '/')
                    {
                        if (!ParseTagString(tagString, tagName, paramsAndValues))
                        {
                            LOG("Wrong tag name on line: " + std::to_string(lineNumber));
                            return false;
                        }
                        tagStringsStack.push(tagName); // Add opening tag to stack
                        wasAtLeastOneTag = true;
                    }
                    else // One line tag check
                    {
                        if (!ParseTagString(tagString.substr(0, tagString.length() - 1), tagName, paramsAndValues))
                        {
                            LOG("Wrong tag name on line: " + std::to_string(lineNumber));
                            return false;
                        }
                    }
                }

                tagString = "";
                continue;
            }

            // Check if tag line opened
            if (isTagStringOpened)
                tagString += *stringChar;
            else valueString += *stringChar;
        }
    }
    
    return true;
}