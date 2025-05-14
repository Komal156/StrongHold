#include "stronghold.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <sstream> // Added for istringstream
using namespace std;

Kingdom::Kingdom(const string& kingdomName, int id, const string& leaderName, const string& heirname)
    : name(kingdomName), stability(50), year(1), socialClassCount(0), mapX(0), mapY(0), kingdomId(id) {
    population = new Population(1000);
    socialClasses[socialClassCount++] = new Peasant(500, 40, 20, 50, 30);
    socialClasses[socialClassCount++] = new Merchant(300, 50, 30, 1000);
    socialClasses[socialClassCount++] = new Noble(200, 50, 50, 50);
    military = new Infantry(100, 80, 50, 70, 50, 100);
    leader = new Monarch(leaderName, 50, 10, "Benevolent", heirname, 10);
    economy = new Economy(5000);
}

Kingdom::~Kingdom() {
    delete population;
    delete military;
    delete leader;
    delete economy;
    for (int i = 0; i < socialClassCount; ++i) {
        delete socialClasses[i];
    }
}

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
            military->recruit(-military->getSoldierCount() / 2);
            adjustStability(-20);
            cout << defender.getName() << " repels the attack from " << name << "!\n";
        }
    }
    catch (const runtime_error& e) {
        cerr << "Error during attack: " << e.what() << "\n";
    }
}

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
    Monarch* monarch = dynamic_cast<Monarch*>(leader);
    cout << "Heir: " << (monarch ? monarch->getHeir() : "N/A") << "\n";
    cout << "Map Position: (" << mapX << ", " << mapY << ")\n";
    cout << "\n============================================================================================================\n";
}

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

    Monarch* monarch = dynamic_cast<Monarch*>(leader);
    if (monarch) {
        outFile << leader->getName() << " "
            << leader->getPopularity() << " "
            << leader->getCorruptionLevel() << " "
            << leader->getLeadershipStyle() << " "
            << monarch->getHeir() << "\n";
    }
    else {
        outFile << leader->getName() << " "
            << leader->getPopularity() << " "
            << leader->getCorruptionLevel() << " "
            << leader->getLeadershipStyle() << " "
            << "No_Heir" << "\n";
    }

    outFile << economy->getTreasury() << " "
        << economy->getTaxRate() << " "
        << economy->getInflation() << " "
        << economy->getGDP() << " "
        << economy->getCorruptionLevel() << "\n";

    outFile << mapX << " " << mapY << " " << kingdomId << "\n";

    outFile.close();
}

bool Kingdom::loadGameState(const string& filename) {
    ifstream inFile(filename);
    if (!inFile) {
        cerr << "Failed to open file for loading: " << filename << "\n";
        return false;
    }

    string tempName;
    if (!getline(inFile, tempName)) {
        inFile.close();
        cerr << "Failed to read kingdom name from " << filename << "\n";
        return false;
    }
    name = tempName;

    if (!(inFile >> stability >> year)) {
        inFile.close();
        cerr << "Failed to read stability or year from " << filename << "\n";
        return false;
    }
    inFile.ignore(numeric_limits<streamsize>::max(), '\n');

    int populationCount;
    if (!(inFile >> populationCount)) {
        inFile.close();
        cerr << "Failed to read population count from " << filename << "\n";
        return false;
    }
    inFile.ignore(numeric_limits<streamsize>::max(), '\n');
    delete population;
    population = new Population(populationCount);

    for (int i = 0; i < socialClassCount; ++i) {
        delete socialClasses[i];
    }
    socialClassCount = 0;

    string line;
    while (socialClassCount < MAX_SOCIAL_CLASSES && getline(inFile, line)) {
        string className;
        int pop, happy, infl;
        istringstream iss(line);
        if (!(iss >> className >> pop >> happy >> infl)) {
            break;
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
            inFile.close();
            cerr << "Unknown social class '" << className << "' in " << filename << "\n";
            return false;
        }
    }

    int soldiers, morale, training, equipment;
    if (!(inFile >> soldiers >> morale >> training >> equipment)) {
        inFile.close();
        cerr << "Failed to read military data from " << filename << "\n";
        return false;
    }
    inFile.ignore(numeric_limits<streamsize>::max(), '\n');
    delete military;
    military = new Infantry(soldiers, morale, training, equipment, 50, 100);

    string leaderName, style, heirName;
    int popularity, corruption;
    if (!(inFile >> leaderName >> popularity >> corruption >> style >> heirName)) {
        inFile.close();
        cerr << "Failed to read leader data from " << filename << "\n";
        return false;
    }
    inFile.ignore(numeric_limits<streamsize>::max(), '\n');
    delete leader;
    leader = new Monarch(leaderName, popularity, corruption, style, heirName, 10);

    int treasury, taxRate, inflation, gdp, corruptionLevel;
    if (!(inFile >> treasury >> taxRate >> inflation >> gdp >> corruptionLevel)) {
        inFile.close();
        cerr << "Failed to read economy data from " << filename << "\n";
        return false;
    }
    inFile.ignore(numeric_limits<streamsize>::max(), '\n');
    delete economy;
    economy = new Economy(treasury);
    if (taxRate < 0 || taxRate > 100) {
        inFile.close();
        cerr << "Invalid tax rate " << taxRate << " in " << filename << "\n";
        return false;
    }
    economy->adjustTaxRate(taxRate);

    int tempMapX, tempMapY, tempKingdomId;
    if (!(inFile >> tempMapX >> tempMapY >> tempKingdomId)) {
        inFile.close();
        cerr << "Failed to read map coordinates or ID from " << filename << "\n";
        return false;
    }
    if (tempMapX < 0 || tempMapX >= MAP_SIZE || tempMapY < 0 || tempMapY >= MAP_SIZE) {
        inFile.close();
        cerr << "Invalid map coordinates in " << filename << "\n";
        return false;
    }
    if (tempKingdomId < 1 || tempKingdomId > MAX_KINGDOMS) {
        inFile.close();
        cerr << "Invalid kingdom ID in " << filename << "\n";
        return false;
    }
    mapX = tempMapX;
    mapY = tempMapY;
    kingdomId = tempKingdomId;

    inFile.close();
    return true;
}

void Kingdom::recalculateStability() {
    int happinessSum = 0;
    for (int i = 0; i < socialClassCount; ++i) {
        happinessSum += socialClasses[i]->getHappiness();
    }
    int averageHappiness = (socialClassCount > 0) ? (happinessSum / socialClassCount) : 50; // Default to 50 if no social classes

    stability = (averageHappiness * 4 / 10) +
        (military->getMorale() * 3 / 10) +
        (leader->getPopularity() * 3 / 10);

    if (stability > 100) stability = 100;
    if (stability < 0) stability = 0;
}

void Kingdom::adjustStability(int modifier) {
    stability += modifier;
    if (stability > 100) stability = 100;
    if (stability < 0) stability = 0;
}

void Kingdom::triggerRandomEvent() {
    // Simplified to avoid dependencies on unimplemented Event class
    int eventType = rand() % 3;
    if (eventType == 0) {
        cout << "A famine has struck the kingdom!\n";
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

void Kingdom::resolveActiveEvents() {
    // Simplified to avoid dependencies
    cout << "Resolving all active events...\n";
    adjustStability(10);
}

void Kingdom::displayActiveEvents() {
    // Simplified to avoid dependencies
    cout << "Active Events (simulated):\n";
    cout << "1. Famine: Stability reduced.\n";
    cout << "2. Plague: Population reduced.\n";
    cout << "3. Invasion: Military strength reduced.\n";
}