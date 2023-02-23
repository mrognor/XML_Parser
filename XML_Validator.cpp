#include "XML_Validator.h"

class line : public std::string {};

std::istream &operator>>(std::istream &is, line &l)
{
    std::getline(is, l);
    return is;
}

template<class T>
bool Validate(T xmlStringIt, T endIt)
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
    bool isSpecString = false;

    for (; xmlStringIt != endIt; xmlStringIt++)
    {
        lineNumber++;

        // Ignore line with xml specification
        if (lineNumber == 1)
        {
            std::string trimString = TrimString((*xmlStringIt), false);
            // Check if it is starts from "<&" string
            if((*xmlStringIt).length() >= 2 && (*xmlStringIt)[0] == '<' && (*xmlStringIt)[1] == '?')
            {
                isSpecString = true;
                continue;
            }
        }

        for (auto stringChar = (*xmlStringIt).begin(); stringChar != (*xmlStringIt).end(); stringChar++)
        {
            // Check if it is closing spec string
            if (isSpecString && *stringChar == '>' && *(stringChar - 1) == '?')
            {
                isSpecString = false;
                continue;
            }

            // Check if it is opening comment
            if ((stringChar + 1) != (*xmlStringIt).end() && (stringChar + 2) != (*xmlStringIt).end() && (stringChar + 3) != (*xmlStringIt).end() &&
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
    
    if (!tagStringsStack.empty())
    {
        LOG("Dont closed tags")
        return false;
    }
    else
        return true;
}

bool XmlValidator::ValidateFile(const std::string& fileName)
{
    std::ifstream fileToValidate(fileName.c_str());
    
    if (!fileToValidate.is_open())
    {
        LOG("Failed to open file: " + fileName);
        return false;
    }

    std::istream_iterator<line> begin(fileToValidate);
    std::istream_iterator<line> end;
    
    return Validate(begin, end);
}

bool XmlValidator::ValidateString(const std::string& str)
{
    std::vector<std::string> vec = {str};
    return ValidateVectorOfString(vec);
}

bool XmlValidator::ValidateVectorOfString(const std::vector<std::string>& vectorToValidate)
{
    return Validate(vectorToValidate.begin(), vectorToValidate.end());
}