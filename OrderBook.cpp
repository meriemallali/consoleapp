#include "OrderBook.h"
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <map>
#include <algorithm>
#include <iostream>
using namespace std; // to avoid typing std:: each time

vector<string> OrderBook::getKnownProductsForAllTimeSteps()
{
    vector<string> products;

    map<string, bool> prodMap;

    for (vector<OrderBookEntry> vect : orders)
    {
        for (OrderBookEntry& e : vect)
        {
            prodMap[e.product] = true;
        }
    }

    // now flatten the map to a vector of strings
    for (auto const& e : prodMap)
    {
        products.push_back(e.first);
    }

    return products;
}


vector<OrderBookEntry> OrderBook::getTimeStampOrdersByTypeAndProduct(OrderBookType type, 
                                        string product, int currentStepNumber)
{
    vector<OrderBookEntry> orders_sub;
    for (OrderBookEntry& e : orders[currentStepNumber - 1])
    {
        if (e.orderType == type && 
            e.product == product)
            {
                orders_sub.push_back(e);
            }
    }
    return orders_sub;
}

vector<OrderBookEntry> OrderBook::getLastTimeStampOrders()
{
    return orders.back();
}

vector<vector<OrderBookEntry>> OrderBook::getOrders()
{
    return orders;
}

void OrderBook::setNewTimeStampOrders(std::vector<OrderBookEntry> entries) {
    orders.push_back(entries); // orders is an array of arrays of entries
}

double OrderBook::getHighPrice(std::vector<OrderBookEntry>& orders)
{
    double max = orders[0].price;
    for (OrderBookEntry& e : orders)
    {
        if (e.price > max)max = e.price;
    }
    return max;
}


double OrderBook::getLowPrice(std::vector<OrderBookEntry>& orders)
{
    double min = orders[0].price;
    for (OrderBookEntry& e : orders)
    {
        if (e.price < min)min = e.price;
    }
    return min;


}

string OrderBook::getTimeForStep(int stepNumber)
{
    return orders[stepNumber - 1][0].timestamp; // get the timestamp from an entry in the last vector of entries
}


int OrderBook::getNumberOfTimeStampsProcessed()
{
    return orders.size();
}

double OrderBook::getAvg(string product, OrderBookType type, int inputNumber, int currentStepNumber)
{
    double sum = 0;
    int count = 0;
    vector<double> matchingPrices;
    int size = orders.size();
    auto end = orders.begin() + currentStepNumber;
    auto begining = end - inputNumber;
    vector<vector<OrderBookEntry>> lastNStepsVect(begining, end);
    for (vector<OrderBookEntry> vect : lastNStepsVect)
    {
        for (OrderBookEntry entry : vect)
        {
            if (entry.product == product && entry.orderType == type)
            {
                sum = sum + entry.price;
                count++;
            }
        }
    }
    return sum / count;
}

vector<OrderBookEntry> OrderBook::getOrdersByTypeAndProductFromTimeStamp(vector<OrderBookEntry> inputVect, OrderBookType type,
    string product)
{
    vector<OrderBookEntry> orders_sub;
    for (OrderBookEntry& e : inputVect)
    {
        if (e.orderType == type &&
            e.product == product)
        {
            orders_sub.push_back(e);
        }
    }
    return orders_sub;
}
