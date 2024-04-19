#include <regex>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

#include "advisorbot.h"
#include "OrderBookEntry.h"
#include "CSVReader.h"

using namespace std; // to avoid typing std:: each time

// Initialize a global array of pointer countaining the commands 
extern const string commands[11] = { "help", "help cmd", "prod", "min",
                                        "max", "avg", "predict", "time","step","exit", "back"};


// global array of pointer containing the specified commands for help
extern const string help[9] = { "help prod","help min","help max",
                                    "help avg","help predict","help time","help step","help exit", "help back"};

const string fileName = "20200317.csv"; // name of the file used

advisorbot::advisorbot()
{
}

// opens the file and reads the first timestamp
void advisorbot::init()
{
    cout << "Reading the .CSV file... " << endl;
    csvReader = CSVReader(fileName);
    vector<OrderBookEntry> currentTimeStampEntries = csvReader.readTimeStampEntriesFromCSV(true);
    orderBook = OrderBook();
    orderBook.setNewTimeStampOrders(currentTimeStampEntries);
    currentStepNumber = orderBook.getNumberOfTimeStampsProcessed();
    currentTime = orderBook.getTimeForStep(currentStepNumber);
    
    displayStepParsingReport();

    string input;

    start();
    while (true)
    {
        input = getUserOption();
        respondToUserOption(input);
    }
}

// output on the start of the program
void advisorbot::start()
{
    cout << "\n type 'help' for the list of commands or 'help cmd' to output help for the specified command" << endl;
    cout << "--------------------------------------------------- " << endl;
}

// help command list all avaible commands 
void advisorbot::printHelp()
{

    cout << " The available commands are : " << endl << endl;

    // Printing commands stored in 2D array: 'commands'
    for (int i = 0; i < 10; i++)
        cout << commands[i] << "\n";

    cout << endl;
    cout << "--------------------------------------------------- " << endl;

}

//output help for the specified command
void advisorbot::helpcmd()
{

     cout << " pass a command to help. eg: " << endl;
    // Printing commands stored in 2D array: 'help'
    for (int i = 0; i < 8; i++)
    {
        cout << help[i] << "\n";

    }
    cout << "--------------------------------------------------- " << endl;

}

void advisorbot::helpprod()
{
    // 
    cout << "command prod list all available products, type in : 'prod' to output available products" << endl;
    cout << "--------------------------------------------------- " << endl;


}
void advisorbot::helpmin()
{
    cout << " to find minimum bid or ask for product in a current time step, type in following format : " << endl;
    cout << "min product ask or min product bid, ";
    cout << "eg: min ETH/BTC ask " << endl;
    cout << "--------------------------------------------------- " << endl;
}
void advisorbot::helpmax()
{
    cout << " to find maximum bid or ask for product in a current time step, type in following format : " << endl;
    cout << "max product ask or max product bid, ";
    cout << "eg: max ETH/BTC bid " << endl;
    cout << "--------------------------------------------------- " << endl;
}
void advisorbot::helpavg()
{
    cout << " compute average ask or bid for the product over the sent number of time steps " << endl;
    cout << "type in the following format : ";
    cout << " avg product ask/bid timesteps, " ;
    cout << "eg: avg ETH/BTC ask 2" << endl;
    cout << "--------------------------------------------------- " << endl;

}
void advisorbot::helppredict()
{
    cout << "predict max or min ask or bid for the sent product for the next time step " << endl;
    cout << "type in the following format:";
    cout << "predict min/max product ask/bid, " ; 
    cout << "eg: predict max ETH/BTC bid" << endl;
    cout << "--------------------------------------------------- " << endl;
}
void advisorbot::helptime()
{
    cout << " state the current timeframe, ";
    cout << " type in : time " << endl;
    cout << "--------------------------------------------------- " << endl;
}
void advisorbot::helpstep()
{
    cout << " \nto move to next time step, " ;
    cout << "type in : step" << endl;
    cout << "--------------------------------------------------- " << endl;
}

void advisorbot::helpexit()
{
    cout << "to exit advisorbot type in : exit " << endl;
    cout << "--------------------------------------------------- " << endl;
}
void advisorbot::helpback()
{
    cout << " \nto move to previous time step, ";
    cout << "type in : back" << endl;
    cout << "--------------------------------------------------- " << endl;
}

