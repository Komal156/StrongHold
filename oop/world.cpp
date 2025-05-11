#include "stronghold.h"
#include <iostream>
using namespace std;

World::World() : kingdomCount(0), messageCount(0), allianceCount(0), currentYear(1) {
    for (int i = 0; i < MAX_KINGDOMS; i++) {
        kingdoms[i] = nullptr;
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
    for (int i = 0; i < allianceCount; i++) {
        delete alliances[i];
        alliances[i] = nullptr;
    }
    delete market;
    market = nullptr;
    delete map;
    map = nullptr;
}

void World::addKingdom(Kingdom* kingdom) {
    if (kingdomCount >= MAX_KINGDOMS) {
        throw runtime_error("Maximum kingdom limit reached.");
    }
    kingdoms[kingdomCount] = kingdom;
    map->assignKingdom(kingdom->getName(), kingdomCount, kingdomCount, kingdomCount + 1);
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

void World::moveKingdom(const string& kingdom, int x, int y) {
    try {
        Kingdom* k = findKingdom(kingdom);
        if (!k) {
            throw runtime_error("Kingdom not found.");
        }
        if (x < 0 || x >= MAP_SIZE || y < 0 || y >= MAP_SIZE) {
            throw runtime_error("Invalid map coordinates.");
        }
        map->moveKingdom(k->getKingdomId(), x, y);
        k->setMapPosition(x, y);
        cout << kingdom << " moved to (" << x << ", " << y << ").\n";
    }
    catch (const runtime_error& e) {
        cerr << "Error moving kingdom: " << e.what() << "\n";
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
    // Placeholder: Assume no embargoes for simplicity
    return false;
}

Kingdom** World::getKingdoms() const {
    return const_cast<Kingdom**>(&kingdoms[0]); // Cast the address of the first element to Kingdom**
}