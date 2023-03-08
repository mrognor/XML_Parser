#include "XML_Parser.h"
#include "XML_Functions.h"

void log(std::string) {}

int main()
{
    XmlParser p;
    
    SetLogFunc(log);

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
}