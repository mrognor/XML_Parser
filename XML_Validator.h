#pragma once

#include <stack>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

class XmlValidator
{
private:
public:
    bool ValidateFile(const std::string& fileName);
};