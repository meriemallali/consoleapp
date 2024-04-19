#pragma once
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>
#include <fstream>


using namespace std;


class OrderBook
{
    public:
        // return vector of all know products in the dataset
        vector<string> getKnownProductsForAllTimeSteps();

        //return vector of entries according to the sent filters
        vector<OrderBookEntry> getTimeStampOrdersByTypeAndProduct(OrderBookType type, string product, int currentStepNumber);

        //return vector of entries for the last timestamp
        vector<OrderBookEntry> getLastTimeStampOrders();

        // getter for all entries
        vector<vector<OrderBookEntry>> getOrders();

        // setter for entries of the current timestamp
        void setNewTimeStampOrders(vector<OrderBookEntry> entries);

        /** returns the earliest time in the orderbook*/
        string getTimeForStep(int stepNumber);

        int getNumberOfTimeStampsProcessed();

        double getAvg(string product, OrderBookType type, int inputNumber, int currentStepNumber);

        vector<OrderBookEntry> getOrdersByTypeAndProductFromTimeStamp(vector<OrderBookEntry> inputVect, OrderBookType type, string product);

        static double getHighPrice(vector<OrderBookEntry>& orders);
        static double getLowPrice(vector<OrderBookEntry>& orders);

    private:
        // orders is a vector of vectors of entries, each vector contains the entries of a single timestamp
        vector<vector<OrderBookEntry>> orders;
};
