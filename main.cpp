#include "XML_Parser.h"
#include "XML_Functions.h"

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

    std::cout << std::endl;
    
    for (auto it = p.GetData().begin(); it != p.GetData().end(); it++)
    {
        if (it->TagName == "tagA" && it->DataType == closingTag)
            p.InsertData(it, {"<correctData> Correct text </correctData>"});           
    }

    for (const auto& it : p.GetData())
        std::cout << it << std::endl;

    p.WriteDataToFile("bin/1.xml");
}