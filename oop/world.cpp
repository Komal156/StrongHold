#include "stronghold.h"
#include <iostream>
using namespace std;

World::World() : kingdomCount(0), messageCount(0), allianceCount(0), currentYear(1) {
    for (int i = 0; i < MAX_KINGDOMS; i++) {
        kingdoms[i] = nullptr;  //to pervent dangling pointer
    }
    for (int i = 0; i < MAX_MESSAGES; i++) {
        messages[i] = nullptr;
    }
    for (int i = 0; i < MAX_ALLIANCES; i++) {
        alliances[i] = nullptr;
    }
    market = new Market();
    map = new Map();
}

World::~World() {
    for (int i = 0; i < kingdomCount; i++) {
        delete kingdoms[i];
        kingdoms[i] = nullptr;
    }
    for (int i = 0; i < messageCount; i++) {
        delete messages[i];
        messages[i] = nullptr;
    }
    for (int i = 0; i < MAX_ALLIANCES; i++) {
        delete alliances[i];
        alliances[i] = nullptr;
    }
    delete market;
    market = nullptr;
    delete map;
    map = nullptr;
}

void World::addKingdom(Kingdom* kingdom)
{
    if (kingdomCount >= MAX_KINGDOMS) 
    {
        throw runtime_error("Maximum kingdom limit reached.");
    }
    kingdoms[kingdomCount] = kingdom;  //Assigns the kingdom pointer to the next available slot in the kingdoms array.
    int x = kingdomCount % MAP_SIZE;   //row   Attempts to place the kingdom in a row based on the count.
    int y = kingdomCount % MAP_SIZE;    //col
    bool found = false;
    for (int i = 0; i < MAP_SIZE * MAP_SIZE; i++)    //Searches all possible positions on the grid.
    {
        x = (kingdomCount + i) % MAP_SIZE;
        y = (kingdomCount + i) % MAP_SIZE;
        if (!map->isOccupied(x, y))
        {
            found = true;
            break;
        }
    }
    if (!found) {
        throw runtime_error("No available map positions for new kingdom.");
    }
    map->assignKingdom(kingdom->getName(), x, y, kingdomCount + 1);   //update map
    kingdom->setMapPosition(x, y); 
    kingdomCount++;
}

void World::addKingdom(Kingdom* kingdom, int x, int y, int id) {
    if (kingdomCount >= MAX_KINGDOMS) {
        throw runtime_error("Maximum kingdom limit reached.");
    }
    if (id < 1 || id > MAX_KINGDOMS) {
        throw runtime_error("Invalid kingdom ID during load.");
    }
    kingdoms[kingdomCount] = kingdom;
    if (map->isOccupied(x, y)) {
        bool found = false;
        for (int i = 0; i < MAP_SIZE * MAP_SIZE; i++) {
            x = (x + i) % MAP_SIZE;
            y = (y + i) % MAP_SIZE;
            if (!map->isOccupied(x, y)) {
                found = true;
                break;
            }
        }
        if (!found) {
            throw runtime_error("No available map positions for loaded kingdom.");
        }
        cout << "Warning: Map position (" << kingdom->getMapX() << "," << kingdom->getMapY() << ") was occupied. Moved to (" << x << "," << y << ").\n";
    }
    map->assignKingdom(kingdom->getName(), x, y, id);
    kingdom->setMapPosition(x, y);
    kingdomCount++;
}

void World::simulateTurn() {
    currentYear++;
    for (int i = 0; i < kingdomCount; i++) {
        if (kingdoms[i]) {
            kingdoms[i]->simulateTurn();
        }
    }
    market->processTrades(kingdoms, kingdomCount);
}

void World::sendMessage(const string& sender, const string& recipient, const string& content) {
    try {
        if (messageCount >= MAX_MESSAGES) {
            throw runtime_error("Message inbox full.");
        }
        if (findKingdom(sender) == nullptr || findKingdom(recipient) == nullptr) {
            throw runtime_error("Invalid sender or recipient.");
        }
        messages[messageCount++] = new Message(sender, recipient, content, currentYear);
        cout << "Message sent from " << sender << " to " << recipient << ".\n";
    }
    catch (const runtime_error& e) {
        cerr << "Error sending message: " << e.what() << "\n";
    }
}

void World::displayMessages(const string& kingdomName) const {
    cout << "Messages for " << kingdomName << ":\n";
    bool found = false;
    for (int i = 0; i < messageCount; i++) {
        if (messages[i] && messages[i]->getRecipient() == kingdomName) {
            messages[i]->display();
            found = true;
        }
    }
    if (!found) {
        cout << "No messages.\n";
    }
}

