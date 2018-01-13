#include "transaction.h"

namespace SBC
{
Transaction::Transaction(QObject *parent) :
    QObject(parent)
{
}

Transaction::Transaction(Transaction *data):
    from(data->from),
    to(data->to),
    amount(data->amount)
{

}

QString Transaction::getFrom() const
{
    return from;
}

void Transaction::setFrom(const QString &value)
{
    from = value;
}

QString Transaction::getTo() const
{
    return to;
}

void Transaction::setTo(const QString &value)
{
    to = value;
}

double Transaction::getAmount() const
{
    return amount;
}

void Transaction::setAmount(double value)
{
    amount = value;
}
}
