#include "XML_Parser.h"
#include "XML_Functions.h"

void f(DataType dataType, std::string path, std::string tagName, 
std::map<std::string, std::string> paramsAndValues, std::string data)
{
    switch (dataType)
    {
    case openingTag:
        std::cout << "Opening tag: " << tagName << " Path: " << path << std::endl;

        if (!paramsAndValues.empty())
        {
            std::cout << "Params:" << std::endl;
            for (auto it : paramsAndValues)
                std::cout << "\tParam name: " << it.first << " Param value: " << it.second << std::endl; 
        } 
        break;

    case inlineTag:
        std::cout << "Inline tag: " << tagName << " Path: " << path << std::endl;
        if (!paramsAndValues.empty())
        {
            std::cout << "Params:" << std::endl;
            for (auto it : paramsAndValues)
                std::cout << "\tParam name: " << it.first << " Param value: " << it.second << std::endl; 
        } 
        break;
    
    case closingTag:
        std::cout << "Closing tag: " << tagName << " Path: " << path << std::endl;
        break;
    
    case text:
        std::cout << "Text: " << data << " Path: " << path << std::endl;
        break;
    }
}

int main()
{
    // // Line with xml specification ignoring and dont checking
    // XmlParser p;
    // if (p.ValidateFile("xml_files/validity_tests.xml"))
    //     std::cout << "true" << std::endl;
    // else 
    //     std::cout << "false" << std::endl;

    // for (const auto& it : p.GetData())
    //     std::cout << it << std::endl;

    // std::cout << std::endl;

    // if (p.ValidateVectorOfString({"<tagA>", "<tagB p='fff'>", "<tagC p='fffff'/>", "</tagB>", "</tagA>"}))
    //     std::cout << "true" << std::endl;
    // else 
    //     std::cout << "false" << std::endl;

    // for (const auto& it : p.GetData())
    //     std::cout << it << std::endl;

    // std::cout << std::endl;

    // if (p.ValidateString("<tagA><tagB p='fff'><tagC p='fffff'/></tagB></tagA>"))
    //     std::cout << "true" << std::endl;
    // else 
    //     std::cout << "false" << std::endl;

    // for (const auto& it : p.GetData())
    //     std::cout << it << std::endl;

    XmlParser p;

    if (p.ValidateFile("xml_files/parsing_tests.xml"))
        std::cout << "true" << std::endl;
    else 
        std::cout << "false" << std::endl;

    for (const auto& it : p.GetData())
        std::cout << it << std::endl;

    p.QueryData(f);

    std::cout << std::endl;

    p.QueryData([](DataType dataType, std::string path, std::string tagName, 
        std::map<std::string, std::string> paramsAndValues, std::string data)
        {
            std::cout << "Data path: " << path << " Data: " << data << std::endl;
        });

    std::cout << std::endl;

    
    p.QueryData([](DataType dataType, std::string path, std::string tagName, 
        std::map<std::string, std::string> paramsAndValues, std::string data)
        {
            static bool isTagE = false;

            if (tagName == "tagE" && dataType == closingTag)
            {
                isTagE = false;
                return;
            }

            if (tagName == "tagE" && dataType == openingTag)
            {
                isTagE = true;
                return;
            }

            if (isTagE)
                std::cout << data << std::endl;
        });

    std::cout << Validate(p.GetData().begin(), p.GetData().end()) << std::endl;
}