#include <iostream>
#include "block.h"
using namespace std;

namespace SBC
{
Block::Block(QObject *parent) :
    QObject(parent)
{
}

Block::Block(int pIndex, qint64 pTimestamp, Transaction* pData,  QByteArray pPreviousHash) :
    index(pIndex),
    timestamp(pTimestamp),
    data(pData),
    previousHash(pPreviousHash),
    hash(calculateHash()),
    nonce(0)
{
}

qint64 Block::getGenesisBlockTimestamp()
{
    return QDateTime::fromString(QString("2018-01-13T02:17:00"), Qt::ISODate).toSecsSinceEpoch();
}

int Block::getIndex() const
{
    return index;
}

void Block::setIndex(int value)
{
    index = value;
}

qint64 Block::getTimestamp() const
{
    return timestamp;
}

void Block::setTimestamp(const qint64 &value)
{
    timestamp = value;
}

uint Block::getNonce() const
{
    return nonce;
}

void Block::setNonce(const uint &value)
{
    nonce = value;
}

Transaction *Block::getData() const
{
    return data;
}

void Block::setData(Transaction *value)
{
    data = value;
}

QByteArray Block::getPreviousHash() const
{
    return previousHash;
}

void Block::setPreviousHash(const QByteArray &value)
{
    previousHash = value;
}

QByteArray Block::getHash() const
{
    return hash;
}

void Block::setHash(const QByteArray &value)
{
    hash = value;
}

QByteArray Block::calculateHash()
{
    QCryptographicHash crypto(QCryptographicHash::Sha256);
    QByteArray inputByteArray;
    QDataStream dataStream(&inputByteArray, QIODevice::WriteOnly);
    dataStream << this->index << this->previousHash << this->timestamp
               << this->data->getFrom()
               << this->data->getTo()
               << this->data->getAmount()
               << this->nonce;
    crypto.addData(inputByteArray);


    return crypto.result();
}

void Block::mineBlock(int difficulty)
{
    QByteArray zeros ;
    zeros.append(difficulty, '0');

    while (this->hash.left(difficulty) != zeros) {
        this->nonce++;
        this->hash = calculateHash();
    }

    cout << "\nBLOCK MINED NONCE: " << this->nonce << endl;
    cout << "BLOCK MINED HASH: " << this->getHash().toHex().toStdString() << endl << endl;
    //qDebug() << "BLOCK MINED: " + this->hash;

}

 QByteArray Block::getGenesisHash()
{
    QCryptographicHash crypto(QCryptographicHash::Sha3_256);
    QByteArray inputByteArray;
    QDataStream dataStream(&inputByteArray, QIODevice::WriteOnly);
    dataStream << 0 << "noHash" << 1010101010 << "from:To:Amount:10000" << 0;
    crypto.addData(inputByteArray);


    return crypto.result();
}

}
