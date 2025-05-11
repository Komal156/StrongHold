#include "stronghold.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <chrono>
#include <limits>
using namespace std;

void displayMainMenu();
void playGame(World& world);
void displayKingdomOptions(World& world, Kingdom& kingdom);
void handleSocialClassInteraction(Kingdom& kingdom);
void handlePopulationActions(Kingdom& kingdom);
void handleMilitaryActions(Kingdom& kingdom);
void handleLeadershipActions(Kingdom& kingdom);
void handleEconomyActions(Kingdom& kingdom);
void handleBankActions(Kingdom& kingdom);
void handleResourceManagement(Kingdom& kingdom);
void handleEventSystem(Kingdom& kingdom);
void handleMultiplayerActions(World& world, Kingdom& kingdom);
void logGameEvent(const string& event, const Kingdom& kingdom);
void displayLogs();
int getValidatedInput(int min, int max);

int main() {
    cout << "\n============================================================================================================\n";
    cout << "\n                                       STRONGHOLD: CORE KINGDOM ENGINE                                      \n";
    cout << "\n============================================================================================================\n";
    World* world = new World();
    bool gameRunning = true;

    while (gameRunning) {
        displayMainMenu();
        int choice = getValidatedInput(1, 4);

        try {
            switch (choice) {
            case 1: { // New Game
                int numKingdoms;
                cout << "Enter number of kingdoms (2-4): ";
                numKingdoms = getValidatedInput(2, 4);
                for (int i = 0; i < numKingdoms; i++) {
                    string kingdomName;
                    cout << "Enter name for kingdom " << i + 1 << ": ";
                    getline(cin, kingdomName);
                    Kingdom* kingdom = new Kingdom(kingdomName, i + 1);
                    world->addKingdom(kingdom);
                    logGameEvent("Kingdom " + kingdomName + " was founded", *kingdom);
                }
                playGame(*world);
                break;
            }
            case 2: { // Load Game
                string filename;
                cout << "Enter save file name: ";
                getline(cin, filename);
                try {
                    World* newWorld = new World();
                    int numKingdoms;
                    cout << "Enter number of kingdoms to load: ";
                    numKingdoms = getValidatedInput(1, 4);
                    for (int i = 0; i < numKingdoms; i++) {
                        string kingdomName;
                        cout << "Enter name for kingdom " << i + 1 << ": ";
                        getline(cin, kingdomName);
                        Kingdom* kingdom = new Kingdom(kingdomName, i + 1);
                        kingdom->loadGameState(filename);
                        newWorld->addKingdom(kingdom);
                        logGameEvent("Kingdom " + kingdomName + " loaded from " + filename, *kingdom);
                    }
                    delete world; // Delete old world only after successful loading
                    world = newWorld;
                    playGame(*world);
                }
                catch (const exception& e) {
                    cerr << "Error loading game: " << e.what() << "\n";
                }
                break;
            }
            case 3: // View Logs
                displayLogs();
                break;

            case 4:
                displayMainMenu();
                // Return to Game
            case 5: // Exit #exit
                gameRunning = false;
                break;
            default:
                cout << "Invalid choice. Try again.\n";
            }
        }
        catch (const exception& e) {
            cerr << "Error: " << e.what() << "\n";
        }
    }

    delete world;
    return 0;
}

int getValidatedInput(int min, int max) {
    int input;
    while (true) {
        if (!(cin >> input)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number: ";
        }
        else if (input < min || input > max) {
            cout << "Invalid input. Please enter a number between " << min << " and " << max << ": ";
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return input;
        }
    }
}

void displayMainMenu() {
    cout << "\n============================================================================================================\n";
    cout << "\n                                                    Main Menu:                                              \n";
    cout << "\n============================================================================================================\n";
    cout << "1. New Game\n";
    cout << "2. Load Game\n";
    cout << "3. View Game Logs\n";
    cout << "4. Return to Game\n";
    cout << "5. Exit\n";
    cout << "Enter your choice: ";
}

