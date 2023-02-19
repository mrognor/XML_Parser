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
    if (str.length() == 0)
        return "";
        
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


bool CheckTagAndParamName(const std::string& tagString)
{
    if (tagString == "")
    {
        LOG("Tag or param name can not be empty");
        return false;
    }

    // Check first symbol on string
    if ((isalpha(tagString[0]) || isalnum(tagString[0]) || tagString[0] == '_') == false)
    {
        LOG("Symbol: \"" + std::string(1, tagString[0]) + "\" not allowed in tag or param name");
        return false;
    }

    for (auto ch : tagString)
    {
        // Check all available symbols in tag name
        if ((isalpha(ch) || isalnum(ch) || ch == '.' || ch == '_' || ch == '-') == false)
        {
            LOG("Symbol: \"" + std::string(1, ch) + "\" not allowed in tag or param name");
            return false;
        }
    }
    return true;
}


bool CheckString(const std::string& valueString)
{
    for (auto it = valueString.begin(); it != valueString.end(); it++)
    {
        if (*it == '<')
        {
            LOG("Symbol \"<\" not allowed in a param value");
            return false;
        }

        // Check ampersands in string
        if(*it == '&')
        {
            // 1 - &lt; that equals "<" symbol
            // 2 - &gt; that equals ">" symbol
            // 3 - &amp; that equals "&" symbol
            // 4 - &apos; that equals "'" symbol
            // 5 - &quot; that equals """ symbol
            int ampersandLineCode = -1;

            // Check if it is & and at least 3 symbols before end
            if((it + 1) != valueString.end() && (it + 2) != valueString.end() && (it + 3) != valueString.end())
            {
                // &lt; check
                if (*(it + 1) == 'l' && *(it + 2) == 't' && *(it + 3) == ';')
                    ampersandLineCode = 1;
                
                // &gt; check
                if (*(it + 1) == 'g' && *(it + 2) == 't' && *(it + 3) == ';')
                    ampersandLineCode = 2;
                
                // Check if it is & and at least 4 symbols before end
                if ((it + 4) != valueString.end())
                {
                    // &amp; check
                    if (*(it + 1) == 'a' && *(it + 2) == 'm' && *(it + 3) == 'p' && *(it + 4) == ';')
                        ampersandLineCode = 3;
                    
                    // Check if it is & and at least 5 symbols before end
                    if ((it + 5) != valueString.end())
                    {
                        // &apos; check
                        if (*(it + 1) == 'a' && *(it + 2) == 'p' && *(it + 3) == 'o' && *(it + 4) == 's' && *(it + 5) == ';')
                            ampersandLineCode = 4;

                        // &quot; check
                        if (*(it + 1) == 'q' && *(it + 2) == 'u' && *(it + 3) == 'o' && *(it + 4) == 't' && *(it + 5) == ';')
                            ampersandLineCode = 5;
                    }
                }
            }

            if (ampersandLineCode == -1)
            {
                LOG("Wrong ampersand usage");
                return false;
            }
        }
    }

    return true;
}


bool ParseTagString(const std::string& tagString, std::string& tagName, std::map<std::string, std::string>& paramsAndValues)
{   
    // Check if tag starts with a space(" ") or a tab("\t")
    if (tagString[0] == ' ' || tagString[0] == '\t')
    {
        LOG("Tag cannot start with a space or a tab");
        return false;
    }

    // Find tag name
    std::string trimmedTagString = TrimString(tagString);
    std::string tmpTagName = trimmedTagString.substr(0, trimmedTagString.find(" "));

    // Check tag name validity and set pointer value if it is not nullptr
    if (CheckTagAndParamName(tmpTagName))
        tagName = tmpTagName;
    else
        return false;    
    
    // Check if it only tag name without params
    if (tagString == tmpTagName)
        return true;

    // Substring without tag name
    std::string trimmedParamsLine = TrimString(tagString.substr(tagString.find(" ")));
    
    bool isSettingParam = false;
    bool isParamValue = false;
    char currentValueSplitter;

    std::string lastString = "";
    std::string paramName;

    std::map<std::string, std::string> tmpParamsAndValues;

    int paramsCounter = 0;
    int valuesCounter = 0;

    for (auto it = trimmedParamsLine.begin(); it != trimmedParamsLine.end(); it++)
    {
        // Opening param value setting "=" symbol
        if (isSettingParam == false && *it == '=')
        {
            isSettingParam = true;
            paramName = TrimString(lastString);
            
            paramsCounter++;
            lastString = "";

            if (!CheckTagAndParamName(paramName))
                return false;

            continue;
        }

        // Opening param value "'" or """ symbol
        if (isSettingParam == true && isParamValue == false && (*it == '"' || *it == '\''))
        {
            isParamValue = true;

            if (!TrimString(lastString).empty())
            {
                LOG("Symbols without \" or ' between param name and param value");
                return false;
            }

            lastString = "";

            if (*it == '"')
                currentValueSplitter = '"';
            else
                currentValueSplitter = '\'';

            continue;
        }

        // Closing param value "'" or """ symbol
        if (isSettingParam == true && isParamValue == true && *it == currentValueSplitter)
        {
            isSettingParam = false;
            isParamValue = false;

            valuesCounter++;

            if (!CheckString(lastString))
                return false;

            tmpParamsAndValues.insert(std::pair<std::string, std::string>(paramName, lastString));

            paramName = "";
            lastString = "";

            continue;
        }

        lastString += *it;
    }

    if (paramsCounter != valuesCounter || !lastString.empty())
    {
        LOG("Params counter not equal values counter");
        return false;
    }

    paramsAndValues = tmpParamsAndValues;
    return true;
}