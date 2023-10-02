\mainpage
1. symbols before and after symbol ":" in tag param name works not corrected
2. Tag with xml specification ignoring
3. XML DTD not supporting

# Usage examples
## Checking data for validity and saving it

1. This function will read the file, check the file data for validity, and if the data is valid, it will write them to the class object
``` c++
    xmlp::XmlParser p;

    if (p.ValidateFile("path_to_file/file_with_xml_data.xml"))
        std::cout << "true" << std::endl;
    else 
        std::cout << "false" << std::endl;
```

2. This function will check the string for validity, and if the string is valid, it will write them to the class object
``` c++
    xmlp::XmlParser p;

    if (p.ValidateString("<string><with><xml><data><to><validate> data </validate></to></data></xml></with><string>"))
        std::cout << "true" << std::endl;
    else 
        std::cout << "false" << std::endl;
```

3. This function will check the string vector for validity, and if the data is valid, it will write them to the class object
Note that the function will work correctly even if the vector elements do not match the xml elements.
``` c++
    xmlp::XmlParser p;

    if (p.ValidateVectorOfStrings({"<vector>", "<with>", "<strings>",
        "<to><validate> data </vali", "date></to>",
        "</strings>", "</with>", "</vector>"});)
    std::cout << "true" << std::endl;
    else 
        std::cout << "false" << std::endl;
```

4. This function will check the container with strings for validity, and if the data is valid, it will write them to the class object. It can handle any iterable container
Note that the function will work correctly even if the container elements do not match the xml elements.
``` c++
    xmlp::XmlParser p;

    std::list<std::string> listWithXmlData = {"<list>", "<with> <xml> <data> </data> </xml> </with>", "</list>"};
    if (p.ValidateContainerWithStrings(listWithXmlData))
        std::cout << "true" << std::endl;
    else 
        std::cout << "false" << std::endl;
```

## XML data processing
The simplest way to process data is a regular for loop
``` c++
    xmlp::XmlParser p;

    if (p.ValidateFile("xml_files/parsing_tests.xml"))
        std::cout << "true" << std::endl;
    else 
        std::cout << "false" << std::endl;

    std::cout << std::endl;

    for (auto it = p.GetData().begin(); it != p.GetData().end(); it++)
    {
        // Data processing
    }
```
Each element of the list with xml data contains the following information:
    * DataType - The data type in the current element. For more information see [XmlDataType](https://mrognor.github.io/XML_Parser_Documentation/doc/html/namespacexmlp.html#a7dfdf690ac1440f989a5f1321770484e)
    * Path - Data path. Format: /path/to/data
    * TagName - Tag name, if the list item stores a tag, if the list item does not store a tag, then this variable is empty
    * ParamsAndValues - Map, where the keys are tag parameters, and the values of the keys are the values of the corresponding tag parameters , if the list item stores a tag, if the list item does not store a tag, then this variable is empty
    * Data - Raw xml data

## Insert XML data
There are two ways to insert data. The first way is to use library functions that check the insertion result for validity. The second way is to insert all the data through the source list, without using library functions.

### Insert with checks
1. This function inserts data before the iterator, checks the validity of the resulting list. If the data is not valid, the function will delete the inserted data and return false.
``` c++
    for (auto it = p.GetData().begin(); it != p.GetData().end(); it++)
    {
        if (it->TagName == "tagA" && it->DataType == xmlp::closingTag)
            p.InsertData(it, {"<correctData> Correct text </correctData>"});           
    }
```

2. This function allows you to insert data into 2 places at once and performs a validation check. If the data is not valid, the function will delete the inserted data and return false. For the function to work, you need to find two iterators before which data will be inserted
``` c++
    std::list<xmlp::XmlData>::iterator beg, end;

    for (auto it = p.GetData().begin(); it != p.GetData().end(); it++)
    {
        if (it->TagName == "tagC" && it->DataType == xmlp::openingTag)
        {
            beg = ++it;
            it--;
        }

        if (it->TagName == "tagC" && it->DataType == xmlp::closingTag)
            end = it;         
    }

    p.InsertData(beg, end, {"<addingTag>"}, {"</addingTag>"});
```

### Insert without checks
You can use standart std::list functions
``` c++
    p.GetData().insert();
    p.GetData().push_back((xmlp::XmlData)"<data>");
    p.GetData().push_front((xmlp::XmlData)"</data>");
```

## Save xml data to file
To save data to a file, use the function. Parametr is file name.
Note that the structure of line breaks in the source file is not preserved
``` c++
    p.WriteDataToFile("bin/1.xml");
```

# Saving RAM
By default, the validator saves the tag path, but if your file is very large and has a complex structure with long paths, saving the path may take up too much RAM. This is because the path stores the names of the previous tags. This is done for ease of operation, but saving the tag path can be disabled. Then the definition of the tag path will need to be written manually and tracked when moving through the list.
``` c++
    xmlp::XmlParser p;

    p.SetPathSaving(false);
```