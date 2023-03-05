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

// bool XmlParser::InsertData(std::list<std::string>::iterator posToInsertIt, std::list<std::string> listToInsert)
// {
//     // Insert user added strings to Data list
//     auto newStart = Data.insert(posToInsertIt, listToInsert.begin(), listToInsert.end());

//     // Check if it is valid XML document
//     if (Validate(Data.begin(), Data.end()))
//     {
//         // Remove user added strings from data list to properly parse it to list with strings
//         Data.erase(newStart, posToInsertIt);
        
//         // Add validation tags to insert list to check its validity inside tags
//         listToInsert.push_front("<ValidationTag>");
//         listToInsert.push_back("</ValidationTag>");
        
//         // Properly parsed user added list with strings
//         std::list<std::string> parsedListToInsert;
//         // Parse user added list
//         Validate(listToInsert.begin(), listToInsert.end(), &parsedListToInsert);

//         // Remove validation tags
//         parsedListToInsert.pop_back();
//         parsedListToInsert.pop_front();

//         Data.insert(posToInsertIt, parsedListToInsert.begin(), parsedListToInsert.end());

//         return true;
//     }
//     else
//     {
//         Data.erase(newStart, posToInsertIt);
//         return false;
//     }
// }

void XmlParser::WriteDataToFile(std::string fileName)
{
    std::ofstream file(fileName.c_str());

    if(!file.is_open())
    {
        LOG("Failed to open file to write");
        return;
    }

    // QueryData([&file](XmlDataType dataType, std::string path, std::string tagName, 
    //     std::map<std::string, std::string> paramsAndValues, std::list<std::string>::iterator data)
    // {
    //     int shift = Count(path, '/');
        
    //     if (!tagName.empty())
    //         shift--;

    //     for (int i = 0; i < shift; i++)
    //         file << "\t";
            
    //     file << *data << std::endl;
    // });
}