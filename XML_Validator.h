#pragma once

#include <stack>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "XML_Functions.h"

class XmlValidator
{
private:
public:
    bool ValidateFile(const std::string& fileName);

    bool ValidateVectorOfString(std::vector<std::string>& vectorToValidate);
};