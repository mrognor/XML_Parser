#pragma once

#include <stack>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>

#include "XML_Functions.h"

namespace xmlp
{
    /// Simple xml parser
    class XmlParser
    {
    private:
        // List with parsed xml data
        std::list<XmlData> Data;
        // Bool variable to specify xml data path saving
        bool IsPathSaving = true;
    public:
        /// Return list with parsed xml data 
        std::list<XmlData>& GetData() { return Data; }

        /*!
            \brief This function is needed to set the path of each tag to be saved.
            
            \warning Disabling the saving of the tag path is necessary to save space in RAM, 
            since the path takes up more space than the tag name, 
            storing the path spends a lot of memory.
            At the same time it is very easy to determine the path during tag processing
        */
        void SetPathSaving(bool isPathSaving) { IsPathSaving = isPathSaving; }

        /*!
            \brief A function for checking a file containing xml information for validity

            This function check the file for validity and if it is valid, it saves it to the list.
            If the data is invalid, the list will be empty.

            \return Returns true if the xml data in file is valid, otherwise false
        */
        bool ValidateFile(const std::string& fileName);

        /*!
            \brief A function for checking a string containing xml information for validity

            This function check the string for validity and if it is valid, it saves it to the list.
            If the data is invalid, the list will be empty.

            \return Returns true if the xml data in string is valid, otherwise false
        */
        bool ValidateString(const std::string& str);

        /*!
            \brief A function for checking a vector containing xml information for validity

            This function check the vector for validity and if it is valid, it saves it to the list.
            If the data is invalid, the list will be empty.

            \return Returns true if the xml data in vector is valid, otherwise false
        */
        bool ValidateVectorOfStrings(const std::vector<std::string>& vectorToValidate);

        /*!
            \brief A template function for checking a container containing xml information for validity

            This function check the container for validity and if it is valid, it saves it to the list.
            If the data is invalid, the list will be empty.

            \return Returns true if the xml data in container is valid, otherwise false
        */
        template <class T>
        bool ValidateContainerWithStrings(const T& containerToValidate);

        /*!
            \brief Function for inserting data into a list with xml data

            Inserts data before the iterator, just like in std
            If adding data failed, the list with xml information does not change

            \warning To insert this symbols: (", ', <, >, &) you have to use next strings: (&quot;, &apos;, &lt;, &gt;, &amp;)
            Note that the function will not replace these sequences with their symbols

            \param[in] posToInsertIt the iterator before which you need to insert data into the list
            \param[in] listToInsert the list with data to insert

            \return will return true if the insertion was successful, otherwise it will return false
        */
        bool InsertData(const std::list<XmlData>::iterator& posToInsertIt, std::list<std::string> listToInsert);

        // Insert data before iterator like in std
        // Warning, data copieng. May end your RAM.
        /*!
            \brief Function for inserting data into a list with xml data

            Inserts first list before the first iterator.
            Inserts second list before the second iterator
            If adding data failed, the list with xml information does not change
            This function completely copies the original list, then inserts your data there, 
            then checks them for validity and, if successful, assigns the original list to the list with the added elements

            \warning In this function, a new list is created larger than the size of the original list, if your list is very large, 
            then you may not have enough RAM, in such cases, insert data through the getter to the original list
            \warning To insert this symbols: (", ', <, >, &) you have to use next strings: (&quot;, &apos;, &lt;, &gt;, &amp;)
            Note that the function will not replace these sequences with their symbols

            \param[in] firstPosToInsertIt the first iterator before which you need to insert first list
            \param[in] secondPosToInsertIt the second iterator before which you need to insert second list
            \param[in] firstListToInsert the first list with data to insert
            \param[in] secondListToInsert the second list with data to insert

            \return will return true if the insertion was successful, otherwise it will return false
        */
        bool InsertData(const std::list<XmlData>::iterator& firstPosToInsertIt, const std::list<XmlData>::iterator& secondPosToInsertIt, 
        std::list<std::string> firstListToInsert, std::list<std::string> secondListToInsert);

        /*! 
            Function to write data to file

            \warning This function fully rewrite file with own writing structure. That means that all tags will be at only one new line 
            \param[in] fileName string with file name
            \return will return true if the write was successful, otherwise it will return false
        */ 
        bool WriteDataToFile(std::string fileName);
    };


    template <class T>
    bool XmlParser::ValidateContainerWithStrings(const T& containerToValidate)
    {
        return Validate(containerToValidate.begin(), containerToValidate.end(), &Data);
    }
}