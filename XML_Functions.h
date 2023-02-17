#pragma once

#include <map>
#include <string>
#include <vector>
#include <iostream>

std::vector<std::string> Split(const std::string& StringToSplit, const std::string& SplitterString);

// This function delete all spaces and tabs from line beggining and ending
std::string TrimString(const std::string& str);

// Check if this string can be valid tag name
bool CheckTagName(const std::string& tagString);

// Check if this string can be valid tag
bool ParseTagString(const std::string& tagString, std::string* tagName = nullptr, std::map<std::string, std::string>* paramsAndValues = nullptr);