#include <iostream>

#include <QtCore>
#include "blockchain.h"

using namespace std;
using namespace SBC;

void chainToJson(BlockChain* pCoin)
{
    QJsonObject objRoot;//root object
    objRoot["BlockChain"] = QString("%1");//(1)
    QJsonArray blocks;//(2)
    auto chain = pCoin->getChain();
    for(int i = 0 ; i < chain.size(); i++)
    {
        QJsonObject block;

        block["Index"] = chain[i]->getIndex();
        block["PreviousHash"] = QString(chain[i]->getPreviousHash().toHex());
        block["Hash"] = QString(chain[i]->getHash().toHex());
        block["Nonce"] = QString::number(chain[i]->getNonce());
        block["TimeStamp"] = QString::number(chain[i]->getTimestamp());

        QJsonObject transaction;
        transaction["from"] = chain[i]->getData()->getFrom();
        transaction["to"] = chain[i]->getData()->getTo();
        transaction["amount"] = chain[i]->getData()->getAmount();
        block["Transaction"] = transaction; //QString(chain[i]->getData());

        blocks.append(block);
    }
    objRoot["BlockChain"] = blocks;

    cout <<  "\n\n BlockChain:\n " <<
            QJsonDocument(objRoot).toJson(QJsonDocument::Indented).toStdString() << endl;

}

int main(int argc, char *argv[])
{
    cout << "Hello World!" << endl;
    cout << "Please enter difficulty number between 1 to 3" << endl;

    uint difficulty = 1;
    cin>> difficulty;
    if(difficulty > 3 || difficulty < 1)
    {
        difficulty = 2; // default
    }

    auto xCoin = new BlockChain(difficulty);
    cout << "Mining block 1..." << endl;

    Transaction* tx1 = new Transaction() ;
    tx1->setFrom("xFromOne");
    tx1->setTo("xToOne");
    tx1->setAmount(20.0);

    xCoin->addBlock(new Block(1, QDateTime::currentSecsSinceEpoch(), tx1));

    cout << "Mining block 2..." << endl;
    Transaction* tx2 = new Transaction() ;
    tx2->setFrom("Alice");
    tx2->setTo("Bobe");
    tx2->setAmount(500);
    xCoin->addBlock(new Block(2, QDateTime::currentSecsSinceEpoch(), tx2));

    // convert blockchain to json object then print
    chainToJson(xCoin);


    cout << "Blockchain valid? " << xCoin->isChainValid()  << endl;

    cout << "Changing a block..." << endl;

    auto block1 = xCoin->getChain()[2];
    auto block1Data = block1->getData();
    block1Data->setAmount(200000);
    //block1->setHash(block1->calculateHash());

    // convert blockchain to json object then print
    chainToJson(xCoin);

    cout <<  "Blockchain valid? " << xCoin->isChainValid()  << endl;

    return 0;
}
