/** @file */
/// \cond HIDDEN_SYMBOLS

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

/// \endcond

/*!
    Function for setting the logging function

    \param[in] logFuncPtr custom logging function
*/
void SetLogFunc(void (*logFuncPtr)(std::string));

/*!
    Function to split a string using a splitter string

    \param[in] stringToSplit The string to be splitted
    \param[in] splitterString The string that is used as a splitter

    \return Returns a vector of string consisting of parts of the stringToSplit
*/
std::vector<std::string> Split(const std::string& stringToSplit, const std::string& splitterString);

/*!
    This function delete all spaces and tabs from line beggining and ending

    \param[in] str The string to trim
    \param[in] isFromLeft The function will delete all spaces from left
    \param[in] isFromRight The function will delete all spaces from right

    \return Returns a trimmed string
*/
std::string TrimString(const std::string& str, bool isFromLeft = true, bool isFromRight = true);

/*!
    This function checks the tag name or the name of its parameter for validity. 
    If it is valid, it will return true, otherwise false

    \param[in] tagString string with tag name or tag parametr name

    \return Return true if this string can be valid tag name or parametr name, return false otherwise
*/
bool CheckTagAndParamName(const std::string& tagString);

/*!
    This function checks the parameter value or strings between tags for validity. 
    If it is valid, it will return true, otherwise false

    \param[in] valueString string with parameter value or strings between tags

    \return Return true if this string can be valid parameter value or strings between tags, return false otherwise
*/
bool CheckString(const std::string& valueString);

/*!
    This function checks the tag string for validity. 
    If it is valid, it will return true, otherwise false

    \param[in] tagString string with tag
    \param[out] tagName string to put tag name if tag string valid
    \param[out] paramsAndValues map to put all params and values if tag string valid

    \return Return true if this string can be valid tag, return false otherwise
*/
bool ParseTagString(const std::string& tagString, std::string& tagName, std::map<std::string, std::string>& paramsAndValues);