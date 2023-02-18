#include "XML_Validator.h"

bool XmlValidator::ValidateFile(const std::string& fileName)
{
    std::ifstream fileToValidate(fileName.c_str());
    
    if (!fileToValidate.is_open())
    {
        LOG("Faile to open file: ");
        return false;
    }

    std::string fileLine;
    std::string tagString;
    std::stack<std::string> tagStringsStack;
    bool isTagStringOpened = false;
    int lineNumber = 0;
    bool isComment = false;

    while (std::getline(fileToValidate, fileLine))
    {
        lineNumber++;
        for (auto stringChar = fileLine.begin(); stringChar != fileLine.end(); stringChar++)
        {
            // Check if it is opening comment
            if ((stringChar + 1) != fileLine.end() && (stringChar + 2) != fileLine.end() &&
            *stringChar == '<' && *(stringChar + 1) == '!' && *(stringChar + 2) == '-' && *(stringChar + 3) == '-')
            {
                isComment = true;
                continue;
            }

            // Check if it is closing comment
            if (*stringChar == '>' && *(stringChar - 1) == '-' && *(stringChar - 2) == '-')
            {
                isComment = false;
                continue;
            }

            // Skip all logic if it is a comment
            if (isComment)
                continue;

            // Check opening tag string
            if (*stringChar == '<')
            {
                // '<' symbol without closing tag
                if (isTagStringOpened)
                {
                    LOG("Opened new tag without closing previous on line: ");
                    return false;
                }

                isTagStringOpened = true;
                continue;
            }

            // Check closing tag string
            if (*stringChar == '>')
            {
                isTagStringOpened = false;

                // Check if it is suitable tag name
                if (isalpha(tagString[0]) == 0 && tagString[0] != '/')
                {
                    LOG("Wrong tag name on line: ");
                    return false;
                }

                // Check if it is closing tag
                if(tagString[0] == '/')
                {
                    // Check if closing tag equals or not opening tag
                    if (tagStringsStack.size() != 0 && tagString.substr(1) == tagStringsStack.top())
                        tagStringsStack.pop();
                    else 
                    {
                        LOG("Closing wrong tag on line: " + std::to_string(lineNumber) + " Expected: " + tagStringsStack.top());
                        return false;
                    }
                }
                else
                {
                    // Check if it is not one line tag
                    if (tagString[tagString.length() - 1] != '/')
                        tagStringsStack.push(tagString); // Add opening tag to stack
                }

                tagString = "";
                continue;
            }

            // Check if tag line opened
            if (isTagStringOpened)
                tagString += *stringChar;
        }
    }
    
    return true;
}