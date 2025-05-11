#include "stronghold.h"
using namespace std;

Loan::Loan(int amt, int rate, int due, const string& lndr) : amount(amt), interestRate(rate), dueDate(due), lender(lndr), isPaid(false) {}

int Loan::calculateInterest() const
{
    return amount * interestRate / 100;
}
bool Loan::isOverdue(int currentYear) const
{
    return !isPaid && currentYear > dueDate;
}
void Loan::makePayment(int payment)
{
    amount -= payment;
    if (amount <= 0)
    {
        isPaid = true;
    }
}