// list all available products
void advisorbot::prod()
{
    int oldStep = currentStepNumber;
    if (!csvReader.hasReachedEOF)
    {
        cout << "Reading CSV file to process the remaining steps needed to list all the products..." << endl;
        while (!csvReader.hasReachedEOF)
        {
            step();
        }
    }

    cout << "List of available products : " << endl;
    for (string const& p : orderBook.getKnownProductsForAllTimeSteps())
    {
        cout << p + "\n";
    }
    currentStepNumber = oldStep;
    currentTime = orderBook.getTimeForStep(currentStepNumber);
    cout << "\nCurrent timestamp : " << currentTime << endl;

    cout << endl;
    cout << "--------------------------------------------------- " << endl;
}

// find minimum bid or ask for product in current time step
void advisorbot::min(vector<string> commandArguments)
{
    double min;
    // check for command validity
    if (commandArguments.size() != 2)
    {
        cout << "\nInvalid command, use the following format : min product bid/ask, example : min ETH/BTC ask" << endl;
    }

    //find minimum bid or ask for product- enter min product ask/bid eg : min ETH/BTC ask or min ETH/BTC bid
    cout << "Executing the min command..." << endl;

    string product = commandArguments[0];
    string typeAsString = commandArguments[1];
    OrderBookType type = OrderBookEntry::stringToOrderBookType(typeAsString);
    vector<OrderBookEntry> matchingEntries  = orderBook.getTimeStampOrdersByTypeAndProduct(type, product, currentStepNumber);
    if (matchingEntries.empty())
    {
        cout << "\nNo matching entries for your query !" << endl;
        return;
    }
    else
    {
        min = OrderBook::getLowPrice(matchingEntries);
        cout << "\nThe min " << typeAsString << " for " << product << " is " << setprecision(8) << min << endl;
    }

    cout << "--------------------------------------------------- " << endl;

}

