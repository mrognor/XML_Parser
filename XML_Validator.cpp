#include "XML_Validator.h"

bool XmlValidator::ValidateFile(const std::string& fileName)
{
    std::ifstream fileToValidate(fileName.c_str());
    
    if (!fileToValidate.is_open())
    {
        std::cerr << "Faile to open file: " << fileName << std::endl;
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
                    std::cerr << "Opened new tag without closing previous on line: " << lineNumber << std::endl;
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
                    std::cerr << "Wrong tag name on line: " << lineNumber << std::endl;
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
                        std::cerr << "Closing wrong tag on line: " << lineNumber << " Expected: " << tagStringsStack.top() << std::endl;
                        return false;
                    }
                }
                else
                    tagStringsStack.push(tagString); // Add opening tag to stack

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