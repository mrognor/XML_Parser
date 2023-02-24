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
    std::list<std::string> Data;
public:
    const std::list<std::string>& getData() { return Data; }

    bool ValidateFile(const std::string& fileName);

    bool ValidateString(const std::string& str);

    bool ValidateVectorOfString(const std::vector<std::string>& vectorToValidate);
};