void World::formAlliance(const string& k1, const string& k2, const string& type, int duration) {
    try {
        if (allianceCount >= MAX_ALLIANCES) {
            throw runtime_error("Maximum alliance limit reached.");
        }
        if (findKingdom(k1) == nullptr || findKingdom(k2) == nullptr) {
            throw runtime_error("Invalid kingdoms for alliance.");
        }
        alliances[allianceCount++] = new Alliance(k1, k2, type, currentYear, duration);
        cout << k1 << " and " << k2 << " formed a " << type << ".\n";
    }
    catch (const runtime_error& e) {
        cerr << "Error forming alliance: " << e.what() << "\n";
    }
}

void World::breakAlliance(const string& k1, const string& k2) {
    try {
        for (int i = 0; i < allianceCount; i++) {
            if (alliances[i] && alliances[i]->isActive() &&
                ((alliances[i]->getKingdom1() == k1 && alliances[i]->getKingdom2() == k2) ||
                    (alliances[i]->getKingdom1() == k2 && alliances[i]->getKingdom2() == k1))) {
                alliances[i]->breakAlliance();
                Kingdom* kingdom1 = findKingdom(k1);
                Kingdom* kingdom2 = findKingdom(k2);
                if (kingdom1 && kingdom2) {
                    kingdom1->adjustStability(-10);
                    kingdom2->adjustStability(-10);
                }
                cout << "Alliance between " << k1 << " and " << k2 << " broken.\n";
                return;
            }
        }
        throw runtime_error("No active alliance found.");
    }
    catch (const runtime_error& e) {
        cerr << "Error breaking alliance: " << e.what() << "\n";
    }
}

void World::proposeTrade(const string& proposer, const string& target, const string& res, int qtyO, const string& reqRes, int qtyR) {
    try {
        if (hasEmbargo(proposer, target)) {
            throw runtime_error("Cannot trade due to embargo.");
        }
        TradeOffer* trade = new TradeOffer(proposer, target, res, qtyO, reqRes, qtyR);
        if (!trade->isValid()) {
            delete trade;
            throw runtime_error("Invalid trade offer.");
        }
        market->addTrade(trade);
        cout << proposer << " proposed a trade to " << target << ".\n";
    }
    catch (const runtime_error& e) {
        cerr << "Error proposing trade: " << e.what() << "\n";
    }
}

void World::proposeSmuggle(const string& proposer, const string& target, const string& res, int qtyO, const string& reqRes, int qtyR, int risk) {
    try {
        SmuggleOffer* smuggle = new SmuggleOffer(proposer, target, res, qtyO, reqRes, qtyR, risk);
        if (!smuggle->isValid()) {
            delete smuggle;
            throw runtime_error("Invalid smuggle offer.");
        }
        market->addSmuggle(smuggle);
        cout << proposer << " proposed a smuggling deal to " << target << ".\n";
    }
    catch (const runtime_error& e) {
        cerr << "Error proposing smuggle: " << e.what() << "\n";
    }
}

void World::declareWar(const string& attacker, const string& defender) {
    try {
        Kingdom* attackerKingdom = findKingdom(attacker);
        Kingdom* defenderKingdom = findKingdom(defender);
        if (!attackerKingdom || !defenderKingdom) {
            throw runtime_error("Invalid kingdoms for war declaration.");
        }
        attackerKingdom->attack(*defenderKingdom);
        cout << attacker << " declared war on " << defender << "!\n";
    }
    catch (const runtime_error& e) {
        cerr << "Error declaring war: " << e.what() << "\n";
    }
}

Kingdom* World::findKingdom(const string& name) {
    for (int i = 0; i < kingdomCount; i++) {
        if (kingdoms[i] && kingdoms[i]->getName() == name) {
            return kingdoms[i];
        }
    }
    return nullptr;
}

bool World::areAllied(const string& k1, const string& k2) const {
    for (int i = 0; i < allianceCount; i++) {
        if (alliances[i] && alliances[i]->isActive() &&
            ((alliances[i]->getKingdom1() == k1 && alliances[i]->getKingdom2() == k2) ||
                (alliances[i]->getKingdom1() == k2 && alliances[i]->getKingdom2() == k1))) {
            return true;
        }
    }
    return false;
}

bool World::hasEmbargo(const string& k1, const string& k2) const {
    return false;
}

Kingdom** World::getKingdoms() const {
    return const_cast<Kingdom**>(&kingdoms[0]);
}