void playGame(World& world) {
    bool inGame = true;
    int currentKingdomIndex = 0; // Track the current kingdom

    while (inGame) {
        try {
            world.simulateTurn();
            Kingdom* currentKingdom = world.getKingdoms()[currentKingdomIndex];
            cout << "\nCurrently in Kingdom: " << currentKingdom->getName() << "\n";
            currentKingdom->displayStatus();
            displayKingdomOptions(world, *currentKingdom);

            int choice = getValidatedInput(1, 13); // Add an extra option for switching kingdoms
            switch (choice) {
            case 1: // Advance Year
                logGameEvent("Year " + to_string(currentKingdom->getYear()) + " advanced", *currentKingdom);
                break;
            case 2: // Social Class Interactions
                handleSocialClassInteraction(*currentKingdom);
                break;
            case 3: // Population Actions
                handlePopulationActions(*currentKingdom);
                break;
            case 4: // Military Actions
                handleMilitaryActions(*currentKingdom);
                break;
            case 5: // Leadership Actions
                handleLeadershipActions(*currentKingdom);
                break;
            case 6: // Economy Actions
                handleEconomyActions(*currentKingdom);
                break;
            case 7: // Bank Actions
                handleBankActions(*currentKingdom);
                break;
            case 8: // Resource Management
                handleResourceManagement(*currentKingdom);
                break;
            case 9: // Event System
                handleEventSystem(*currentKingdom);
                break;
            case 10: // Multiplayer Actions
                handleMultiplayerActions(world, *currentKingdom);
                break;
            case 11: { // Save Game
                string filename;
                cout << "Enter save file name: ";
                getline(cin, filename);
                currentKingdom->saveGameState(filename);
                logGameEvent("Game saved to " + filename, *currentKingdom);
                cout << "Game saved successfully.\n";
                break;
            }
            case 12: // Return to Main Menu
                inGame = false;
                break;
            case 13: { // Switch Kingdom
                cout << "Enter kingdom index (1-" << world.getKingdomCount() << "): ";
                int newIndex = getValidatedInput(1, world.getKingdomCount()) - 1;
                currentKingdomIndex = newIndex;
                break;
            }
            default:
                cout << "Invalid choice. Try again.\n";
            }
        }
        catch (const exception& e) {
            cerr << "Error during game simulation: " << e.what() << "\n";
        }
    }
}

void displayKingdomOptions(World& world, Kingdom& kingdom) {
    cout << "\n============================================================================================================\n";
    cout << "\n                                                 : Kingdom Options :                                        \n";
    cout << "\n============================================================================================================\n";
    cout << "1. Advance to Next Year\n";
    cout << "2. Social Class Interactions\n";
    cout << "3. Population Actions\n";
    cout << "4. Military Actions\n";
    cout << "5. Leadership Actions\n";
    cout << "6. Economy Actions\n";
    cout << "7. Bank Actions\n";
    cout << "8. Resource Management\n";
    cout << "9. Event System\n";
    cout << "10. Multiplayer Actions\n";
    cout << "11. Save Game\n";
    cout << "12. Return to Main Menu\n";
    cout << "13. Switch Kingdom\n"; // New option
    cout << "Enter your choice: ";
}

