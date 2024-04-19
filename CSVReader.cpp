#include "CSVReader.h"
#include <iostream>
#include <fstream>
using namespace std; // to avoid typing std:: each time


CSVReader::CSVReader()
{

}


CSVReader::CSVReader(string name)
{
	fileName = name;
	fileStream = ifstream(fileName);
	hasReachedEOF = false;
}

// reads the csv file and returns a vector containing the entries of the next timestamp
vector<OrderBookEntry> CSVReader::readTimeStampEntriesFromCSV(bool startFromBeginning)
{
	vector<OrderBookEntry> entries;
	string line;
	string currentTimeStamp = "";
	string lineTimeStamp;
	
	if (!fileStream.is_open())
	{
		fileStream.open(fileName);
	}
	
	if (hasReachedEOF)
	{
		cout << "The whole CSV file has been parsed, no more additional steps detected" << endl;
		return entries; // return empty vector
	}
	if (!startFromBeginning)
	{
		fileStream.seekg(readPosition);// continue reading from last location instead of reading from the beginning of the file
	}
	do
	{
		try {
			getline(fileStream, line);
			if (!line.empty())
			{
				OrderBookEntry obe = stringsToOBE(tokenise(line, ','));
				if (currentTimeStamp == "") // for the first iteration, to set the timestamp value
				{
					currentTimeStamp = obe.timestamp;
				}
				lineTimeStamp = obe.timestamp;
				if (lineTimeStamp == currentTimeStamp)
				{
					entries.push_back(obe); // push all the values of this timestamp into a subvector 
					readPosition = fileStream.tellg(); // save the reading location
				}
			}
		}
		catch (const exception& e)
		{
			cout << "CSVReader::readTimeStampEntriesFromCSV bad data" << endl;
		}
		catch (...)
		{
			cout << "CSVReader::readTimeStampEntriesFromCSV bad data" << endl;
		}

	} while (lineTimeStamp == currentTimeStamp && !fileStream.eof());

	if (fileStream.eof())
	{
		hasReachedEOF = true;
		cout << "Finished parsing the entire file" << endl;
	}
	return entries;
}

std::vector<std::string> CSVReader::tokenise(std::string csvLine, char separator)
{
	std::vector<std::string> tokens;
	signed int start, end;
	std::string token;
	start = csvLine.find_first_not_of(separator, 0);
	do {
		end = csvLine.find_first_of(separator, start);
		if (start == csvLine.length() || start == end) break;
		if (end >= 0) token = csvLine.substr(start, end - start);
		else token = csvLine.substr(start, csvLine.length() - start);
		tokens.push_back(token);
		start = end + 1;
	} while (end > 0);

	return tokens;
}

OrderBookEntry CSVReader::stringsToOBE(std::vector<std::string> tokens)
{
	double price, amount;

	if (tokens.size() != 5) // bad
	{
		std::cout << "Bad line " << std::endl;
		throw std::exception{};
	}
	// we have 5 tokens
	try {
		price = std::stod(tokens[3]);
		amount = std::stod(tokens[4]);
	}
	catch (const std::exception& e) {
		std::cout << "CSVReader::stringsToOBE Bad float! " << tokens[3] << std::endl;
		std::cout << "CSVReader::stringsToOBE Bad float! " << tokens[4] << std::endl;
		throw;
	}

	OrderBookEntry obe{ price,
						amount,
						tokens[0],
						tokens[1],
						OrderBookEntry::stringToOrderBookType(tokens[2]) };

	return obe;
}


OrderBookEntry CSVReader::stringsToOBE(std::string priceString,
	std::string amountString,
	std::string timestamp,
	std::string product,
	OrderBookType orderType)
{
	double price, amount;
	try {
		price = std::stod(priceString);
		amount = std::stod(amountString);
	}
	catch (const std::exception& e) {
		std::cout << "CSVReader::stringsToOBE Bad float! " << priceString << std::endl;
		std::cout << "CSVReader::stringsToOBE Bad float! " << amountString << std::endl;
		throw;
	}
	OrderBookEntry obe{ price,
					amount,
					timestamp,
					product,
					orderType };

	return obe;
}
