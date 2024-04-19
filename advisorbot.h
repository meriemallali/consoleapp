#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include <string>
using namespace std; // to avoid typing std:: each time


class advisorbot
{
	public:
		/** Call this to start the sim */
		advisorbot();
		void init();
		CSVReader csvReader;
	private:
		// methods
		void start();

		void exit();
		void printHelp();
		void helpcmd();
		void helpprod();
		void helpmin();
		void helpmax();
		void helpavg();
		void helppredict();
		void helptime();
		void helpstep();
		void helpback();
		void helpexit();
		void prod();
		void min(vector<string> commandArguments);
		void max(vector<string> commandArguments);
		void avg(vector<string> commandArguments);
		void predict(vector<string> commandArguments);
		void time();
		void step();
		void stepBack();
		void displayStepParsingReport();

		string getUserOption();
		void respondToUserOption(std::string userOption);
		vector<string> tokeniseCommand(string commandLine);
		vector<string> getCommandArguments(vector<string> commandTokens);

		// fields
		string currentTime;
		OrderBook orderBook;
		int currentStepNumber = 0;
};
