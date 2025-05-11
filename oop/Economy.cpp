#include "stronghold.h"
#include <iostream>
using namespace std;


Economy::Economy(int initialTreasury)
    : treasury(initialTreasury), taxRate(10), inflation(0), gdp(1000), corruptionLevel(0),
    food(1000, 10), // Initialize Food with 1000 units and 10 farms
    wood(500, 5),   // Initialize Wood with 500 units and 5 forests
    iron(300, 3)    // Initialize Iron with 300 units and 3 mines
{
    bank = new Bank(initialTreasury / 2);
}

void Economy::collectTaxes(SocialClass** socialClasses, int classCount) {
    for (int i = 0; i < classCount; i++) {
        treasury += socialClasses[i]->calculateTaxContribution() * taxRate / 100;
    }
}

void Economy::calculateInflation() {
    inflation = treasury / 1000;
    if (inflation > 100) inflation = 100;
}

void Economy::adjustTaxRate(int newRate)
{
    if (newRate < 0 || newRate > 100)
    {
        cout << "Invalid tax rate. Please enter a value between 0 and 100.\n";
        return;
    }
    taxRate = newRate;
}

void Economy::fundService(int amount)
{
    if (amount < 0 || amount > treasury)
    {
        cout << "Invalid funding amount. Please enter a value within available treasury.\n";
        return;
    }
    treasury -= amount;
}

void Economy::processLoans() {
    bank->collectDebt();
}

void Economy::addToTreasury(int amount) {
    if (amount < 0) {
        cout << "Invalid amount. Please enter a positive number.\n";
        return;
    }
    treasury += amount;
}
Economy::~Economy() {
    delete bank; // To avoid memory leak
}
///a