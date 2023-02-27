#include "XML_Parser.h"

// This class required only to extract lines from files with iterators
class line : public std::string {};

// Define operator stream reading from file to line class
// Required because if read to string all spaces will be stop reading line
std::istream &operator>>(std::istream &is, line &l)
{
    std::getline(is, l);
    return is;
}

// This function is needed to check the container with xml data for validity
// Takes the beginning and end of the container as a parameter
// The iterator must contain the std::string
template<class T>
bool Validate(T begin, T end, std::list<std::string>& listWithAllData)
{
    listWithAllData.clear();

    // String to store all chars in tags
    std::string tagString;
    // String to store all chars between tags
    std::string betweenTagsString;
    // String to store comments
    std::string commentString;

    std::stack<std::string> tagStringsStack;
    bool isTagStringOpened = false;
    int lineNumber = 0;
    bool isComment = false;
    bool wasAtLeastOneTag = false;
    bool isSpecString = false;

    for (auto xmlStringIt = begin; xmlStringIt != end; xmlStringIt++)
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

                std::string trimmedValueString = TrimString(betweenTagsString);
                if (!CheckString(trimmedValueString))
                {
                    LOG("Invalid symbol on line: " + std::to_string(lineNumber));
                    return false;
                }
                
                if (!TrimString(betweenTagsString).empty())
                    listWithAllData.push_back(TrimString(betweenTagsString));
                    
                betweenTagsString.clear();
                continue;
            }

            // Check closing tag string
            if (*stringChar == '>')
            {
                isTagStringOpened = false;

                // Check if it is closing tag
                if (tagString[0] == '/')
                {
                    // Check if closing tag equals or not opening tag
                    if (tagStringsStack.size() != 0 && TrimString(tagString.substr(1)) == tagStringsStack.top())
                        tagStringsStack.pop();
                    else 
                    {
                        if (tagStringsStack.empty())
                        {
                            LOG("Closing not opened tag on line: " + std::to_string(lineNumber));
                        }
                        else
                        {
                            LOG("Closing wrong tag on line: " + std::to_string(lineNumber) + " Expected: " + tagStringsStack.top());
                        }
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

                listWithAllData.push_back("<" + tagString + ">");
                tagString.clear();
                continue;
            }

            // Check if tag line opened
            if (isTagStringOpened)
                tagString += *stringChar;
            else betweenTagsString += *stringChar;
        }

        // Add non tag and non comment line to data list
        if (!isTagStringOpened && !isComment && !TrimString(betweenTagsString).empty())
        {
            listWithAllData.push_back(TrimString(betweenTagsString));
            betweenTagsString.clear();
        }

        // Clear comment string if new line
        if (isComment)
        {
            commentString.clear();
            continue;
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

bool XmlParser::ValidateFile(const std::string& fileName)
{
    std::ifstream fileToValidate(fileName.c_str());
    
    if (!fileToValidate.is_open())
    {
        LOG("Failed to open file: " + fileName);
        return false;
    }

    std::istream_iterator<line> begin(fileToValidate);
    std::istream_iterator<line> end;
    
    return Validate(begin, end, Data);
}

bool XmlParser::ValidateString(const std::string& str)
{
    std::vector<std::string> vec = {str};
    return ValidateVectorOfString(vec);
}

bool XmlParser::ValidateVectorOfString(const std::vector<std::string>& vectorToValidate)
{
    return Validate(vectorToValidate.begin(), vectorToValidate.end(), Data);
}

void XmlParser::Find()
{
    std::string path;
    std::string tagName;
    std::string tagString;
    DataType data;
    std::map<std::string, std::string> paramsAndValues;

    for (const auto& it : Data)
    {
        tagName.clear();
        data = empty;
        paramsAndValues.clear();
        tagString.clear();

        // Is tag
        if(it.length() >= 3 && it[0] == '<' && it[it.length() - 1] == '>')
        {
            data = openingTag;
            tagString = it.substr(1, it.length() - 2);

            // Closing tag
            if (it.length() >= 4 && it[1] == '/')
            {
                data = closingTag;
                tagString = tagString.substr(1);
            }
            // Inline tag
            if (it.length() >= 4 && it[it.length() - 2] == '/')
            {
                data = inlineTag;
                tagString = tagString.substr(0, tagString.length() - 1);
            }
        }
        else 
            data = text;

        // If opening or inline tag
        if (data == 0 || data == 2)
        {
            ParseTagString(tagString, tagName, paramsAndValues);
            path += "/" + tagName;
        }
        
        // If closing tag
        if (data == 1)
        {
            ParseTagString(tagString, tagName, paramsAndValues);
            path = path.substr(0, path.length() - 1 - tagName.length());
        }

        std::cout << "Path: " << path << "    Tag name: " << tagName << "     ";
        std::cout << it << std::endl;

        // Remove inline tag
        if (data == 2)
        {
            path = path.substr(0, path.length() - 1 - tagName.length());
        }
    }
}