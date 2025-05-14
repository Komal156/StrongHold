#include "stronghold.h"
#include <iostream>
using namespace std;

Map::Map() : kingdomCount(0) {
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            grid[i][j] = 0;
        }
    }
    for (int i = 0; i < MAX_KINGDOMS; i++) {
        kingdomNames[i] = "";
    }
}

void Map::assignKingdom(const string& name, int x, int y, int id) {
    try {
        if (x < 0 || x >= MAP_SIZE || y < 0 || y >= MAP_SIZE) {
            throw runtime_error("Invalid map coordinates.");
        }
        if (grid[x][y] != 0) {
            throw runtime_error("Map position already occupied.");
        }
        if (id < 1 || id > MAX_KINGDOMS) {
            throw runtime_error("Invalid kingdom ID.");
        }
        grid[x][y] = id;
        kingdomNames[id - 1] = name;
        kingdomCount++;
    }
    catch (const runtime_error& e) {
        cerr << "Error assigning kingdom: " << e.what() << "\n";
        throw;
    }
}

void Map::display() const {
    cout << "Kingdom Locations (5x5):\n";
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (grid[i][j] == 0) {
                cout << "* ";
            }
            else {
                cout << grid[i][j] << " ";
            }
        }
        cout << "\n";
    }
    cout << "\nKingdoms:\n";
    for (int i = 0; i < kingdomCount; i++) {
        if (!kingdomNames[i].empty()) {
            cout << i + 1 << ": " << kingdomNames[i] << "\n";
        }
    }
}

void Map::clear() {
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            grid[i][j] = 0;
        }
    }
    for (int i = 0; i < MAX_KINGDOMS; i++) {
        kingdomNames[i] = "";
    }
    kingdomCount = 0;
}

bool Map::isOccupied(int x, int y) const {
    if (x < 0 || x >= MAP_SIZE || y < 0 || y >= MAP_SIZE) {
        return true;
    }
    return grid[x][y] != 0;
}
//updated the code