// find maximum bid or ask for product in current time step
void advisorbot::max(vector<string> commandArguments)
{
    double max;

    // check for command validity
    if (commandArguments.size() != 2)
    {
        cout << "\nInvalid command, use the following format : max product bid/ask, example : max ETH/BTC ask" << endl;
    }

    //find minimum bid or ask for product- enter min product ask/bid eg : min ETH/BTC ask or min ETH/BTC bid
    cout << "Executing the max command..." << endl;

    string product = commandArguments[0];
    string typeAsString = commandArguments[1];
    OrderBookType type = OrderBookEntry::stringToOrderBookType(typeAsString);
    vector<OrderBookEntry> matchingEntries = orderBook.getTimeStampOrdersByTypeAndProduct(type, product, currentStepNumber);
    if (matchingEntries.empty())
    {
        cout << "\nNo matching entries for your query !" << endl;
        return;
    }
    else
    {
        max = OrderBook::getHighPrice(matchingEntries);
        cout << "\nThe max " << typeAsString << " for " << product << " is " << max << endl;
    }

    cout << "--------------------------------------------------- " << endl;

}
//compute average ask or bid for the sent product over the sent number of time steps.
void advisorbot::avg(vector<string> commandArguments)
{
    cout << "Executing the avg command..." << endl;
    // check for command validity
    if (commandArguments.size() != 3 || (commandArguments[1] != "ask" && commandArguments[1] != "bid")) // avg ETH/BTC ask timessteps
    {
        cout << "\nInvalid command, use the following format : avg product ask/bid timesteps, example : avg ETH/BTC ask 10" << endl;
        return;
    }

    string product = commandArguments[0];
    string typeAsString = commandArguments[1];
    OrderBookType type = OrderBookEntry::stringToOrderBookType(typeAsString);
    string numberOfStepsAsStr = commandArguments[2];
    int numberOfStepsWanted = 0;
    try {
        numberOfStepsWanted = stoi(numberOfStepsAsStr);
    }
    catch (std::exception& e) {
        cout << "\nInvalid command, use the following format : avg ETH/BTC ask/bid timesteps, example : avg ETH/BTC ask 10" << endl;
        return;
    }

    if (numberOfStepsWanted < 1)
    {
        cout << "\nInvalid number of steps given, number of steps should be > 1" << endl;
        return;
    }

    int number = 0;
    if (numberOfStepsWanted > currentStepNumber) {
        cout << "Current step is lower than the step supplied, the average will be calculated over the last " << currentStepNumber <<" steps instead" << endl;
        number = currentStepNumber;
    }
    else
    {
        number = numberOfStepsWanted;
    }

    double avg = orderBook.getAvg(product, type, number, currentStepNumber);
    cout << "\nThe average " << typeAsString << " for " << product << " in the last " << number << " timesteps is : " << avg << endl;
    
    cout << "--------------------------------------------------- " << endl;

}
//predict max or min ask or bid for the sent product for the next time step
void advisorbot::predict(vector<string> commandArguments)
{
    cout << "Executing the predict command..." << endl;

    // check for command validity : predict max ETH/BTC bid
    if (commandArguments.size() != 3 
        || (commandArguments[0] != "min" && commandArguments[0] != "max")
        || (commandArguments[2] != "ask" && commandArguments[2] != "bid"))
    {
        cout << "\nInvalid command, use the following format : predict min/max product ask/bid, example : predict max ETH/BTC bid" << endl;
        return;
    }

    string maxOrMin = commandArguments[0];
    string product = commandArguments[1];
    string typeAsString = commandArguments[2];
    OrderBookType type = OrderBookEntry::stringToOrderBookType(typeAsString);

    double sum = 0;
    int count = 0;
    vector<vector<OrderBookEntry>> allOrders = orderBook.getOrders();

    auto subVectBegin = allOrders.begin();
    auto subVectEnd = allOrders.begin() + currentStepNumber;

    vector<vector<OrderBookEntry>> subVect(subVectBegin, subVectEnd);

    // using moving average from the PREVIOUS steps
    for (vector<OrderBookEntry> vect : subVect)
    {
        vector<OrderBookEntry> matchingEntries = orderBook.getOrdersByTypeAndProductFromTimeStamp(vect, type, product);
        if (maxOrMin == "max")
        {
            sum = sum + orderBook.getHighPrice(matchingEntries);
        }

        if (maxOrMin == "min")
        {
            sum = sum + orderBook.getLowPrice(matchingEntries);
        }
        count++;
    }

    double movingAverage = sum / count;
    cout << "The predicted " << maxOrMin << " for the next step is " << movingAverage << endl;
    cout << "--------------------------------------------------- " << endl;
}
//state current time in dataset, i.e. which timeframe are we looking at
void advisorbot::time()
{
    cout << " " + currentTime << endl;
    cout << "--------------------------------------------------- " << endl;
}
//move to next time step
void advisorbot::step()
{
    if (orderBook.getNumberOfTimeStampsProcessed() < currentStepNumber + 1)
    {
        vector<OrderBookEntry> newTimeStampEntries = csvReader.readTimeStampEntriesFromCSV(false);
        if (newTimeStampEntries.size() > 0) {
            orderBook.setNewTimeStampOrders(newTimeStampEntries);
            currentStepNumber++;
            currentTime = orderBook.getTimeForStep(currentStepNumber);
            displayStepParsingReport();
        }
    }
    else
    {
        currentStepNumber++;
        currentTime = orderBook.getTimeForStep(currentStepNumber);
        cout << "Moved to timestep number : " << currentStepNumber << endl;
        cout << "Current timestamp : " << currentTime << endl;
    }
    cout << "--------------------------------------------------- " << endl;
}

void advisorbot::displayStepParsingReport()
{
    cout << "Number of new entries successfuly parsed in the current step : " << orderBook.getLastTimeStampOrders().size() << endl;
    cout << "Number of different timestamps processed : " << currentStepNumber << endl;
    cout << "Last processed timestamp : " << currentTime << endl;

}

