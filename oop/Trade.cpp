#include "stronghold.h"
#include <iostream>
using namespace std;

TradeOffer::TradeOffer(const string& prop, const string& tgt, const string& res, int qtyO, const string& reqRes, int qtyR)
    : proposer(prop), target(tgt), resourceType(res), quantityOffered(qtyO), requestedResource(reqRes), quantityRequested(qtyR), accepted(false) {
}

bool TradeOffer::accept(Kingdom& proposerKingdom, Kingdom& targetKingdom) {
    Resource* offeredResource = nullptr;
    Resource* requestedResourcePtr = nullptr;

    // Determine the offered resource
    if (resourceType == "Food") offeredResource = proposerKingdom.getEconomy()->getFood();
    else if (resourceType == "Wood") offeredResource = proposerKingdom.getEconomy()->getWood();
    else if (resourceType == "Iron") offeredResource = proposerKingdom.getEconomy()->getIron();

    // Determine the requested resource
    if (requestedResource == "Food") requestedResourcePtr = targetKingdom.getEconomy()->getFood();
    else if (requestedResource == "Wood") requestedResourcePtr = targetKingdom.getEconomy()->getWood();
    else if (requestedResource == "Iron") requestedResourcePtr = targetKingdom.getEconomy()->getIron();

    // Check if both kingdoms have enough resources
    if (offeredResource && requestedResourcePtr &&
        offeredResource->getQuantity() >= quantityOffered &&
        requestedResourcePtr->getQuantity() >= quantityRequested) {
        offeredResource->consume(quantityOffered);
        requestedResourcePtr->consume(quantityRequested);

        // Transfer resources
        if (resourceType == "Food") targetKingdom.getEconomy()->getFood()->produce(quantityOffered);
        else if (resourceType == "Wood") targetKingdom.getEconomy()->getWood()->produce(quantityOffered);
        else if (resourceType == "Iron") targetKingdom.getEconomy()->getIron()->produce(quantityOffered);

        if (requestedResource == "Food") proposerKingdom.getEconomy()->getFood()->produce(quantityRequested);
        else if (requestedResource == "Wood") proposerKingdom.getEconomy()->getWood()->produce(quantityRequested);
        else if (requestedResource == "Iron") proposerKingdom.getEconomy()->getIron()->produce(quantityRequested);

        accepted = true;
        return true;
    }
    return false;
}

SmuggleOffer::SmuggleOffer(const string& prop, const string& tgt, const string& res, int qtyO, const string& reqRes, int qtyR, int risk)
    : TradeOffer(prop, tgt, res, qtyO, reqRes, qtyR), riskLevel(risk) {
}

bool SmuggleOffer::accept(Kingdom& proposerKingdom, Kingdom& targetKingdom) {
    // Check risk of discovery
    int roll = rand() % 100;
    if (roll < riskLevel) {
        cout << "Smuggle discovered! Stability penalty applied.\n";
        proposerKingdom.adjustStability(-20);
        targetKingdom.adjustStability(-20);
        return false;
    }
    return TradeOffer::accept(proposerKingdom, targetKingdom);
}