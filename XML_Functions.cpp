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

bool CheckTagLine(const std::string& tagLine)
{   
    std::string tagName = tagLine.substr(0, tagLine.find(" "));
    std::cout << tagName << "____endline" << std::endl;
    // Substring without tag name
    std::string trimmedTagLine = TrimString(tagLine.substr(tagLine.find(" ")));
    std::cout << trimmedTagLine << "____endline" << std::endl;

    bool IsSettingParam = false;
    bool IsParam = false;
    for (auto it = trimmedTagLine.begin(); it != trimmedTagLine.end(); it++)
    {

    }
}