// go back to the previous processed timestamp
void advisorbot::stepBack()
{
    // we dont have to re read the file, the order entries related to the previous timestamp are already on the orderbook object
    // step back => go to orderbook.orders[index - 1]
    if (currentStepNumber <= 1)
    {
        cout << "Cannot step back cause you're on the first timestamp" << endl;
        return;
    }
    currentStepNumber = currentStepNumber--;
    currentTime = orderBook.getTimeForStep(currentStepNumber);
    cout << "Moved to timestep number : " << currentStepNumber << endl;
    cout << "Current timestamp : " << currentTime << endl;
    cout << "--------------------------------------------------- " << endl;
}

//exit the program
void advisorbot::exit()
{
    std::exit(EXIT_SUCCESS); // exit the program with a success status code
}
// read user input 
string advisorbot::getUserOption()
{
    string command = "";
    string inputLine;
    cout << "user>";
    // read a line from the input 
    getline(cin, inputLine);
    try {
        command = string(inputLine);
    }
    catch (const exception)
    {
        cout << "Invalid command" << endl;
    }

    return command;
}

void advisorbot::respondToUserOption(string command)
{
    cout << "advisorbot>>";
    vector<string> tokens = tokeniseCommand(command);
    string commandName = tokens[0];
    // check if the command is in the array of commands or the array of help commands
    int sizeCommands = sizeof(commands) / sizeof(*commands);
    bool existsInCommands = find(commands, commands + sizeCommands, commandName) != commands + sizeCommands;
    int sizeHelp = sizeof(help) / sizeof(*help);
    bool existsInHelp = find(help, help + sizeHelp, commandName) != help + sizeHelp; // check if command is a help command, example : "help min"

    if (command == " " || (!existsInCommands && !existsInHelp))
    {
        cout << " Invalid choice. Type help to print available commands" << endl;
        cout << "--------------------------------------------------- " << endl;
        return;
    }

    // help cmd commands 
    if (command == commands[0])
    {
        printHelp();
    }
    if (command == commands[1])
    {
        helpcmd();
    }
    if (command == help[0])
    {
        helpprod();
    }
    if (command == help[1])
    {
        helpmin();
    }
    if (command == help[2])
    {
        helpmax();
    }
    if (command == help[3])
    {
        helpavg();
    }
    if (command == help[4])
    {
        helppredict();
    }
    if (command == help[5])
    {
        helptime();
    }
    if (command == help[6])
    {
        helpstep();
    }
    if (command == help[7])
    {
        helpexit();
    }
    if (command == help[8])
    {
        helpback();
    }

    if (tokens[0] == commands[2])
    {
        prod();
    }
    if (tokens[0] == commands[3])
    {
        vector<string> arguments = getCommandArguments(tokens);

        min(arguments);
    }
    if (tokens[0] == commands[4])
    {
        vector<string> arguments = getCommandArguments(tokens);
        max(arguments);
    }
    if (tokens[0] == commands[5])
    {
        vector<string> arguments = getCommandArguments(tokens);
        avg(arguments);
    }
    if (tokens[0] == commands[6])
    {
        vector<string> arguments = getCommandArguments(tokens);
        predict(arguments);
    }

    if (command == commands[7])
    {
        time();
    }
    if (command == commands[8])
    {
        step();
    }
    if (command == commands[9])
    {
        cout << " Bye!" << endl;
        exit();
    }
    if (command == commands[10])
    {
        stepBack();
    }

}


// uses a regular expression to split the user input into a vector of tokens, and supporting multiple sequential spaces between them
vector<string> advisorbot::tokeniseCommand(string commandLine)
{
    vector<string> elems;
    regex re("\\s+"); // multiple spaces
    sregex_token_iterator iter(commandLine.begin(), commandLine.end(), re, -1);
    sregex_token_iterator end;

    while (iter != end) {
        if (iter->length()) {
            elems.push_back(*iter);
        }
        ++iter;
    }

    return elems;
}


// returns a vector of the command arguments
vector<string> advisorbot::getCommandArguments(vector<string> commandTokens) {
    // create a subvector containing the remaining tokens after the command name itself for example "min ETH/BTC bid" => "ETH/BTC bid"
    vector<string>::iterator first = commandTokens.begin() + 1;
    vector<string>::iterator last = commandTokens.begin() + commandTokens.size();
    vector<string> restOfTokens = vector<string>(first, last);
    return restOfTokens;

}