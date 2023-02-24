#include "XML_Parser.h"
#include "XML_Functions.h"


int main()
{
    // Line with xml specification ignoring and dont checking
    XmlParser p;
    if (p.ValidateFile("xml_files/validity_tests.xml"))
        std::cout << "true" << std::endl;
    else 
        std::cout << "false" << std::endl;

    for (const auto& it : p.getData())
        std::cout << it << std::endl;

    std::cout << std::endl;

    if (p.ValidateVectorOfString({"<tagA>", "<tagB p='fff'>", "<tagC p='fffff'/>", "</tagB>", "</tagA>"}))
        std::cout << "true" << std::endl;
    else 
        std::cout << "false" << std::endl;

    for (const auto& it : p.getData())
        std::cout << it << std::endl;

    std::cout << std::endl;

    if (p.ValidateString("<tagA><tagB p='fff'><tagC p='fffff'/></tagB></tagA>"))
        std::cout << "true" << std::endl;
    else 
        std::cout << "false" << std::endl;

    for (const auto& it : p.getData())
        std::cout << it << std::endl;

    std::cout << std::endl;

    if (p.ValidateFile("xml_files/parsing_tests.xml"))
        std::cout << "true" << std::endl;
    else 
        std::cout << "false" << std::endl;

    for (const auto& it : p.getData())
        std::cout << it << std::endl;
}