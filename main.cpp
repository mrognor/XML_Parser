#include "XML_Validator.h"
#include "XML_Parser.h"
#include "XML_Functions.h"


int main()
{
    XmlValidator v;
    if (v.ValidateFile("tests.xml"))
        std::cout << "true" << std::endl;
    else 
        std::cout << "false" << std::endl;

    // for (auto it : ParseTagString("tagB     mo = \" gw rgeb ===gbeb 'gvberb regbe's\"   gr    'gs'      "))
    //     std::cout << "Param name: " << it.first << " param value: " << it.second << "____END" << std::endl;

    // To-Do
    // 1. -- inside comment
    // 2. Only one root element check
    // 3. XML setup line ignore
}