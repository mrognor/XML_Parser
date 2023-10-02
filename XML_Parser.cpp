#include "XML_Parser.h"

namespace xmlp
{
    // This class required only to extract lines from files with iterators
    class line : public std::string {};

    // Define operator stream reading from file to line class
    // Required because if read to string all spaces will be stop reading line
    std::istream &operator>>(std::istream &is, line &l)
    {
        std::getline(is, l);
        return is;
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
        
        return Validate(begin, end, &Data);
    }

    bool XmlParser::ValidateString(const std::string& str)
    {
        std::vector<std::string> vec = {str};
        return ValidateContainerWithStrings(vec);
    }

    bool XmlParser::ValidateVectorOfStrings(const std::vector<std::string>& vectorToValidate)
    {
        return Validate(vectorToValidate.begin(), vectorToValidate.end(), &Data);
    }

    bool XmlParser::InsertData(const std::list<XmlData>::iterator& posToInsertIt, std::list<std::string> listToInsert)
    {
        if (posToInsertIt == std::list<xmlp::XmlData>::iterator())
            return false;

        std::list<XmlData> listWithInsertData;
        listToInsert.emplace_front("<VALIDATION_ROOT_TAG>");
        listToInsert.emplace_back("</VALIDATION_ROOT_TAG>");

        // Check if it appending valid data
        if (Validate(listToInsert.begin(), listToInsert.end(), &listWithInsertData))
        {
            for (auto it = ++listWithInsertData.begin(); it != --listWithInsertData.end(); ++it)
            {
                std::string properPath = it->Path.substr(it->Path.find('/') + 1);
                properPath = properPath.substr(properPath.find('/') + 1);
                
                if (posToInsertIt->DataType == closingTag)
                    properPath = properPath + posToInsertIt->TagName + "/";
                
                it->Path = posToInsertIt->Path + properPath;
            }

            Data.insert(posToInsertIt, ++listWithInsertData.begin(), --listWithInsertData.end());
            return true;
        }
        else
            return false;
    }

    bool XmlParser::InsertData(const std::list<XmlData>::iterator& firstPosToInsertIt, const std::list<XmlData>::iterator& secondPosToInsertIt, 
        std::list<std::string> firstListToInsert, std::list<std::string> secondListToInsert)
    {
        if (firstPosToInsertIt == std::list<xmlp::XmlData>::iterator() || secondPosToInsertIt == std::list<xmlp::XmlData>::iterator())
            return false;

        std::list<XmlData> listWithInsertData;

        Data.insert(firstPosToInsertIt, firstListToInsert.begin(), firstListToInsert.end());
        Data.insert(secondPosToInsertIt, secondListToInsert.begin(), secondListToInsert.end()); 

        // Check if it appending valid data
        if (Validate(Data.begin(), Data.end(), &listWithInsertData))
        {
            Data = listWithInsertData;
            return true;
        }
        else
            return false;
    }

    bool XmlParser::WriteDataToFile(std::string fileName)
    {
        std::ofstream file(fileName.c_str());

        if(!file.is_open())
        {
            LOG("Failed to open file to write");
            file.close();
            return false;
        }

        for (const auto& it : Data)
        {
            int tabCount = Count(it.Path, '/');

            for (int i = 1; i < tabCount; ++i)
                file << '\t';
            file << it.Data << std::endl;
        }
        
        file.close();
        return true;
    }

    bool XmlParser::FindInlineElement(std::string tagName, std::list<XmlData>::iterator& iterator)
    {
        for (auto it = Data.begin(); it != Data.end(); ++it)
        {
            if (it->TagName == tagName && it->DataType == xmlp::inlineTag)
            {
                iterator = it;
                return true;
            }        
        }

        iterator = Data.end();
        return false;
    }

    bool XmlParser::FindOpeningElement(std::string tagName, std::list<XmlData>::iterator& iterator)
    {
        for (auto it = Data.begin(); it != Data.end(); ++it)
        {
            if (it->TagName == tagName && it->DataType == xmlp::openingTag)
            {
                iterator = it;
                return true;
            }        
        }

        iterator = Data.end();
        return false;
    }

    bool XmlParser::FindClosingElement(std::string tagName, std::list<XmlData>::iterator& iterator)
    {
        for (auto it = Data.begin(); it != Data.end(); ++it)
        {
            if (it->TagName == tagName && it->DataType == xmlp::closingTag)
            {
                iterator = it;
                return true;
            }
        }

        iterator = Data.end();
        return false;
    }
}