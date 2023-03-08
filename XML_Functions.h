/** @file */
/// \cond HIDDEN_SYMBOLS

#pragma once

#include <map>
#include <list>
#include <stack>
#include <string>
#include <vector>
#include <iostream>


// Pointer to log function. By default set to nullptr inside XML_Functions.cpp
// Can be defined by user using SetLogFunc
extern void (*LogFuncPtr)(std::string);

#ifndef NDEBUG
#define LOG(error) \
    if (LogFuncPtr == nullptr) \
        std::cerr << error << std::endl; \
    else \
        LogFuncPtr(error);
#else
#define LOG(error)
#endif

/// \endcond

enum XmlDataType
{
    empty = -1,
    openingTag = 0,
    closingTag = 1,
    inlineTag = 2,
    text = 3,
    comment = 4
};

struct XmlData
{
    XmlDataType DataType;
    std::string Path;
    std::string TagName;
    std::map<std::string, std::string> ParamsAndValues;
    std::string Data;

    XmlData() {}
    XmlData(std::string data) : Data(data) {}
    operator std::string() const { return Data; }
    friend std::ostream& operator<< (std::ostream &out, const XmlData &data);
};


/*!
    Function for setting the logging function

    \param[in] logFuncPtr custom logging function
*/
void SetLogFunc(void (*logFuncPtr)(std::string));

/*!
    Function to split a string using a splitter string

    \param[in] stringToSplit The string to be splitted
    \param[in] splitterString The string that is used as a splitter

    \return Returns a vector of string consisting of parts of the stringToSplit
*/
std::vector<std::string> Split(const std::string& stringToSplit, const std::string& splitterString);

/*!
    This function delete all spaces and tabs from line beggining and ending

    \param[in] str The string to trim
    \param[in] isFromLeft The function will delete all spaces from left
    \param[in] isFromRight The function will delete all spaces from right

    \return Returns a trimmed string
*/
std::string TrimString(const std::string& str, bool isFromLeft = true, bool isFromRight = true);

/*!
    This function checks the tag name or the name of its parameter for validity. 
    If it is valid, it will return true, otherwise false

    \param[in] tagString string with tag name or tag parametr name

    \return Return true if this string can be valid tag name or parametr name, return false otherwise
*/
bool CheckTagAndParamName(const std::string& tagString);

/*!
    This function checks the parameter value or strings between tags for validity. 
    If it is valid, it will return true, otherwise false

    \param[in] valueString string with parameter value or strings between tags

    \return Return true if this string can be valid parameter value or strings between tags, return false otherwise
*/
bool CheckString(const std::string& valueString);

/*!
    This function checks the tag string for validity. 
    If it is valid, it will return true, otherwise false

    \param[in] tagString string with tag
    \param[out] tagName string to put tag name if tag string valid
    \param[out] paramsAndValues map to put all params and values if tag string valid

    \return Return true if this string can be valid tag, return false otherwise
*/
bool ParseTagString(const std::string& tagString, std::string& tagName, std::map<std::string, std::string>& paramsAndValues);

template <class T, class V>
int Count(T where, V what)
{
    int count = 0;
    for (const auto& it : where)
        if (it == what) count++;
    return count;
}

