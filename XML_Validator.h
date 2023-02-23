#pragma once

#include <stack>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <iterator>

#include "XML_Functions.h"

class XmlValidator
{
private:
public:
    bool ValidateFile(const std::string& fileName);

    bool ValidateString(const std::string& str);

    bool ValidateVectorOfString(const std::vector<std::string>& vectorToValidate);
};