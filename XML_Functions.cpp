#include "XML_Functions.h"

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

std::string TrimString(const std::string& str)
{
    int start = 0;

    while (str[start] == ' ' || str[start] == '\t')
        start++;

    int end = str.length();

    while (str[end - 1] == ' ' || str[end - 1] == '\t')
        end--;

    return str.substr(start, end - start);
}

bool CheckTagName(const std::string& tagString)
{

}

bool ParseTagString(const std::string& tagString, std::string* tagName, std::map<std::string, std::string>* paramsAndValues)
{   
    // Check if tag starts with space(" ")
    if (tagString[0] == ' ')
        return false;

    // Check if it is tag without params and with good tag name
    if (tagString.find(" ") == -1 && tagString.find("<") == -1 && tagString.find("&") == -1)
    {
        if (tagName != nullptr)
            *tagName = tagString.substr(0, tagString.find(" "));
        return true;
    }

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