void handleMultiplayerActions(World& world, Kingdom& kingdom) {
    cout << "\n============================================================================================================\n";
    cout << "\n                                             : Multiplayer Actions :                                        \n";
    cout << "\n============================================================================================================\n";
    cout << "1. Send Message\n";
    cout << "2. View Messages\n";
    cout << "3. Form Alliance\n";
    cout << "4. Break Alliance\n";
    cout << "5. Propose Trade\n";
    cout << "6. Propose Smuggling\n";
    cout << "7. View Market\n";
    cout << "8. Declare War\n";
    cout << "9. Move Kingdom\n";
    cout << "10. View Map\n";
    cout << "11. Back\n";
    cout << "Enter your choice: ";

    int choice = getValidatedInput(1, 11);
    switch (choice) {
    case 1: { // Send Message
        string recipient, content;
        cout << "Enter recipient kingdom name: ";
        getline(cin, recipient);
        cout << "Enter message content: ";
        getline(cin, content);
        world.sendMessage(kingdom.getName(), recipient, content);
        logGameEvent("Message sent to " + recipient, kingdom);
        break;
    }
    case 2: // View Messages
        world.displayMessages(kingdom.getName());
        break;
    case 3: { // Form Alliance
        string ally, type;
        int duration;
        cout << "Enter ally kingdom name: ";
        getline(cin, ally);
        cout << "Enter alliance type (Alliance/Treaty): ";
        getline(cin, type);
        cout << "Enter duration (years): ";
        duration = getValidatedInput(1, 10);
        world.formAlliance(kingdom.getName(), ally, type, duration);
        logGameEvent("Alliance formed with " + ally, kingdom);
        break;
    }
    case 4: { // Break Alliance
        string ally;
        cout << "Enter ally kingdom name to break alliance with: ";
        getline(cin, ally);
        world.breakAlliance(kingdom.getName(), ally);
        logGameEvent("Alliance broken with " + ally, kingdom);
        break;
    }
    case 5: { // Propose Trade
        string target, res, reqRes;
        int qtyO, qtyR;
        cout << "Enter target kingdom name: ";
        getline(cin, target);
        cout << "Enter resource to offer (Food/Wood/Iron): ";
        getline(cin, res);
        cout << "Enter quantity to offer: ";
        qtyO = getValidatedInput(1, 1000);
        cout << "Enter requested resource (Food/Wood/Iron): ";
        getline(cin, reqRes);
        cout << "Enter requested quantity: ";
        qtyR = getValidatedInput(1, 1000);
        world.proposeTrade(kingdom.getName(), target, res, qtyO, reqRes, qtyR);
        logGameEvent("Trade proposed to " + target, kingdom);
        break;
    }
    case 6: { // Propose Smuggling
        string target, res, reqRes;
        int qtyO, qtyR, risk;
        cout << "Enter target kingdom name: ";
        getline(cin, target);
        cout << "Enter resource to offer (Food/Wood/Iron): ";
        getline(cin, res);
        cout << "Enter quantity to offer: ";
        qtyO = getValidatedInput(1, 1000);
        cout << "Enter requested resource (Food/Wood/Iron): ";
        getline(cin, reqRes);
        cout << "Enter requested quantity: ";
        qtyR = getValidatedInput(1, 1000);
        cout << "Enter risk level (0-100): ";
        risk = getValidatedInput(0, 100);
        world.proposeSmuggle(kingdom.getName(), target, res, qtyO, reqRes, qtyR, risk);
        logGameEvent("Smuggling proposed to " + target, kingdom);
        break;
    }
    case 7: // View Market
        world.getMarket()->displayTrades();
        world.getMarket()->displaySmuggles();
        break;
    case 8: { // Declare War
        string defender;
        cout << "Enter kingdom to declare war on: ";
        getline(cin, defender);
        world.declareWar(kingdom.getName(), defender);
        logGameEvent("War declared on " + defender, kingdom);
        break;
    }
    case 9: { // Move Kingdom
        int x, y;
        cout << "Enter new X coordinate (0-4): ";
        x = getValidatedInput(0, 4);
        cout << "Enter new Y coordinate (0-4): ";
        y = getValidatedInput(0, 4);
        world.moveKingdom(kingdom.getName(), x, y);
        logGameEvent("Kingdom moved to (" + to_string(x) + "," + to_string(y) + ")", kingdom);
        break;
    }
    case 10: // View Map
        world.getMap()->display();
        break;
    case 11: // Back
        return;
    default:
        cout << "Invalid choice.\n";
    }
}

void handleLeadershipActions(Kingdom& kingdom) {
    Leader* leader = kingdom.getLeader();
    cout << "\n============================================================================================================\n";
    cout << "\n                                              :Leadership Actions:                                          \n";
    cout << "\n============================================================================================================\n";
    cout << "1. Make Decision\n";
    cout << "2. Address Crisis\n";
    cout << "3. Hold Election (if applicable)\n";
    cout << "4. View Leader Details\n";
    cout << "5. Back\n";
    cout << "Enter your choice: ";

    int choice = getValidatedInput(1, 5);

    switch (choice) {
    case 1: {
        cout << "\n============================================================================================================\n";
        cout << "\n                                               :Decision Options:                                           \n";
        cout << "\n============================================================================================================\n";
        cout << "1. Harsh Decision (order, control)\n";
        cout << "2. Moderate Decision (balance)\n";
        cout << "3. Benevolent Decision (people first)\n";
        cout << "Enter choice: ";
        int decision = getValidatedInput(1, 3);
        leader->makeDecision(decision);
        kingdom.adjustStability(2);
        logGameEvent("Leader made decision type " + to_string(decision), kingdom);
        break;
    }
    case 2:
        leader->addressCrisis();
        kingdom.adjustStability(2);
        logGameEvent("Leader addressed a crisis", kingdom);
        break;
    case 3:
        if (leader->holdElection()) {
            cout << "Election held! New leader may take over.\n";
            kingdom.adjustStability(1);
            logGameEvent("Election was held", kingdom);
        }
        else {
            cout << "This leader doesn't hold elections.\n";
        }
        break;
    case 4:
        cout << "\n============================================================================================================\n";
        cout << "\n                                              :Leader Details:                                              \n";
        cout << "\n============================================================================================================\n";
        cout << "Name: " << leader->getName() << "\n";
        cout << "Style: " << leader->getLeadershipStyle() << "\n";
        cout << "Popularity: " << leader->getPopularity() << "%\n";
        cout << "Corruption: " << leader->getCorruptionLevel() << "%\n";
        break;
    case 5:
        return;
    default:
        cout << "Invalid choice.\n";
    }
}

