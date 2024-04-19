#pragma once

#include "OrderBookEntry.h"
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class CSVReader
{
    public:
     CSVReader();
     CSVReader(string fileName);
     vector<OrderBookEntry> readTimeStampEntriesFromCSV(bool startFromBeginning);
     vector<string> tokenise(string csvLine, char separator);
     OrderBookEntry stringsToOBE(string price, 
                                        string amount, 
                                        string timestamp, 
                                        string product, 
                                        OrderBookType OrderBookType);

     bool hasReachedEOF; // wether or not we have read all of the file

    private:
     OrderBookEntry stringsToOBE(vector<string> strings);
     ifstream fileStream;
     streampos readPosition;
     string fileName;
     
};