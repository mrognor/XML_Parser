# Main programm binary
project(main)

add_executable(main main.cpp)
target_link_libraries(main XML_Parser)

# XML_AutoTests binary
project(XML_AutoTests)

add_executable(XML_AutoTests auto_tests.cpp)
target_link_libraries(XML_AutoTests XML_Parser)

# XML_Parser lib binary
project(XML_Parser)

add_library(XML_Parser STATIC XML_Parser.h XML_Parser.cpp XML_Functions.h XML_Functions.cpp)