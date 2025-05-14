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
void Economy::calculateGDP(Population* population, SocialClass** socialClasses, int classCount)
{
    // Simple GDP calculation based on population and social class contributions
    gdp = population->getTotalCount() * 10; // Base GDP from population
    for (int i = 0; i < classCount; ++i) 
    {
        gdp += socialClasses[i]->calculateTaxContribution() * 5; // Contribution from each social class
    }
}

void Economy::adjustTaxRate(int rate)
{
    if (rate < 0 || rate > 100) {
        throw runtime_error("Invalid tax rate: " + to_string(rate) + ". Must be between 0 and 100.");
    }
    taxRate = rate;
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
void Economy::updateInflation() {
    // Simple inflation update based on economic factors
    int baseInflation = 2; // Base inflation rate
    int taxImpact = taxRate / 10; // Higher tax rates increase inflation
    int corruptionImpact = corruptionLevel / 20; // Corruption adds to inflation
    inflation = baseInflation + taxImpact + corruptionImpact;
    if (inflation < 0) inflation = 0; // Ensure non-negative inflation
    if (inflation > 20) inflation = 20; // Cap inflation at 20%
}
void Economy::addToTreasury(int amount) {
    if (amount < 0) {
        cout << "Invalid amount. Please enter a positive number.\n";
        return;
    }
    treasury += amount;
}
Economy::~Economy()
{
    delete bank; // To avoid memory leak
}
///a