#pragma once
#include <QObject>
#include "block.h"

namespace SBC{

class Block;

class BlockChain : public QObject
{
    Q_OBJECT
public:
    Block* createGenesisBlock();
    Block* getLatestBlock();
    void addBlock(Block* newBlock);
    bool isChainValid();

    BlockChain(int pDifficulty);
    explicit BlockChain(QObject *parent=0);

    int getDifficulty() const;
    void setDifficulty(int value);

    QVector<Block *> getChain() const;
    void setChain(const QVector<Block *> &value);

private:
    int difficulty;
    QVector<Block*> chain;
    explicit BlockChain(const BlockChain& rhs) = delete;
    BlockChain& operator= (const BlockChain& rhs) = delete;
};
}