void handleSocialClassInteraction(Kingdom& kingdom) {
    SocialClass** socialClasses = kingdom.getSocialClasses();
    int classCount = kingdom.getSocialClassCount();
    cout << "\n============================================================================================================\n";
    cout << "\n                                       :Social Class Interactions:                                          \n";
    cout << "\n============================================================================================================\n";
    cout << "1. Collect Taxes\n";
    cout << "2. Address Class Demands\n";
    cout << "3. View Social Class Details\n";
    cout << "4. Back\n";
    cout << "Enter your choice: ";

    int choice = getValidatedInput(1, 4);

    switch (choice) {
    case 1: {
        int totalTaxes = 0;
        for (int i = 0; i < classCount; ++i) {
            int tax = socialClasses[i]->calculateTaxContribution();
            totalTaxes += tax;
        }
        kingdom.getEconomy()->addToTreasury(totalTaxes);
        kingdom.adjustStability(1);
        logGameEvent("Collected taxes from all social classes", kingdom);
        break;
    }
    case 2:
        for (int i = 0; i < classCount; ++i) {
            if (socialClasses[i]->getHappiness() < 50) {
                socialClasses[i]->demandChange();
            }
        }
        kingdom.adjustStability(1);
        logGameEvent("Addressed demands of social classes", kingdom);
        break;
    case 3:
        for (int i = 0; i < classCount; ++i) {
            cout << socialClasses[i]->getClassName() << " - Population: " << socialClasses[i]->getPopulation()
                << ", Happiness: " << socialClasses[i]->getHappiness() << "%\n";
        }
        break;
    case 4:
        return;
    default:
        cout << "Invalid choice.\n";
    }
}

void handlePopulationActions(Kingdom& kingdom) {
    Population* population = kingdom.getPopulation();
    cout << "\n============================================================================================================\n";
    cout << "\n                                              :Population Actions:                                          \n";
    cout << "\n============================================================================================================\n";
    cout << "1. View Population Details\n";
    cout << "2. Adjust Growth Rate\n";
    cout << "3. Improve Health\n";
    cout << "4. Back\n";
    cout << "Enter your choice: ";

    int choice = getValidatedInput(1, 4);

    switch (choice) {
    case 1:
        cout << "Total Population: " << population->getTotalCount() << "\n";
        cout << "Growth Rate: " << population->getGrowthRate() << "%\n";
        cout << "Health: " << population->getHealth() << "%\n";
        cout << "Employment Rate: " << population->getEmploymentRate() << "%\n";
        break;
    case 2: {
        int food, housing;
        cout << "Enter food availability (0-100): ";
        food = getValidatedInput(0, 100);
        cout << "Enter housing quality (0-100): ";
        housing = getValidatedInput(0, 100);
        population->calculateGrowth(food, housing);
        kingdom.adjustStability(2);
        logGameEvent("Population growth rate adjusted", kingdom);
        break;
    }
    case 3: {
        int sanitation, medicalCare;
        cout << "Enter sanitation level (0-100): ";
        sanitation = getValidatedInput(0, 100);
        cout << "Enter medical care level (0-100): ";
        medicalCare = getValidatedInput(0, 100);
        population->updateHealth(sanitation, medicalCare);
        kingdom.adjustStability(2);
        logGameEvent("Population health improved", kingdom);
        break;
    }
    case 4:
        return;
    default:
        cout << "Invalid choice.\n";
    }
}

