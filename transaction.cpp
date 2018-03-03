#include "transaction.h"

namespace SBC
{
Transaction::Transaction(QString pFrom, QString pTo, double pAmount):
    from(pFrom),
    to(pTo),
    amount(pAmount)
{    
    hash = calculateHash();
}

Transaction::Transaction(QString pFrom, QString pTo, double pAmount, QByteArray pHash):
    from(pFrom),
    to(pTo),
    amount(pAmount),
    hash(pHash)
{

}

Transaction::Transaction(QObject *parent) :
    QObject(parent)
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

QByteArray Transaction::calculateHash()
{
    QCryptographicHash crypto(QCryptographicHash::Sha256);
    QByteArray inputByteArray;
    QDataStream dataStream(&inputByteArray, QIODevice::WriteOnly);

    dataStream << from
               << to
               << amount;

    crypto.addData(inputByteArray);


    return crypto.result();
}

QByteArray Transaction::getTxHash() const
{
    return hash;
}

void Transaction::setTxHash(const QByteArray &value)
{
    hash = value;
}

}
