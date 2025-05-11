#include "stronghold.h"
#include <iostream>
using namespace std;

ArmyUnit::ArmyUnit(int soldiers, int mrle, int training, int equipment) : soldierCount(soldiers), morale(mrle), trainingLevel(training), equipmentQuality(equipment) {}

void ArmyUnit::recruit(int amount) {
    if (amount < 0) {
        cout << "Invalid recruitment amount. Please enter a positive number.\n";
        return;
    }
    soldierCount += amount;
}


void ArmyUnit::train(int intensity) {
    if (intensity < 0 || intensity > 100) {
        cout << "Invalid training intensity. Please enter a value between 0 and 100.\n";
        return;
    }
    trainingLevel += intensity;
    if (trainingLevel > 100) trainingLevel = 100; // Cap training level at 100
}

int ArmyUnit::calculateStrength() const {
    return soldierCount * (morale + trainingLevel + equipmentQuality) / 3;
}

void ArmyUnit::paySalaries(int amount) {
    if (amount < 0) {
        cout << "Invalid salary amount. Please enter a positive number.\n";
        return;
    }
    morale += amount / 10; // Increase morale based on salary
    if (morale > 100) morale = 100; // Cap morale at 100
}

void ArmyUnit::updateMorale(int modifier) {
    morale += modifier;
    if (morale > 100) morale = 100; // Cap morale at 100
    if (morale < 0) morale = 0; // Ensure morale doesn't go negative
}

// Infantry Implementation
Infantry::Infantry(int soldiers, int mrle, int training, int equipment, int armr, int wpns)
    : ArmyUnit(soldiers, mrle, training, equipment), armor(armr), weapons(wpns) {
}

int Infantry::calculateStrength() const {
    return ArmyUnit::calculateStrength() + armor + weapons;
}

void Infantry::train(int intensity) {
    ArmyUnit::train(intensity);
    armor += intensity / 10; // Improve armor slightly with training
    if (armor > 100) armor = 100; // Cap armor at 100
}

// Cavalry Implementation
Cavalry::Cavalry(int soldiers, int mrle, int training, int equipment, int hrs)
    : ArmyUnit(soldiers, mrle, training, equipment), horses(hrs) {
}

int Cavalry::calculateStrength() const {
    return ArmyUnit::calculateStrength() + horses * 2; // Horses add more strength
}

void Cavalry::recruit(int amount) {
    ArmyUnit::recruit(amount);
    horses += amount / 10; // Recruit horses proportionally
}
//army