void handleMilitaryActions(Kingdom& kingdom) {
    ArmyUnit* military = kingdom.getMilitary();
    cout << "\n============================================================================================================\n";
    cout << "\n                                            :Military Actions:                                              \n";
    cout << "\n============================================================================================================\n";
    cout << "1. Recruit Soldiers\n";
    cout << "2. Train Army\n";
    cout << "3. View Military Strength\n";
    cout << "4. Back\n";
    cout << "Enter your choice: ";

    int choice = getValidatedInput(1, 4);

    switch (choice) {
    case 1: {
        int amount;
        cout << "Enter number of soldiers to recruit: ";
        amount = getValidatedInput(0, 1000);
        military->recruit(amount);
        kingdom.adjustStability(2);
        logGameEvent("Recruited " + to_string(amount) + " soldiers", kingdom);
        break;
    }
    case 2: {
        int intensity;
        cout << "Enter training intensity (0-100): ";
        intensity = getValidatedInput(0, 100);
        military->train(intensity);
        kingdom.adjustStability(2);
        logGameEvent("Army trained with intensity " + to_string(intensity), kingdom);
        break;
    }
    case 3:
        cout << "Military Strength: " << military->calculateStrength() << "\n";
        break;
    case 4:
        return;
    default:
        cout << "Invalid choice.\n";
    }
}

void handleEconomyActions(Kingdom& kingdom) {
    Economy* economy = kingdom.getEconomy();
    cout << "\n============================================================================================================\n";
    cout << "\n                                                   :Economy Actions:                                        \n";
    cout << "\n============================================================================================================\n";
    cout << "1. Adjust Tax Rate\n";
    cout << "2. Fund Public Service\n";
    cout << "3. View Economy Details\n";
    cout << "4. Back\n";
    cout << "Enter your choice: ";

    int choice = getValidatedInput(1, 4);

    switch (choice) {
    case 1: {
        int newRate;
        cout << "Enter new tax rate (0-100): ";
        newRate = getValidatedInput(0, 100);
        economy->adjustTaxRate(newRate);
        if (newRate < 20) {
            kingdom.adjustStability(1);
        }
        else {
            kingdom.adjustStability(-1);
        }
        logGameEvent("Tax rate adjusted to " + to_string(newRate), kingdom);
        break;
    }
    case 2: {
        int amount;
        cout << "Enter amount to fund public service: ";
        amount = getValidatedInput(0, economy->getTreasury());
        economy->fundService(amount);
        kingdom.adjustStability(2);
        logGameEvent("Funded public service with " + to_string(amount) + " gold", kingdom);
        break;
    }
    case 3:
        cout << "\n============================================================================================================\n";
        cout << "\n                                               :Economy Details:                                            \n";
        cout << "\n============================================================================================================\n";
        cout << "Treasury: " << economy->getTreasury() << " gold\n";
        cout << "Tax Rate: " << economy->getTaxRate() << "%\n";
        cout << "Inflation: " << economy->getInflation() << "%\n";
        cout << "GDP: " << economy->getGDP() << "\n";
        cout << "Corruption Level: " << economy->getCorruptionLevel() << "%\n";
        break;
    case 4:
        return;
    default:
        cout << "Invalid choice.\n";
    }
}

void handleBankActions(Kingdom& kingdom) {
    Bank* bank = kingdom.getEconomy()->getBank();
    cout << "\n============================================================================================================\n";
    cout << "\n                                                       :Bank Actions:                                       \n";
    cout << "\n============================================================================================================\n";
    cout << "1. Issue Loan\n";
    cout << "2. Collect Debt\n";
    cout << "3. Audit Bank\n";
    cout << "4. View Bank Details\n";
    cout << "5. Back\n";
    cout << "Enter your choice: ";

    int choice = getValidatedInput(1, 5);

    switch (choice) {
    case 1: {
        int amount, termYears;
        string lender;
        cout << "Enter loan amount: ";
        amount = getValidatedInput(1, bank->getGoldReserves());
        cout << "Enter loan term in years: ";
        termYears = getValidatedInput(1, 30);
        cout << "Enter lender name: ";
        getline(cin, lender);
        Loan* loan = bank->issueLoan(amount, termYears, lender);
        if (loan) {
            cout << "Loan issued successfully.\n";
            logGameEvent("Loan issued to " + lender, kingdom);
        }
        else {
            cout << "Failed to issue loan.\n";
        }
        break;
    }
    case 2:
        bank->collectDebt();
        logGameEvent("Bank collected debt", kingdom);
        break;
    case 3: {
        int corruptionThreshold;
        cout << "Enter corruption threshold for audit: ";
        corruptionThreshold = getValidatedInput(0, 100);
        bank->audit(corruptionThreshold);
        logGameEvent("Bank audit conducted", kingdom);
        break;
    }
    case 4:
        cout << "\nBank Details:\n";
        cout << "Gold Reserves: " << bank->getGoldReserves() << "\n";
        cout << "Active Loans: " << bank->getLoanCount() << "\n";
        break;
    case 5:
        return;
    default:
        cout << "Invalid choice.\n";
    }
}

