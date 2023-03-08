#include "XML_Parser.h"
#include "XML_Functions.h"

int main()
{
    XmlParser p;

    std::cout << "Valid file check: ";
    if (p.ValidateFile("auto_tests/valid_file.xml"))
        std::cout << "Success!" << std::endl;
    else 
        std::cout << "Failed!" << std::endl;

    std::cout << std::endl;
}