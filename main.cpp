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

    // std::string tagName;
    // std::map<std::string, std::string> tagParams;

    // if (ParseTagString("tag.1-2_3  val = 'par&lt;hgh;'", tagName, tagParams))
    //     std::cout << "true" << std::endl;
    // else 
    //     std::cout << "false" << std::endl;
    
    // std::cout << "Tag name: "<< tagName << std::endl;
    
    // for (auto it : tagParams)
    //     std::cout << "Param: " << it.first << " Value: " << it.second << std::endl;

    // To-Do
    // 1. -- inside comment
    // 2. Only one root element check
    // 3. XML setup line ignore
}