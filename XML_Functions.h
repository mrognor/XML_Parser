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
bool Validate(T begin, T end, std::list<std::string>* listWithAllData = nullptr)
{
    if (listWithAllData != nullptr)
        listWithAllData->clear();

    // String to store all chars in tags
    std::string tagString;
    // String to store all chars between tags
    std::string betweenTagsString;
    // String to store comments
    std::string commentString;
    // Stack with all opened but not closed tags
    std::stack<std::string> tagStringsStack;
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

        // For loop by string
        for (auto stringChar = (*xmlStringIt).begin(); stringChar != (*xmlStringIt).end(); stringChar++)
        {
            // Check if it is closing spec string
            if (isSpecString && *stringChar == '>' && *(stringChar - 1) == '?')
            {
                isSpecString = false;
                continue;
            }

            // Check if it is opening comment
            if ((stringChar + 1) != (*xmlStringIt).end() && (stringChar + 2) != (*xmlStringIt).end() &&
            *stringChar == '<' && *(stringChar + 1) == '!' && *(stringChar + 2) == '-' && *(stringChar + 3) == '-')
            {
                isComment = true;
                commentString += *stringChar;
                continue;
            }

            // Check if it is closing comment
            if ((stringChar - 1) != (*xmlStringIt).begin() &&
                *stringChar == '>' && *(stringChar - 1) == '-' && *(stringChar - 2) == '-')
            {
                isComment = false;
                commentString += *stringChar;

                if (commentString.substr(3, commentString.length() - 5).find("--") != -1)
                {
                    LOG("The string \"--\" is not valid inside comments");
                    return false;
                }

                // Add comment to list
                if(listWithAllData != nullptr && !commentString.empty())
                    listWithAllData->push_back(commentString);

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

                // Check string between tags to valid
                std::string trimmedValueString = TrimString(betweenTagsString);
                if (!CheckString(trimmedValueString))
                {
                    LOG("Invalid symbol on line: " + std::to_string(lineNumber));
                    return false;
                }
                
                // Add string between tag to list
                if (!TrimString(betweenTagsString).empty() && listWithAllData != nullptr)
                    listWithAllData->push_back(TrimString(betweenTagsString));
                    
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

                if (listWithAllData != nullptr)
                    listWithAllData->push_back("<" + tagString + ">");
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
            if (listWithAllData != nullptr)
                listWithAllData->push_back(TrimString(betweenTagsString));
            betweenTagsString.clear();
        }
    }
    
    // Check if it is some unclosed tags
    if (!tagStringsStack.empty())
    {
        LOG("Dont closed tags")
        return false;
    }
    else
        return true;
}