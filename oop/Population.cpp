#include "stronghold.h"
#include <iostream>
using namespace std;

Population::Population(int initialCount)
    : totalCount(initialCount), growthRate(0), health(100), employmentRate(100) {
}

void Population::calculateGrowth(int foodAvailability, int housingQuality) {  //ideal food and husing=100;
    growthRate = (foodAvailability + housingQuality) / 2; //average 
    totalCount += growthRate;
}

void Population::updateHealth(int sanitation, int medicalCare) {
    health = (sanitation + medicalCare) / 2;
    if (health > 100) health = 100; // Ensure health stays within range
    if (health < 0) health = 0;
}

int Population::getRevoltRisk(int happinessAvg) const {
    return 100 - happinessAvg; // Higher happiness reduces revolt risk
}

void Population::migrate(int amount) {
    totalCount += amount;
    if (totalCount < 0) totalCount = 0; // Ensure population doesn't go negative
}
//a