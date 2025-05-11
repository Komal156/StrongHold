#include "stronghold.h"
#include <iostream>
using namespace std;


// Resource Implementation
Resource::Resource(const string& resName, int initialQuantity, int prodRate) : name(resName), quantity(initialQuantity), productionRate(prodRate) {}

void Resource::produce(int efficiency) {
    if (efficiency < 0 || efficiency > 100)
    {
        cout << "Invalid efficiency. Please enter a value between 0 and 100.\n";
        return;
    }
    quantity += productionRate * efficiency / 100;
}

bool Resource::consume(int amount)
{
    if (amount < 0)
    {
        cout << "Invalid consumption amount. Please enter a positive number.\n";
        return false;
    }
    if (quantity < amount)
    {
        cout << "Not enough resources to consume.\n";
        return false;
    }
    quantity -= amount;
    return true;
}

int Resource::calculateDepletion() const {
    return productionRate / 10; // Example calculation
}

// Food Implementation
Food::Food(int initialQuantity, int farmCount)
    : Resource("Food", initialQuantity, farmCount * 10), farms(farmCount) {
}

void Food::produce(int efficiency) {
    Resource::produce(efficiency);
    quantity += farms * efficiency / 10;
}

bool Food::consume(int amount) {
    return Resource::consume(amount);
}

// Wood Implementation
Wood::Wood(int initialQuantity, int forestCount)
    : Resource("Wood", initialQuantity, forestCount * 5), forests(forestCount) {
}

void Wood::produce(int efficiency) {
    Resource::produce(efficiency);
}

// Iron Implementation
Iron::Iron(int initialQuantity, int mineCount)
    : Resource("Iron", initialQuantity, mineCount * 3), mines(mineCount) {
}

void Iron::produce(int efficiency) {
    Resource::produce(efficiency);
}
//a