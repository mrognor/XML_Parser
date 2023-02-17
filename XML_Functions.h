#pragma once

#include <string>
#include <vector>
#include <iostream>

std::vector<std::string> Split(const std::string& StringToSplit, const std::string& SplitterString);

// This function delete all spaces and tabs from line beggining and ending
std::string TrimString(const std::string& str);

bool CheckTagLine(const std::string& tagLine);

