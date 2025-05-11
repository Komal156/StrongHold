#include "stronghold.h"
#include <iostream>
using namespace std;

// SocialClass Implementation
SocialClass::SocialClass(const string& name, int pop, int happy, int infl)
    : className(name), population(pop), happiness(happy), influence(infl) {
}

void SocialClass::updateHappiness(int modifier)
{
    happiness += modifier;
    if (happiness > 100) happiness = 100; // Ensure happiness stays within range
    if (happiness < 0) happiness = 0;
}

void SocialClass::demandChange() const {
    cout << className << " class demands change!\n";
}

// Peasant Implementation
Peasant::Peasant(int pop, int happy, int infl, int lab, int skl)
    : SocialClass("Peasant", pop, happy, infl), labor(lab), skill(skl) {
}

int Peasant::calculateTaxContribution() const {
    return population * labor * 10; // Adjusted for integer calculations
}

void Peasant::demandChange() const {
    cout << "Peasants demand better working conditions!\n";
}

// Merchant Implementation
Merchant::Merchant(int pop, int happy, int infl, int wlth)
    : SocialClass("Merchant", pop, happy, infl), wealth(wlth) {
}

int Merchant::calculateTaxContribution() const {
    return population * wealth * 20; // Adjusted for integer calculations
}

void Merchant::demandChange() const {
    cout << "Merchants demand lower taxes!\n";
}

// Noble Implementation
Noble::Noble(int pop, int happy, int infl, int lnd) : SocialClass("Noble", pop, happy, infl), land(lnd) {}

int Noble::calculateTaxContribution() const
{
    return population * land * 30; // Adjusted for integer calculations
}

void Noble::demandChange() const
{
    cout << "Nobles demand more privileges!\n";
}