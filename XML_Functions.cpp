#include "XML_Functions.h"

void (*LogFuncPtr)(std::string) = nullptr;


void SetLogFunc(void (*logFuncPtr)(std::string))
{
    LogFuncPtr = logFuncPtr; 
}


std::vector<std::string> Split(const std::string& StringToSplit, const std::string& SplitterString)
{
    std::vector<std::string> ReturnVector;
    int i = 0;
    std::string SplittedString = "";
    while (i < StringToSplit.size())
    {
        if (StringToSplit[i] == SplitterString[0])
        {
            bool IsSplitter = true;
            for (int j = 1; j < SplitterString.size(); j++)
            {
                if (StringToSplit[i + j] != SplitterString[j])
                {
                    IsSplitter = false;
                    break;
                }
            }
            if (IsSplitter)
            {
                ReturnVector.push_back(SplittedString);
                SplittedString = "";
                i += SplitterString.size();
                continue;
            }
        }
        SplittedString += StringToSplit[i];
        i++;
    }
    ReturnVector.push_back(SplittedString);
    return ReturnVector;
}


std::string TrimString(const std::string& str, bool isFromLeft, bool isFromRight)
{
    int start = 0;

    if (isFromLeft)
    {
        while (str[start] == ' ' || str[start] == '\t')
            start++;
    }

    int end = str.length();

    if (isFromRight)
    {
        while (str[end - 1] == ' ' || str[end - 1] == '\t')
            end--;
    }

    return str.substr(start, end - start);
}


bool CheckTagName(const std::string& tagString)
{
    // Check first symbol on string
    if ((isalpha(tagString[0]) || isalnum(tagString[0]) || tagString[0] == '_') == false)
    {
        LOG("Symbol: \"" + std::string(1, tagString[0]) + "\" not allowed in tag name");
        return false;
    }

    for (auto ch : tagString)
    {
        // Check all available symbols in tag name
        if ((isalpha(ch) || isalnum(ch) || ch == '.' || ch == '_' || ch == '-') == false)
        {
            LOG("Symbol: \"" + std::string(1, ch) + "\" not allowed in tag name");
            return false;
        }
    }
    return true;
}


bool ParseTagString(const std::string& tagString, std::string* tagName, std::map<std::string, std::string>* paramsAndValues)
{   
    // Check if tag starts with space(" ")
    if (tagString[0] == ' ' || tagString[0] == '\t')
    {
        LOG("Tag cannot start with a space or a tab");
        return false;
    }

    // Find tag name
    std::string trimmedTagString = TrimString(tagString);
    std::string tmpTagName = trimmedTagString.substr(0, trimmedTagString.find(" "));

    // Check tag name validity and set pointer value if it is not nullptr
    if (!CheckTagName(tmpTagName))
        return false;
    else
        if (tagName != nullptr)
            *tagName = tmpTagName;
    
    return true;
    // Substring without tag name
    // std::string trimmedTagLine = TrimString(tagString.substr(tagString.find(" ")));
    
    // bool isSettingParam = false;
    // bool isParamValue = false;
    // char currentValueSplitter;

    // std::string lastString = "";
    // std::string paramName;

    // for (auto it = trimmedTagLine.begin(); it != trimmedTagLine.end(); it++)
    // {
    //     // Opening param value setting "=" symbol
    //     if (isSettingParam == false && *it == '=')
    //     {
    //         isSettingParam = true;
    //         paramName = TrimString(lastString);
    //         lastString = "";
    //         continue;
    //     }

    //     // Opening param value "'" or """ symbol
    //     if (isSettingParam == true && isParamValue == false && (*it == '"' || *it == '\''))
    //     {
    //         isParamValue = true;
    //         lastString = "";

    //         if (*it == '"')
    //             currentValueSplitter = '"';
    //         else
    //             currentValueSplitter = '\'';

    //         continue;
    //     }

    //     // Closing param value "'" or """ symbol
    //     if (isSettingParam == true && isParamValue == true && *it == currentValueSplitter)
    //     {
    //         isSettingParam = false;
    //         isParamValue = false;

    //         if (paramsAndValues != nullptr)
    //             paramsAndValues->insert(std::pair<std::string, std::string>(paramName, lastString));

    //         paramName = "";
    //         lastString = "";

    //         continue;
    //     }

    //     lastString += *it;
    // }

    return true;
}