/*!
    This function is needed to check the container with xml data for validity

    \warning The iterator must contain the std::string

    \param[in] begin container start
    \param[in] end container end
    \param[out] listWithAllData list to store all xml data from container
*/
template<class T>
bool Validate(T begin, T end, std::list<XmlData>* listWithAllData = nullptr, bool isPathSaving = true)
{
    if (listWithAllData != nullptr)
        listWithAllData->clear();

    // String to store all chars in tags
    std::string tagString;
    // String to store all chars between tags
    std::string betweenTagsString;
    // String to store comments
    std::string commentString;
    // List with all opened but not closed tags
    std::list<std::string> tagStringsStackList;
    // Is string with tag name and params opened by "<" symbol
    bool isTagStringOpened = false;
    // Line number in file or number of element in vector 
    int lineNumber = 0;
    // Is string with comment
    bool isComment = false;
    // Variable to store if it was root element
    bool wasAtLeastOneTag = false;
    // Is string with xml specification
    bool isSpecString = false;

    // For loop by iterator
    for (auto xmlStringIt = begin; xmlStringIt != end; xmlStringIt++)
    {
        lineNumber++;
        std::string xmlString = (std::string)(*xmlStringIt);

        // Ignore line with xml specification
        if (lineNumber == 1)
        {
            std::string trimString = TrimString(xmlString, false);
            // Check if it is starts from "<?" string
            if(xmlString.length() >= 5 && xmlString[0] == '<' && xmlString[1] == '?' && xmlString[2] == 'x' && 
            xmlString[3] == 'm' && xmlString[4] == 'l')
            {
                isSpecString = true;
                continue;
            }
        }

        // For loop by string
        for (auto stringChar = xmlString.begin(); stringChar != xmlString.end(); stringChar++)
        {
            // Check if it is closing spec string
            if (isSpecString && *stringChar == '>' && *(stringChar - 1) == '?')
            {
                isSpecString = false;
                continue;
            }

            // Skip logic if it is spec string
            if (isSpecString)
                continue;

            // Check if it is opening comment
            if ((stringChar + 1) != xmlString.end() && (stringChar + 2) != xmlString.end() &&
            *stringChar == '<' && *(stringChar + 1) == '!' && *(stringChar + 2) == '-' && *(stringChar + 3) == '-')
            {
                isComment = true;
                commentString += *stringChar;
                continue;
            }

            // Check if it is closing comment
            if (stringChar != xmlString.begin() && (stringChar - 1) != xmlString.begin() &&
                *stringChar == '>' && *(stringChar - 1) == '-' && *(stringChar - 2) == '-')
            {
                isComment = false;
                commentString += *stringChar;

                // Check if it is string "--" on comment
                if (commentString.substr(4, commentString.length() - 7).find("--") != -1)
                {
                    LOG("The string \"--\" is not valid inside comments");
                    return false;
                }

                // Add comment to XmlData list
                if(listWithAllData != nullptr && !commentString.empty())
                {
                    // Create XmlData object
                    XmlData commentData;
                    commentData.Data = commentString;
                    commentData.DataType = comment;

                    // Find path
                    if (isPathSaving)
                    {
                        std::string path = "/";
                        for (const auto& it : tagStringsStackList)
                            path = path + it + "/";
                        commentData.Path = path;
                    }

                    // Store data
                    listWithAllData->push_back(commentData);
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
                // Check if it is second root element
                if (tagStringsStackList.size() == 0 && wasAtLeastOneTag)
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

                // Check string between tags to valid
                std::string trimmedValueString = TrimString(betweenTagsString);
                if (!CheckString(trimmedValueString))
                {
                    LOG("Invalid symbol on line: " + std::to_string(lineNumber));
                    return false;
                }
                
                // Add string between tag to XmlData list
                if (!TrimString(betweenTagsString).empty() && listWithAllData != nullptr)
                {
                    // Create XmlData object
                    XmlData textData;
                    textData.Data = TrimString(betweenTagsString);
                    textData.DataType = text;

                    // Find path
                    if (isPathSaving)
                    {
                        std::string path = "/";
                        for (const auto& it : tagStringsStackList)
                            path = path + it + "/";
                        textData.Path = path;
                    }

                    // Store data
                    listWithAllData->push_back(textData);
                }

                betweenTagsString.clear();
                continue;
            }

            // Check closing tag string
            if (*stringChar == '>')
            {
                isTagStringOpened = false;
                std::string tagName;
                std::map<std::string, std::string> paramsAndValues;

                // Check if it is closing tag
                if (tagString[0] == '/')
                {
                    // Check if closing tag equals or not opening tag
                    if (tagStringsStackList.size() != 0 && TrimString(tagString.substr(1)) == tagStringsStackList.back())
                    {
                        tagName = tagStringsStackList.back();
                        tagStringsStackList.pop_back();
                    }
                    else 
                    {
                        if (tagStringsStackList.empty())
                        {
                            LOG("Closing not opened tag on line: " + std::to_string(lineNumber));
                        }
                        else
                        {
                            LOG("Closing wrong tag on line: " + std::to_string(lineNumber) + " Expected: " + tagStringsStackList.back());
                        }
                        return false;
                    }
                }
                else
                {
                    // Check if it is not one line tag
                    if (tagString[tagString.length() - 1] != '/')
                    {
                        if (!ParseTagString(tagString, tagName, paramsAndValues))
                        {
                            LOG("Wrong tag name on line: " + std::to_string(lineNumber));
                            return false;
                        }

                        tagStringsStackList.push_back(tagName); // Add opening tag to stack
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

                // Add tag to XmlDataList
                if (listWithAllData != nullptr)
                {
                    // Create XmlData object
                    XmlData tagData;
                    
                    // Find XmlData object type
                    // Opening tag
                    tagData.DataType = openingTag;
                    // Closing tag
                    if (tagString[0] == '/')
                        tagData.DataType = closingTag;
                    // Inline tag
                    if (tagString[tagString.length() - 1] == '/')
                        tagData.DataType = inlineTag;

                    // Format raw data
                    if (tagData.DataType == closingTag)
                        tagData.Data = "<" + TrimString(tagString) + ">";
                    else
                    {
                        tagData.Data = "<" + tagName;

                        for (const auto& it : paramsAndValues)
                            tagData.Data = tagData.Data + " " + it.first + "=\"" + it.second + "\"";
                        
                        if (tagData.DataType == inlineTag)
                            tagData.Data += "/";

                        tagData.Data += ">";
                    }

                    // Set XmlData object tag name and map with params and values
                    tagData.ParamsAndValues = paramsAndValues;
                    tagData.TagName = tagName;

                    // Find path
                    if (isPathSaving)
                    {
                        std::string path = "/";
                        if (!tagStringsStackList.empty())
                        {
                            // Add all path elements except last
                            for (auto it = tagStringsStackList.begin(); it != --tagStringsStackList.end(); it++)
                                path = path + *it + "/" ;

                            // Add last path element if it is closing tag
                            if (tagData.DataType == closingTag || tagData.DataType == inlineTag)
                                path = path + tagStringsStackList.back() + "/";
                        }
                        tagData.Path = path;
                    }

                    // Store data
                    listWithAllData->push_back(tagData);
                }
                tagString.clear();
                continue;
            }

            // Check if tag line opened
            if (isTagStringOpened)
                tagString += *stringChar;
            else betweenTagsString += *stringChar;
        }

        // Skip logic if it is spec string
        if (isSpecString)
            continue;

        // Add new line symbol to comment
        if (isComment)
        {
            commentString += "\n";
            continue;
        }

        // Add non tag and non comment line to data list
        if (!isTagStringOpened && !isComment && !TrimString(betweenTagsString).empty())
        {
            if (tagStringsStackList.size() == 0)
            {
                LOG("Text outside xml root on line number: " + std::to_string(lineNumber));
                return false;
            }

            // Add string between tag to XmlData list
            if (!TrimString(betweenTagsString).empty() && listWithAllData != nullptr)
            {
                // Create XmlData object
                XmlData textData;
                textData.Data = TrimString(betweenTagsString);
                textData.DataType = text;

                // Find path
                if (isPathSaving)
                {
                    std::string path = "/";
                    for (const auto& it : tagStringsStackList)
                        path = path + it + "/";
                    textData.Path = path;
                }

                // Store data
                listWithAllData->push_back(textData);
            }

            betweenTagsString.clear();
        }
    }
    
    // Check if it is some unclosed tags
    if (!tagStringsStackList.empty())
    {
        LOG("Dont closed tags")
        return false;
    }
    else
        return true;
}