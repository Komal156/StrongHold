#include "stronghold.h"
#include <iostream>
using namespace std;

Message::Message(const string& sndr, const string& rcpt, const string& cntnt, int yr)
    : sender(sndr), recipient(rcpt), content(cntnt), year(yr) {
}

void Message::display() const {
    cout << "Year " << year << ": From " << sender << " to " << recipient << ": " << content << "\n";
}