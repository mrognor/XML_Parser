#pragma once

#include <stack>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>

#include "XML_Functions.h"

class XmlParser
{
private:
    std::list<XmlData> Data;
public:
    std::list<XmlData>& GetData() { return Data; }

    bool ValidateFile(const std::string& fileName);

    bool ValidateString(const std::string& str);

    bool ValidateVectorOfStrings(const std::vector<std::string>& vectorToValidate);

    template <class T>
    bool ValidateContainerWithStrings(const T& containerToValidate);

    // Insert data before iterator like in std
    // bool InsertData(std::list<std::string>::iterator posToInsertIt, std::list<std::string> listToInsert);

    bool WriteDataToFile(std::string fileName);
};


template <class T>
bool XmlParser::ValidateContainerWithStrings(const T& containerToValidate)
{
    return Validate(containerToValidate.begin(), containerToValidate.end(), &Data);
}