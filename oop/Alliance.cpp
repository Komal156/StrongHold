#include "stronghold.h"
#include <iostream>
using namespace std;

Alliance::Alliance(const string& k1, const string& k2, const string& typ, int yr, int dur)
    : kingdom1(k1), kingdom2(k2), type(typ), active(true), startYear(yr), duration(dur) {
}

void Alliance::breakAlliance() {
    active = false;
    cout << type << " between " << kingdom1 << " and " << kingdom2 << " has been broken.\n";
}