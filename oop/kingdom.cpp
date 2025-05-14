#include "stronghold.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib> // for rand()
using namespace std;

Kingdom::Kingdom(const string& kingdomName, int id, const string& leaderName, const string& heirname)
    : name(kingdomName), stability(50), year(1), socialClassCount(0), mapX(0), mapY(0), kingdomId(id)
{
    population = new Population(1000);
    socialClasses[socialClassCount++] = new Peasant(500, 40, 20, 50, 30);
    socialClasses[socialClassCount++] = new Merchant(300, 50, 30, 1000);
    socialClasses[socialClassCount++] = new Noble(200, 50, 50, 50);
    military = new Infantry(100, 80, 50, 70, 50, 100);
    leader = new Monarch(leaderName, 50, 10, "Benevolent", heirname, 10);
    economy = new Economy(5000);
}
// Destructor
Kingdom::~Kingdom() {
    delete population;
    delete military;
    delete leader;
    delete economy;
    for (int i = 0; i < socialClassCount; ++i) {
        delete socialClasses[i];
    }
}

// Simulate a turn in the kingdom
void Kingdom::simulateTurn() {
    ++year;
    population->calculateGrowth(80, 70);
    population->updateHealth(90, 80);
    for (int i = 0; i < socialClassCount; ++i) {
        socialClasses[i]->updateHappiness(1);
    }
    military->updateMorale(2);
    economy->collectTaxes(socialClasses, socialClassCount);
    economy->calculateInflation();
    economy->processLoans();
    recalculateStability();
}

// Attack another kingdom
void Kingdom::attack(Kingdom& defender) {
    try {
        int attackerStrength = military->calculateStrength();
        int defenderStrength = defender.getMilitary()->calculateStrength();
        if (attackerStrength <= 0 || defenderStrength <= 0) {
            throw runtime_error("Invalid military strength for battle.");
        }
        if (attackerStrength > defenderStrength) {
            defender.getMilitary()->recruit(-defender.getMilitary()->getSoldierCount() / 2);
            defender.adjustStability(-20);
            adjustStability(10);
            cout << name << " wins the battle against " << defender.getName() << "!\n";
        }
        else {
            military->recruit(-military->getSoldierCount() / 2); // Fixed soldierCount to getSoldierCount()
            adjustStability(-20);
            cout << defender.getName() << " repels the attack from " << name << "!\n";
        }
    }
    catch (const runtime_error& e) {
        cerr << "Error during attack: " << e.what() << "\n";
    }
}

// Betray an ally kingdom
void Kingdom::betray(Kingdom& ally) {
    try {
        cout << name << " betrays " << ally.getName() << "!\n";
        adjustStability(-30);
        ally.adjustStability(-20);
        military->updateMorale(-10);
        ally.getMilitary()->updateMorale(-10);
    }
    catch (const runtime_error& e) {
        cerr << "Error during betrayal: " << e.what() << "\n";
    }
}

// Display kingdom status
void Kingdom::displayStatus() const {
    cout << "\n************************************************************************************************************\n";
    cout << "\n                                         Kingdom Status                                                    \n";
    cout << "\n************************************************************************************************************\n";
    cout << "Name: " << name << "\n";
    cout << "Year: " << year << "\n";
    cout << "Stability: " << stability << "\n";
    cout << "Population: " << population->getTotalCount() << "\n";
    cout << "Treasury: " << economy->getTreasury() << " gold\n";
    cout << "Military Strength: " << military->calculateStrength() << "\n";
    cout << "Leader: " << leader->getName() << " (" << leader->getLeadershipStyle() << ")\n";
    // Added: Display heir name
    cout << "Heir: " << dynamic_cast<Monarch*>(leader)->getHeir() << "\n"; // NEW: Added heir display
    cout << "\n============================================================================================================\n";
}

// Save kingdom state to file
void Kingdom::saveGameState(const string& filename) const {
    ofstream outFile(filename);
    if (!outFile) {
        throw runtime_error("Failed to open file for saving.");
    }

    outFile << name << "\n";
    outFile << stability << "\n";
    outFile << year << "\n";
    outFile << population->getTotalCount() << "\n";

    for (int i = 0; i < socialClassCount; ++i) {
        outFile << socialClasses[i]->getClassName() << " "
            << socialClasses[i]->getPopulation() << " "
            << socialClasses[i]->getHappiness() << " "
            << socialClasses[i]->getInfluence() << "\n";
    }

    outFile << military->getSoldierCount() << " "
        << military->getMorale() << " "
        << military->getTrainingLevel() << " "
        << military->getEquipmentQuality() << "\n";

    Monarch* monarch = dynamic_cast<Monarch*>(leader); // NEW: Safely cast leader to Monarch
    if (monarch) {
        outFile << leader->getName() << " "
            << leader->getPopularity() << " "
            << leader->getCorruptionLevel() << " "
            << leader->getLeadershipStyle() << " "
            << monarch->getHeir() << "\n"; // Use monarch->getHeir() if cast succeeds
    }
    else {
        outFile << leader->getName() << " "
            << leader->getPopularity() << " "
            << leader->getCorruptionLevel() << " "
            << leader->getLeadershipStyle() << " "
            << "No_Heir" << "\n"; // Fallback if not a Monarch
    }

    outFile << economy->getTreasury() << " "
        << economy->getTaxRate() << " "
        << economy->getInflation() << " "
        << economy->getGDP() << " "
        << economy->getCorruptionLevel() << "\n";

    outFile.close();
}

