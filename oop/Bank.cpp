#include "stronghold.h"
#include <iostream>
using namespace std;


Bank::Bank(int initialReserves) : goldReserves(initialReserves), loanInterestRate(5), corruptionCases(0), loanCount(0), year(0) {}

Loan* Bank::issueLoan(int amount, int termYears, const string& lender)
{
    if (loanCount >= MAX_RESOURCES) {
        cout << "Cannot issue more loans. Max limit reached.\n";
        return nullptr;
    }
    int dueDate = year + termYears;
    Loan* newLoan = new Loan(amount, loanInterestRate, dueDate, lender);
    activeLoans[loanCount++] = newLoan;
    goldReserves -= amount;
    return newLoan;
}

void Bank::collectDebt() {
    for (int i = 0; i < loanCount; i++) {
        if (activeLoans[i]->isOverdue(2025))
        {
            cout << "Loan overdue. Collecting debt from " << activeLoans[i]->getAmount() << "\n";
            goldReserves += activeLoans[i]->getAmount();
            delete activeLoans[i];
            activeLoans[i] = nullptr;
        }
    }
}

void Bank::audit(int corruptionThreshold)
{
    if (corruptionCases > corruptionThreshold)
    {
        cout << "Bank audit triggered due to high corruption.\n";
    }
}

bool Bank::detectFraud(int auditSeverity)
{
    return corruptionCases > auditSeverity;
}

//a
