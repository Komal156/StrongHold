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
        grid[x][y] = id;
        kingdomNames[id - 1] = name;
        kingdomCount++;
    }
    catch (const runtime_error& e) {
        cerr << "Error assigning kingdom: " << e.what() << "\n";
    }
}

void Map::moveKingdom(int kingdomId, int newX, int newY) {
    try {
        if (newX < 0 || newX >= MAP_SIZE || newY < 0 || newY >= MAP_SIZE) {
            throw runtime_error("Invalid map coordinates.");
        }
        int oldX = -1, oldY = -1;
        for (int i = 0; i < MAP_SIZE; i++) {
            for (int j = 0; j < MAP_SIZE; j++) {
                if (grid[i][j] == kingdomId) {
                    oldX = i;
                    oldY = j;
                    break;
                }
            }
        }
        if (oldX == -1 || oldY == -1) {
            throw runtime_error("Kingdom not found on map.");
        }
        if (grid[newX][newY] != 0) {
            throw runtime_error("Destination position occupied.");
        }
        grid[oldX][oldY] = 0;
        grid[newX][newY] = kingdomId;
    }
    catch (const runtime_error& e) {
        cerr << "Error moving kingdom: " << e.what() << "\n";
    }
}

void Map::display() const {
    cout << "Map (5x5):\n";
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (grid[i][j] == 0) {
                cout << ". ";
            }
            else {
                cout << grid[i][j] << " ";
            }
        }
        cout << "\n";
    }
    cout << "Kingdoms:\n";
    for (int i = 0; i < kingdomCount; i++) {
        if (!kingdomNames[i].empty()) {
            cout << i + 1 << ": " << kingdomNames[i] << "\n";
        }
    }
}

int Map::getDistance(const string& k1, const string& k2) const {
    int x1 = -1, y1 = -1, x2 = -1, y2 = -1;
    int id1 = -1, id2 = -1;
    for (int i = 0; i < kingdomCount; i++) {
        if (kingdomNames[i] == k1) id1 = i + 1;
        if (kingdomNames[i] == k2) id2 = i + 1;
    }
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            if (grid[i][j] == id1) { x1 = i; y1 = j; }
            if (grid[i][j] == id2) { x2 = i; y2 = j; }
        }
    }
    if (x1 == -1 || x2 == -1) return 10; // Default large distance
    return abs(x1 - x2) + abs(y1 - y2);
}

bool Map::isControlled(int x, int y, int kingdomId) const {
    return grid[x][y] == kingdomId;
}
