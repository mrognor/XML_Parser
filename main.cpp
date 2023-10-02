#include "XML_Parser.h"
#include "XML_Functions.h"

int main()
{
    xmlp::XmlParser p;

    if (p.ValidateFile("xml_files/parsing_tests.xml"))
        std::cout << "true" << std::endl;
    else 
        std::cout << "false" << std::endl;

    std::cout << std::endl;
    
    std::list<xmlp::XmlData>::iterator beg, end;

    for (auto it = p.GetData().begin(); it != p.GetData().end(); ++it)
    {
        if (it->TagName == "tagC" && it->DataType == xmlp::openingTag)
        {
            beg = ++it;
            it--;
        }

        if (it->TagName == "tagC" && it->DataType == xmlp::closingTag)
            end = it;
        

        if (it->TagName == "tagA" && it->DataType == xmlp::closingTag)
            p.InsertData(it, {"<correctData> Correct text </correctData>"});           
    }

    p.InsertData(beg, end, {"<addingTag>"}, {"</addingTag>"});
    
    for (const auto& it : p.GetData())
        std::cout << it << std::endl;

    p.WriteDataToFile("bin/1.xml");
}