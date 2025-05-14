#include "stronghold.h"
#include <iostream>
using namespace std;

Alliance::Alliance(const string& k1, const string& k2, const string& typ, int yr, int dur)
    : kingdom1(k1), kingdom2(k2), type(typ), active(true), startYear(yr), duration(dur)
{
    if (k1.empty() || k2.empty() || k1 == k2) {
        throw runtime_error("Invalid kingdom names for alliance.");
    }
    if (dur < 0) {
        throw runtime_error("Duration cannot be negative.");
    }
}

Alliance::Alliance()
    : kingdom1(""), kingdom2(""), type(""), active(false), startYear(0), duration(0)
{
}

void Alliance::breakAlliance()
{
    active = false;
    cout << type << " between " << kingdom1 << " and " << kingdom2 << " has been broken.\n";
}

bool Alliance::isExpired(int currentYear) const {
    return currentYear > startYear + duration;
}
//updated the alliance