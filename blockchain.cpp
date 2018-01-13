#include "blockchain.h"

namespace SBC
{

BlockChain::BlockChain(QObject *parent) :
    QObject(parent)
{
}

int BlockChain::getDifficulty() const
{
    return difficulty;
}

void BlockChain::setDifficulty(int value)
{
    difficulty = value;
}

QVector<Block *> BlockChain::getChain() const
{
    return chain;
}

void BlockChain::setChain(const QVector<Block *> &value)
{
    chain = value;
}

Block* BlockChain::createGenesisBlock()
{
    Transaction* data = new Transaction() ;
    data->setFrom("xManFrom");
    data->setTo("xManTo");
    data->setAmount(10000.0);
    return new Block(0, Block::getGenesisBlockTimestamp(), data, Block::getGenesisHash());
}

Block *BlockChain::getLatestBlock()
{
    return this->chain[this->chain.length() - 1];
}

void BlockChain::addBlock(Block *newBlock)
{
    newBlock->setPreviousHash(getLatestBlock()->getHash());
    newBlock->mineBlock(this->getDifficulty());
    this->chain.append(newBlock);
}

bool BlockChain::isChainValid()
{
    for (int i = 1; i < this->chain.length(); i++)
    {
        auto currentBlock = this->chain[i];
        auto previousBlock = this->chain[i - 1];

        if (currentBlock->getHash() != currentBlock->calculateHash())
        {
            return false;
        }

        if (currentBlock->getPreviousHash() != previousBlock->getHash())
        {
            return false;
        }
    }

    return true;
}

BlockChain::BlockChain(int pDifficulty):
    difficulty(pDifficulty)

{
    this->chain.append( this->createGenesisBlock());
}

}
