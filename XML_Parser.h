#pragma once

#include <stack>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>

#include "XML_Functions.h"

namespace xmlp
{
    class XmlParser
    {
    private:
        std::list<XmlData> Data;
        bool IsPathSaving = true;
    public:
        /*!
            \brief This function is needed to set the path of each tag to be saved.
            
            Disabling the saving of the tag path is necessary to save space in RAM, 
            since the path takes up more space than the tag name, 
            storing the path spends a lot of memory.
            At the same time it is very easy to determine the path during tag processing
        */
        void SetPathSaving(bool isPathSaving) { IsPathSaving = isPathSaving; }

        std::list<XmlData>& GetData() { return Data; }

        bool ValidateFile(const std::string& fileName);

        bool ValidateString(const std::string& str);

        bool ValidateVectorOfStrings(const std::vector<std::string>& vectorToValidate);

        template <class T>
        bool ValidateContainerWithStrings(const T& containerToValidate);

        // Insert data before iterator like in std
        bool InsertData(const std::list<XmlData>::iterator& posToInsertIt, std::list<std::string> listToInsert);

        // Insert data before iterator like in std
        // Warning, data copieng. May end your RAM.
        bool InsertData(const std::list<XmlData>::iterator& firstPosToInsertIt, const std::list<XmlData>::iterator& secondPosToInsertIt, 
        std::list<std::string> firstListToInsert, std::list<std::string> secondListToInsert);

        bool WriteDataToFile(std::string fileName);
    };


    template <class T>
    bool XmlParser::ValidateContainerWithStrings(const T& containerToValidate)
    {
        return Validate(containerToValidate.begin(), containerToValidate.end(), &Data);
    }
}