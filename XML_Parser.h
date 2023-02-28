#pragma once

#include <stack>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>

#include "XML_Functions.h"

enum DataType
{
    empty = -1,
    openingTag = 0,
    closingTag = 1,
    inlineTag = 2,
    text = 3
};

class XmlParser
{
private:
    std::list<std::string> Data;
public:
    std::list<std::string>& GetData() { return Data; }

    bool ValidateFile(const std::string& fileName);

    bool ValidateString(const std::string& str);

    bool ValidateVectorOfStrings(const std::vector<std::string>& vectorToValidate);

    template <class T>
    bool ValidateContainerWithStrings(const T& containerToValidate);

    // Insert data before iterator like in std
    bool InsertData(std::list<std::string>::iterator posToInsertIt, std::list<std::string> listToInsert);

    void WriteDataToFile(std::string fileName);
    
    template <class T>
    void QueryData(T func);
};

template <class T>
void XmlParser::QueryData(T func)
{
    std::string path;
    std::string tagName;
    std::string tagString;
    DataType dataType;
    std::map<std::string, std::string> paramsAndValues;

    for (std::list<std::string>::iterator it = Data.begin(); it != Data.end(); it++)
    {
        tagName.clear();
        dataType = empty;
        paramsAndValues.clear();
        tagString.clear();

        // Is tag
        if((*it).length() >= 3 && (*it)[0] == '<' && (*it)[(*it).length() - 1] == '>')
        {
            dataType = openingTag;
            tagString = (*it).substr(1, (*it).length() - 2);

            // Closing tag
            if ((*it).length() >= 4 && (*it)[1] == '/')
            {
                dataType = closingTag;
                tagString = tagString.substr(1);
            }
            // Inline tag
            if ((*it).length() >= 4 && (*it)[(*it).length() - 2] == '/')
            {
                dataType = inlineTag;
                tagString = tagString.substr(0, tagString.length() - 1);
            }
        }
        else 
            dataType = text;

        // If opening or inline tag
        if (dataType == 0 || dataType == 2)
        {
            ParseTagString(tagString, tagName, paramsAndValues);
            path += "/" + tagName;
        }
        
        // If closing tag
        if (dataType == 1)
            ParseTagString(tagString, tagName, paramsAndValues);
        

        // Call user specified function with params:
        // 1 - data type, enum with information about current data type
        // 2 - string with current data path
        // 3 - tag name if in data stored tag
        // 4 - data itself
        func(dataType, path, tagName, paramsAndValues, it);

        // Remove closing tag from path
        if (dataType == 1)
            path = path.substr(0, path.length() - 1 - tagName.length());

        // Remove inline tag
        if (dataType == 2)
        {
            path = path.substr(0, path.length() - 1 - tagName.length());
        }
    }
}

template <class T>
bool XmlParser::ValidateContainerWithStrings(const T& containerToValidate)
{
    return Validate(containerToValidate.begin(), containerToValidate.end(), &Data);
}