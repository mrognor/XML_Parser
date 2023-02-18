#pragma once

#include <map>
#include <string>
#include <vector>
#include <iostream>

// Pointer to log function. By default set to nullptr inside XML_Functions.cpp
// Can be defined by user using SetLogFunc
extern void (*LogFuncPtr)(std::string);

#ifndef NDEBUG
#define LOG(error) \
    if (LogFuncPtr == nullptr) \
        std::cerr << error << std::endl; \
    else \
        LogFuncPtr(error);
#else
#define LOG(error)
#endif

// Function to set log function. Gets pointer to function with signature: void(std::string)
void SetLogFunc(void (*logFuncPtr)(std::string));

std::vector<std::string> Split(const std::string& StringToSplit, const std::string& SplitterString);

// This function delete all spaces and tabs from line beggining and ending.
// This function have got 2 params to set sides to trim string.
// isFromLeft means that function will delete all spaces from left
// isFromRight means that function will delete all spaces from right
std::string TrimString(const std::string& str, bool isFromLeft = true, bool isFromRight = true);

// Check if this string can be valid tag name
bool CheckTagName(const std::string& tagString);

// Check if this string can be valid tag
bool ParseTagString(const std::string& tagString, std::string* tagName = nullptr, std::map<std::string, std::string>* paramsAndValues = nullptr);