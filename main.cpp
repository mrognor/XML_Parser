#include "XML_Validator.h"
#include "XML_Parser.h"
#include "XML_Functions.h"


int main()
{
    // Line with xml specification ignoring and dont checking
    XmlValidator v;
    if (v.ValidateFile("tests.xml"))
        std::cout << "true" << std::endl;
    else 
        std::cout << "false" << std::endl;
}