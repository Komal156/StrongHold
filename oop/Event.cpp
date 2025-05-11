#include "stronghold.h"
#include <iostream>
using namespace std;


Event::Event(const string& evtName, int sev, int dur) : name(evtName), severity(sev), duration(dur), currentDuration(0) {}

void Event::trigger(Kingdom& kingdom) {
    cout << "Event " << name << " triggered with severity " << severity << ".\n";
}
void Event::resolve(Kingdom& kingdom) {
    cout << "Event " << name << " resolved.\n";
    currentDuration = duration;
}

int Event::getImpact() const {
    return severity * 10; // Example impact calculation
}

// Famine Implementation
Famine::Famine(int severity)
    : Event("Famine", severity, 3), foodLossRate(severity * 10) {
}

void Famine::trigger(Kingdom& kingdom) {
    Event::trigger(kingdom);
    kingdom.getEconomy()->getFood()->consume(foodLossRate);
    kingdom.adjustStability(-severity);
}

void Famine::resolve(Kingdom& kingdom) {
    Event::resolve(kingdom);
}

int Famine::getImpact() const {
    return foodLossRate;
}

// Plague Implementation
Plague::Plague(int severity)
    : Event("Plague", severity, 2), mortalityRate(severity * 5) {
}

void Plague::trigger(Kingdom& kingdom) {
    Event::trigger(kingdom);
    kingdom.getPopulation()->migrate(-mortalityRate);
    kingdom.adjustStability(-severity);
}

void Plague::resolve(Kingdom& kingdom) {
    Event::resolve(kingdom);
}

int Plague::getImpact() const {
    return mortalityRate;
}

// Invasion Implementation
Invasion::Invasion(int severity, const string& invadingEnemy)
    : Event("Invasion", severity, 5), enemy(invadingEnemy), troopCount(severity * 100)
{
}

void Invasion::trigger(Kingdom& kingdom) {
    Event::trigger(kingdom);
    kingdom.getMilitary()->recruit(-troopCount / 10);
    kingdom.adjustStability(-severity);
}

void Invasion::resolve(Kingdom& kingdom)
{
    Event::resolve(kingdom);
}

int Invasion::getImpact() const
{
    return troopCount;
}
//a