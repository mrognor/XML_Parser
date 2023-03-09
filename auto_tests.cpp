#include "XML_Parser.h"
#include "XML_Functions.h"

void log(std::string) {}

int main()
{
    xmlp::XmlParser p;
    
    xmlp::SetLogFunc(log);

    std::cout << "Testing started!" << std::endl;
    std::cout << "Valid file check: ";

    if (p.ValidateFile("xml_files/valid_file.xml"))
        std::cout << "\x1B[32m" << "Success!" << "\033[0m" << std::endl;
    else 
        std::cout << "\x1B[31m" << "Failed!" <<  "\033[0m" << std::endl;

    std::cout << std::endl;

    std::cout << "Invalid data check: " << std::endl;

    std::cout << std::endl;

    std::cout << "Invalid data type: Spec string started with space: ";
    if (!p.ValidateString(" <?xml version=\"1.0\" encoding=\"UTF-8\"?>"))
        std::cout << "\x1B[32m" <<  "Success! Checking invalid data return false!" << "\033[0m" << std::endl;
    else 
        std::cout << "\x1B[31m" << "Failed! Checking invalid data return true, but its not correct!" << "\033[0m" << std::endl;
    
    std::cout << std::endl;
    
    std::cout << "Invalid data type: Spec string started with tab: ";
    if (!p.ValidateString("\t<?xml version=\"1.0\" encoding=\"UTF-8\"?>"))
        std::cout << "\x1B[32m" << "Success! Checking invalid data return false!" << "\033[0m" << std::endl;
    else 
        std::cout << "\x1B[31m" << "Failed! Checking invalid data return true, but its not correct!" << "\033[0m" << std::endl;
    
    std::cout << std::endl;

    std::cout << "Invalid data type: Spec tag started with space: ";
    if (!p.ValidateString("<? xml version=\"1.0\" encoding=\"UTF-8\"?>"))
        std::cout << "\x1B[32m" << "Success! Checking invalid data return false!" << "\033[0m" << std::endl;
    else 
        std::cout << "\x1B[31m" << "Failed! Checking invalid data return true, but its not correct!" << "\033[0m" << std::endl;

    std::cout << std::endl;

    std::cout << "Invalid data type: Spec tag started with tab: ";
    if (!p.ValidateString("<?\txml version=\"1.0\" encoding=\"UTF-8\"?>"))
        std::cout << "\x1B[32m" << "Success! Checking invalid data return false!" << "\033[0m" << std::endl;
    else 
        std::cout << "\x1B[31m" << "Failed! Checking invalid data return true, but its not correct!" << "\033[0m" << std::endl;
    
    std::cout << std::endl;

    std::cout << "Invalid data type: Spec string not at file begining: ";
    if (!p.ValidateVectorOfStrings({"\n", "<?\txml version=\"1.0\" encoding=\"UTF-8\"?>"}))
        std::cout << "\x1B[32m" << "Success! Checking invalid data return false!" << "\033[0m" << std::endl;
    else 
        std::cout << "\x1B[31m" << "Failed! Checking invalid data return true, but its not correct!" << "\033[0m" << std::endl;

    std::cout << std::endl;

    std::cout << "Invalid data type: Comment line contain \"--\" string at start of comment: ";
    if (!p.ValidateString("<root> <!---- --> </root>"))
        std::cout << "\x1B[32m" << "Success! Checking invalid data return false!" << "\033[0m" << std::endl;
    else
        std::cout << "\x1B[31m" << "Failed! Checking invalid data return true, but its not correct!" << "\033[0m" << std::endl;

    std::cout << std::endl;

    std::cout << "Invalid data type: Comment line contain \"--\" string at middle of comment: ";
    if (!p.ValidateString("<root> <!-- -- --> </root>"))
        std::cout << "\x1B[32m" << "Success! Checking invalid data return false!" << "\033[0m" << std::endl;
    else
        std::cout << "\x1B[31m" << "Failed! Checking invalid data return true, but its not correct!" << "\033[0m" << std::endl;

    std::cout << std::endl;

    std::cout << "Invalid data type: Comment line contain \"--\" string at end of comment: ";
    if (!p.ValidateString("<root> <!-- ----> </root>"))
        std::cout << "\x1B[32m" << "Success! Checking invalid data return false!" << "\033[0m" << std::endl;
    else
        std::cout << "\x1B[31m" << "Failed! Checking invalid data return true, but its not correct!" << "\033[0m" << std::endl;

    std::cout << std::endl;

    std::cout << "Invalid data type: Dont closed comment at end of file: ";
    if (!p.ValidateString("<root><tagA> <!-- </tagB></root>"))
        std::cout << "\x1B[32m" << "Success! Checking invalid data return false!" << "\033[0m" << std::endl;
    else
        std::cout << "\x1B[31m" << "Failed! Checking invalid data return true, but its not correct!" << "\033[0m" << std::endl;

    std::cout << std::endl;

    std::cout << "Invalid data type: Two roots: ";
    if (!p.ValidateString("<root></root><root></root>"))
        std::cout << "\x1B[32m" << "Success! Checking invalid data return false!" << "\033[0m" << std::endl;
    else
        std::cout << "\x1B[31m" << "Failed! Checking invalid data return true, but its not correct!" << "\033[0m" << std::endl;

    std::cout << std::endl;

    std::cout << "Invalid data type: Dont closed tag: ";
    if (!p.ValidateString("<root><tagA></root>"))
        std::cout << "\x1B[32m" << "Success! Checking invalid data return false!" << "\033[0m" << std::endl;
    else
        std::cout << "\x1B[31m" << "Failed! Checking invalid data return true, but its not correct!" << "\033[0m" << std::endl;

    std::cout << std::endl;

    std::cout << "Invalid data type: Dont closed tag at end of file: ";
    if (!p.ValidateString("<root><tagA></tagA>"))
        std::cout << "\x1B[32m" << "Success! Checking invalid data return false!" << "\033[0m" << std::endl;
    else
        std::cout << "\x1B[31m" << "Failed! Checking invalid data return true, but its not correct!" << "\033[0m" << std::endl;

    std::cout << std::endl;

    std::cout << "Invalid data type: Dont opened tag: ";
    if (!p.ValidateString("<root></tagA></root>"))
        std::cout << "\x1B[32m" << "Success! Checking invalid data return false!" << "\033[0m" << std::endl;
    else
        std::cout << "\x1B[31m" << "Failed! Checking invalid data return true, but its not correct!" << "\033[0m" << std::endl;

    std::cout << std::endl;

    std::cout << "Invalid data type: Closing wrong tag: ";
    if (!p.ValidateString("<root><tagA></tagB></root>"))
        std::cout << "\x1B[32m" << "Success! Checking invalid data return false!" << "\033[0m" << std::endl;
    else
        std::cout << "\x1B[31m" << "Failed! Checking invalid data return true, but its not correct!" << "\033[0m" << std::endl;

    std::cout << std::endl;

    // Checking tag name
    bool isAllGoodWithTagNames = true;

    for (int i = 0; i < 256; i++)
    {
        std::string tagName = "<root" + std::string(1, (unsigned char)i);
        tagName += "> data </root";
        tagName += std::string(1, (unsigned char)i);
        tagName += ">";

        if (i == '_' || i == '-' || i == '.' || i == ' ' || i == '\t' || isalnum(i) || isalpha(i))
        {
            if (!p.ValidateString(tagName))
                isAllGoodWithTagNames = false;
        }
        else
        {
            if (p.ValidateString(tagName))
                isAllGoodWithTagNames = false;
        }
    }

    std::cout << "Checking tag name: ";
    if (isAllGoodWithTagNames)
        std::cout << "\x1B[32m" << "Success!" << "\033[0m" << std::endl;
    else
        std::cout << "\x1B[31m" << "Failed!" << "\033[0m" << std::endl;

    std::cout << std::endl;

    // Checking tag name first symbol
    isAllGoodWithTagNames = true;

    for (int i = 0; i < 256; i++)
    {
        std::string tagName = "<" + std::string(1, (unsigned char)i);
        tagName += "root> data </";
        tagName += std::string(1, (unsigned char)i);
        tagName += "root>";

        if (isalpha(i))
        {
            if (!p.ValidateString(tagName))
                isAllGoodWithTagNames = false;
        }
        else
        {
            if (p.ValidateString(tagName))
                isAllGoodWithTagNames = false;
        }
    }

    std::cout << "Checking first symbol in tag name: ";
    if (isAllGoodWithTagNames)
        std::cout << "\x1B[32m" << "Success!" << "\033[0m" << std::endl;
    else
        std::cout << "\x1B[31m" << "Failed!" << "\033[0m" << std::endl;

        std::cout << std::endl;

    // Checking param name
    bool isAllGoodWithParamNames = true;

    for (int i = 0; i < 256; i++)
    {
        std::string paramName = "<root param" + std::string(1, (unsigned char)i);
        paramName += " = \"d\"> data </root>";

        if (i == '_' || i == '-' || i == '.' || i == ' ' || i == '\t' || isalnum(i) || isalpha(i))
        {
            if (!p.ValidateString(paramName))
                isAllGoodWithParamNames = false;
        }
        else
        {
            if (p.ValidateString(paramName))
                isAllGoodWithParamNames = false;
        }
    }

    std::cout << "Checking param name: ";
    if (isAllGoodWithParamNames)
        std::cout << "\x1B[32m" << "Success!" << "\033[0m" << std::endl;
    else
        std::cout << "\x1B[31m" << "Failed!" << "\033[0m" << std::endl;

    std::cout << std::endl;

    // Checking param name first symbol
    isAllGoodWithParamNames = true;

    for (int i = 0; i < 256; i++)
    {
        std::string paramName = "<root " + std::string(1, (unsigned char)i);
        paramName += "param = \"d\"> data </root>";

        if (i == '_' || i == ' ' || i == '\t' || isalpha(i))
        {
            if (!p.ValidateString(paramName))
                isAllGoodWithParamNames = false;
        }
        else
        {
            if (p.ValidateString(paramName))
            {
                std::cout << i << std::endl;
                isAllGoodWithParamNames = false;
            }
        }
    }

    std::cout << "Checking param name first symbol: ";
    if (isAllGoodWithParamNames)
        std::cout << "\x1B[32m" << "Success!" << "\033[0m" << std::endl;
    else
        std::cout << "\x1B[31m" << "Failed!" << "\033[0m" << std::endl;

    std::cout << std::endl;

    // Ampersand checks
    std::cout << "Ampersand check(wrong usage): ";
    if (!p.ValidateString("<root> data &lt data</root>"))
        std::cout << "\x1B[32m" << "Success! Checking invalid data return false!" << "\033[0m" << std::endl;
    else
        std::cout << "\x1B[31m" << "Failed! Checking invalid data return true, but its not correct!" << "\033[0m" << std::endl;
    
    std::cout << "Ampersand check(proper usage): ";
    if (p.ValidateString("<root> data &lt; data</root>"))
        std::cout << "\x1B[32m" << "Success! Checking invalid data return false!" << "\033[0m" << std::endl;
    else
        std::cout << "\x1B[31m" << "Failed! Checking invalid data return true, but its not correct!" << "\033[0m" << std::endl;
}