void handleResourceManagement(Kingdom& kingdom) {
    Economy* economy = kingdom.getEconomy();
    cout << "\n============================================================================================================\n";
    cout << "\n                                                  :Resource Management:                                     \n";
    cout << "\n============================================================================================================\n";
    cout << "1. Produce Food\n";
    cout << "2. Harvest Wood\n";
    cout << "3. Mine Iron\n";
    cout << "4. View Resource Details\n";
    cout << "5. Back\n";
    cout << "Enter your choice: ";

    int choice = getValidatedInput(1, 5);

    switch (choice) {
    case 1: {
        int efficiency;
        cout << "Enter production efficiency (0-100): ";
        efficiency = getValidatedInput(0, 100);
        economy->getFood()->produce(efficiency);
        logGameEvent("Food production increased", kingdom);
        break;
    }
    case 2: {
        int efficiency;
        cout << "Enter harvesting efficiency (0-100): ";
        efficiency = getValidatedInput(0, 100);
        economy->getWood()->produce(efficiency);
        logGameEvent("Wood harvesting increased", kingdom);
        break;
    }
    case 3: {
        int efficiency;
        cout << "Enter mining efficiency (0-100): ";
        efficiency = getValidatedInput(0, 100);
        economy->getIron()->produce(efficiency);
        logGameEvent("Iron mining increased", kingdom);
        break;
    }
    case 4:
        cout << "\n============================================================================================================\n";
        cout << "\n                                              :Resource Details:                                            \n";
        cout << "\n============================================================================================================\n";
        cout << "Food: " << economy->getFood()->getQuantity() << "\n";
        cout << "Wood: " << economy->getWood()->getQuantity() << "\n";
        cout << "Iron: " << economy->getIron()->getQuantity() << "\n";
        break;
    case 5:
        return;
    default:
        cout << "Invalid choice.\n";
    }
}

void handleEventSystem(Kingdom& kingdom) {
    cout << "\n============================================================================================================\n";
    cout << "\n                                                      :Event System:                                        \n";
    cout << "\n============================================================================================================\n";
    cout << "1. Trigger Random Event\n";
    cout << "2. Resolve Active Events\n";
    cout << "3. View Active Events\n";
    cout << "4. Back\n";
    cout << "Enter your choice: ";

    int choice = getValidatedInput(1, 4);

    switch (choice) {
    case 1:
        kingdom.triggerRandomEvent();
        logGameEvent("Random event triggered", kingdom);
        break;
    case 2:
        kingdom.resolveActiveEvents();
        logGameEvent("Active events resolved", kingdom);
        break;
    case 3:
        kingdom.displayActiveEvents();
        break;
    case 4:
        return;
    default:
        cout << "Invalid choice.\n";
    }
}

void logGameEvent(const string& event, const Kingdom& kingdom) {
    ofstream logFile("game_logs.txt", ios::app);
    if (logFile.is_open()) {
        logFile << "Year " << kingdom.getYear() << ": " << event << "\n";
        logFile.close();
    }
    else {
        cerr << "Failed to open log file.\n";
    }
}

void displayLogs() {
    ifstream logFile("game_logs.txt");
    if (logFile.is_open()) {
        cout << "\n=== Game Logs ===\n";
        string line;
        while (getline(logFile, line)) {
            cout << line << "\n";
        }
        logFile.close();
    }
    else {
        cerr << "No logs available.\n";
    }
}