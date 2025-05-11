#include "stronghold.h"
#include <iostream>
using namespace std;


Market::Market() : tradeCount(0), smuggleCount(0) {
    for (int i = 0; i < MAX_TRADES; i++) {
        trades[i] = nullptr;
        smuggles[i] = nullptr;
    }
}

Market::~Market() {
    for (int i = 0; i < tradeCount; i++) {
        delete trades[i];
    }
    for (int i = 0; i < smuggleCount; i++) {
        delete smuggles[i];
    }
}

void Market::addTrade(TradeOffer* trade) {
    if (tradeCount < MAX_TRADES) {
        trades[tradeCount++] = trade;
    }
    else {
        delete trade;
        throw runtime_error("Market trade limit reached.");
    }
}

void Market::addSmuggle(SmuggleOffer* smuggle) {
    if (smuggleCount < MAX_TRADES) {
        smuggles[smuggleCount++] = smuggle;
    }
    else {
        delete smuggle;
        throw runtime_error("Market smuggle limit reached.");
    }
}

void Market::processTrades(Kingdom** kingdoms, int kingdomCount) {
    int newTradeCount = 0;
    for (int i = 0; i < tradeCount; i++) {
        if (trades[i] && !trades[i]->isAccepted()) {
            Kingdom* prop = nullptr;
            Kingdom* tgt = nullptr;
            for (int j = 0; j < kingdomCount; j++) {
                if (kingdoms[j] && kingdoms[j]->getName() == trades[i]->getProposer()) prop = kingdoms[j];
                if (kingdoms[j] && kingdoms[j]->getName() == trades[i]->getTarget()) tgt = kingdoms[j];
            }
            if (prop && tgt) {
                bool accepted = trades[i]->accept(*prop, *tgt);
                if (accepted) {
                    cout << "Trade accepted: " << trades[i]->getProposer() << " -> " << trades[i]->getTarget() << "\n";
                }
                else {
                    cout << "Trade rejected: " << trades[i]->getProposer() << " -> " << trades[i]->getTarget() << "\n";
                }
                delete trades[i];
                trades[i] = nullptr;
            }
            else {
                trades[newTradeCount] = trades[i];
                trades[i] = nullptr;
                newTradeCount++;
            }
        }
    }
    tradeCount = newTradeCount;

    int newSmuggleCount = 0;
    for (int i = 0; i < smuggleCount; i++) {
        if (smuggles[i] && !smuggles[i]->isAccepted()) {
            Kingdom* prop = nullptr;
            Kingdom* tgt = nullptr;
            for (int j = 0; j < kingdomCount; j++) {
                if (kingdoms[j] && kingdoms[j]->getName() == smuggles[i]->getProposer()) prop = kingdoms[j];
                if (kingdoms[j] && kingdoms[j]->getName() == smuggles[i]->getTarget()) tgt = kingdoms[j];
            }
            if (prop && tgt) {
                bool accepted = smuggles[i]->accept(*prop, *tgt);
                if (accepted) {
                    cout << "Smuggle accepted: " << smuggles[i]->getProposer() << " -> " << smuggles[i]->getTarget() << "\n";
                }
                else {
                    cout << "Smuggle rejected: " << smuggles[i]->getProposer() << " -> " << smuggles[i]->getTarget() << "\n";
                }
                delete smuggles[i];
                smuggles[i] = nullptr;
            }
            else {
                smuggles[newSmuggleCount] = smuggles[i];
                smuggles[i] = nullptr;
                newSmuggleCount++;
            }
        }
    }
    smuggleCount = newSmuggleCount;
}

void Market::displayTrades() const {
    cout << "Active Trades:\n";
    for (int i = 0; i < tradeCount; i++) {
        if (trades[i] && !trades[i]->isAccepted()) {
            cout << trades[i]->getProposer() << " offers " << trades[i]->getQuantityOffered()
                << " " << trades[i]->getResourceType() << " to " << trades[i]->getTarget()
                << " for " << trades[i]->getQuantityRequested() << " " << trades[i]->getRequestedResource() << "\n";
        }
    }
    if (tradeCount == 0) {
        cout << "No active trades.\n";
    }
}

void Market::displaySmuggles() const {
    cout << "Active Smuggling Deals:\n";
    for (int i = 0; i < smuggleCount; i++) {
        if (smuggles[i] && !smuggles[i]->isAccepted()) {
            cout << smuggles[i]->getProposer() << " offers " << smuggles[i]->getQuantityOffered()
                << " " << smuggles[i]->getResourceType() << " to " << smuggles[i]->getTarget()
                << " for " << smuggles[i]->getQuantityRequested() << " " << smuggles[i]->getRequestedResource()
                << " (Risk: " << smuggles[i]->getRiskLevel() << "%)\n";
        }
    }
    if (smuggleCount == 0) {
        cout << "No active smuggling deals.\n";
    }
}