// Load kingdom state from file
void Kingdom::loadGameState(const string& filename) {
    ifstream inFile(filename);
    if (!inFile) {
        throw runtime_error("Failed to open file for loading: " + filename);
    }

    // Read kingdom name
    if (!getline(inFile, name)) {
        throw runtime_error("Failed to read kingdom name from " + filename);
    }
    // Read stability and year
    if (!(inFile >> stability >> year)) {
        throw runtime_error("Failed to read stability or year from " + filename);
    }

    // Read population
    int populationCount;
    if (!(inFile >> populationCount)) {
        throw runtime_error("Failed to read population count from " + filename);
    }
    delete population;
    population = new Population(populationCount);

    // Clear existing social classes
    for (int i = 0; i < socialClassCount; ++i) {
        delete socialClasses[i];
    }
    socialClassCount = 0;

    // Read social classes
    for (int i = 0; i < MAX_SOCIAL_CLASSES; ++i) {
        string className;
        int pop, happy, infl;
        if (!(inFile >> className >> pop >> happy >> infl)) {
            break; // Stop if no more classes (end of data)
        }
        if (className == "Peasant") {
            socialClasses[socialClassCount++] = new Peasant(pop, happy, infl, 50, 30);
        }
        else if (className == "Merchant") {
            socialClasses[socialClassCount++] = new Merchant(pop, happy, infl, 1000);
        }
        else if (className == "Noble") {
            socialClasses[socialClassCount++] = new Noble(pop, happy, infl, 50);
        }
        else {
            throw runtime_error("Unknown social class '" + className + "' in " + filename);
        }
    }

    // Read military
    int soldiers, morale, training, equipment;
    if (!(inFile >> soldiers >> morale >> training >> equipment)) {
        throw runtime_error("Failed to read military data from " + filename);
    }
    delete military;
    military = new Infantry(soldiers, morale, training, equipment, 50, 100);

    // Read leader (note: heir is not hardcoded)
    string leaderName, style, heirName;
    int popularity, corruption;
    if (!(inFile >> leaderName >> popularity >> corruption >> style >> heirName)) {
        throw runtime_error("Failed to read leader data from " + filename);
    }
    delete leader;
    leader = new Monarch(leaderName, popularity, corruption, style, heirName, 10);

    // Read economy
    int treasury, taxRate, inflation, gdp, corruptionLevel;
    if (!(inFile >> treasury >> taxRate >> inflation >> gdp >> corruptionLevel)) {
        throw runtime_error("Failed to read economy data from " + filename);
    }
    delete economy;
    economy = new Economy(treasury);
    if (taxRate < 0 || taxRate > 100) {
        throw runtime_error("Invalid tax rate " + to_string(taxRate) + " in " + filename + ". Must be between 0 and 100.");
    }
    economy->adjustTaxRate(taxRate);

    inFile.close();
}

// Recalculate kingdom stability
void Kingdom::recalculateStability() {
    int happinessSum = 0;
    for (int i = 0; i < socialClassCount; ++i) {
        happinessSum += socialClasses[i]->getHappiness();
    }
    int averageHappiness = happinessSum / socialClassCount;

    stability = (averageHappiness * 4 / 10) +
        (military->getMorale() * 3 / 10) +
        (leader->getPopularity() * 3 / 10);

    if (stability > 100) stability = 100;
    if (stability < 0) stability = 0;
}

// Adjust kingdom stability with a modifier
void Kingdom::adjustStability(int modifier) {
    stability += modifier;
    if (stability > 100) stability = 100;
    if (stability < 0) stability = 0;
}

// Trigger a random event
void Kingdom::triggerRandomEvent() {
    int eventType = rand() % 3;
    if (eventType == 0) {
        cout << "A famine has struck the kingdom!\n";
        economy->getFood()->consume(100);
        adjustStability(-10);
    }
    else if (eventType == 1) {
        cout << "A plague has spread across the kingdom!\n";
        population->migrate(-50);
        adjustStability(-20);
    }
    else if (eventType == 2) {
        cout << "An invasion threatens the kingdom!\n";
        military->recruit(-20);
        adjustStability(-15);
    }
}

// Resolve all active events
void Kingdom::resolveActiveEvents() {
    cout << "Resolving all active events...\n";
    adjustStability(10);
    economy->getFood()->produce(50);
}

// Display active events
void Kingdom::displayActiveEvents() {
    cout << "Active Events:\n";
    cout << "1. Famine: Food resources are low.\n";
    cout << "2. Plague: Population health is declining.\n";
    cout << "3. Invasion: Military strength is under threat.\n";
}