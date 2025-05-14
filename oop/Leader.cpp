#include "stronghold.h"
#include <iostream>
using namespace std;


Leader::Leader(const string& nm, int pop, int corrupt, const string& style) : name(nm), popularity(pop), corruptionLevel(corrupt), leadershipStyle(style) {}
void Leader::addressCrisis()
{
    cout << name << " is addressing a crisis...\n";
    popularity -= 10; // Crisis reduces the popularity
    if (popularity < 0) popularity = 0;
}

bool Leader::holdElection() const
{
    return popularity > 50; // Election if popularity is above 50
}

// Monarch Implementation
Monarch::Monarch(const string& nm, int pop, int corrupt, const string& style,
    const string& heirname, int years)
    : Leader(nm, pop, corrupt, style), heir(heirname), reignYears(years) 
{
}
string Monarch::getHeir() const {
    return heir; // Returns the heir's name stored in the heir member variable
}

void Monarch::makeDecision(int option) {
    if (option < 1 || option > 3) {
        cout << "Invalid decision. Please choose a valid option.\n";
        return;
    }
    cout << "Monarch " << name << " made a decision.\n";
}

bool Monarch::holdElection() const {
    return false; // Monarchs do not hold elections
}

// ElectedLeader Implementation
ElectedLeader::ElectedLeader(const string& nm, int pop, int corrupt, const string& style,
    int term, int current) : Leader(nm, pop, corrupt, style), termLength(term), currentTerm(current) {
}

void ElectedLeader::makeDecision(int option)
{
    if (option < 1 || option > 3)
    {
        cout << "Invalid decision. Please choose a valid option.\n";
        return;
    }
    cout << "Elected Leader " << name << " made a decision.\n";
}

bool ElectedLeader::holdElection() const
{
    return currentTerm >= termLength; // Election is held if term is complete
}
//a