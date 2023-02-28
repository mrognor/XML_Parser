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
    
    return Validate(begin, end, Data);
}

bool XmlParser::ValidateString(const std::string& str)
{
    std::vector<std::string> vec = {str};
    return ValidateContainerWithStrings(vec);
}

bool XmlParser::ValidateVectorOfStrings(const std::vector<std::string>& vectorToValidate)
{
    return Validate(vectorToValidate.begin(), vectorToValidate.end(), Data);
}