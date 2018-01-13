#pragma once
#include <QObject>
#include <QtCore>
#include <QCryptographicHash>
#include <QDebug>
#include "transaction.h"

namespace SBC
{

class Transaction;

class Block : public QObject
{
    Q_OBJECT
public:
    explicit Block(QObject *parent=0);

    int getIndex() const;
    void setIndex(int value);

    qint64 getTimestamp() const;
    void setTimestamp(const qint64 &value);

    uint getNonce() const;
    void setNonce(const uint &value);

    Transaction *getData() const;
    void setData(Transaction *value);

    QByteArray calculateHash();

    QByteArray getPreviousHash() const;
    void setPreviousHash(const QByteArray &value);

    QByteArray getHash() const;
    void setHash(const QByteArray &value);

    void mineBlock(int difficulty);

    Block(int pIndex, qint64 pTimestamp, Transaction* pData, QByteArray pPreviousHash = nullptr);

    static qint64 getGenesisBlockTimestamp();
    static QByteArray getGenesisHash();
private:
    int index;
    QByteArray previousHash ;
    qint64 timestamp ;
    Transaction* data;
    QByteArray hash;
    uint nonce;

    explicit Block(const Block& rhs) = delete;
    Block& operator= (const Block& rhs) = delete;
};

}


