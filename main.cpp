#include <iostream>
#include <QtCore/QCoreApplication>
#include <QFile>
#include <QDir>
#include "blockchain.h"

using namespace SBC;

void chainToJson(BlockChain* pCoin)
{
    QJsonObject objRoot;//root object

    QJsonArray blocks;//(2)

    for(auto i = 0 ; i < pCoin->chain.size(); i++)
    {
        QJsonObject block;
        block["BlockIndex"] = QString::number(i);
        block["PreviousHash"] = QString(pCoin->chain[i]->getPreviousHash().toHex());
        block["BlockHash"] = QString(pCoin->chain[i]->getHash().toHex());
        block["Nonce"] = QString::number(pCoin->chain[i]->getNonce());
        block["TimeStamp"] = QString::number(pCoin->chain[i]->getTimestamp());

        QJsonArray transactions;
        unsigned int j =0;
        for(const auto& tx : pCoin->chain[i]->transactions)
        {
            QJsonObject transaction;
            transaction["TxIndex"] = QString::number(j);
            transaction["from"] = tx->getFrom();
            transaction["to"] = tx->getTo();
            transaction["amount"] = tx->getAmount();
            transaction["TxHash"] = QString(tx->getTxHash().toHex());
            transactions.append(transaction);
            j++;
        }

        block["Transactions"] = transactions;

        blocks.append(block);
    }
    objRoot["SimpleBlockChain"] = blocks;

    std::cout <<  "\n\nSimpleBlockChain:\n " <<
                  QJsonDocument(objRoot).toJson(QJsonDocument::Indented).toStdString() << std::endl;

    // save json to disk
    auto simpleBlcokChainToJsonFilePath = QDir::currentPath() + "/SimpleBlcokChainToJson.json";
    QFile simpleBlcokChainToJsonfile(simpleBlcokChainToJsonFilePath);
    if (!simpleBlcokChainToJsonfile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    // empty the old file if exist
    simpleBlcokChainToJsonfile.resize(0);

    simpleBlcokChainToJsonfile.write(QJsonDocument(objRoot).toJson(QJsonDocument::Indented));

    std::cout << "\nSimpleBlcokChainToJson path is: " + simpleBlcokChainToJsonFilePath.toStdString() << std::endl;

    simpleBlcokChainToJsonfile.flush();
    simpleBlcokChainToJsonfile.close();

}

void chainToStream(QDataStream& out, BlockChain* pCoin)
{
    out << pCoin->getDifficulty();
    out << pCoin->getMiningReward();

    // the blocks count
    long long blockCount = pCoin->chain.size();
    out << blockCount;

    // out the blocks
    // first out the transactions count
    // then content of block header
    // then loop to each transaction
    for(const auto& bx : pCoin->chain)
    {
        unsigned int txCount = bx->transactions.size();
        out << txCount;
        out << bx->getTimestamp();
        out << bx->getNonce();
        QString endBh = "endBlockHeader";
        out << endBh;

        // tx info
        for(const auto& tx : bx->transactions)
        {
            out << tx->getFrom() ;
            out << tx->getTo();
            out << tx->getAmount();
            QString endTx = "endTx";
            out << endTx;
        }
        QString endBk = "endBlock";
        out << endBk;
    }
}

void chainFromStream(QDataStream& in, BlockChain*& pCoin)
{
    int difficulty;
    int miningReward;

    in >> difficulty;
    in >> miningReward;
    pCoin = new BlockChain(difficulty, miningReward);

    long long blockCount;
    in >> blockCount;

    std::vector<Block*> blocks;

    for(auto i =0; i< blockCount; i++)
    {
        unsigned int txCount ;
        in >> txCount;

        qint64 timestamp;
        uint nonce;
        QString endBh;
        QString endBk;
        in >> timestamp ;
        in >> nonce;
        in >> endBh;

        std::vector<Transaction*> txs;

        for(unsigned int j=0; j< txCount; j++)
        {
            QString from;
            QString to;
            double amount;
            QString endTx;
            in >> from;
            in >> to;
            in >> amount;
            in >> endTx;
            if(i != 0)
                txs.push_back(new Transaction(from, to, amount));
        }
        in >> endBk;

        if(i != 0)
            pCoin->chain.push_back(new Block(timestamp, txs, pCoin->getLatestBlock()->getHash()));

    }

    // mine all blocks (except genesis #0)
    for(auto k =1; k< blockCount; k++)
    {
        pCoin->chain[k]->mineBlock(pCoin->getDifficulty());
    }
}

int initMain()
{
    std::cout << "Hello Simple Blockchain World!" << std::endl;

    BlockChain* savedCoin = new BlockChain(1, 1000) ;
    auto readChainFromFilePath = QDir::currentPath() + "/SimpleBlcokChainBinary.json";
    QFile readChainToFile(readChainFromFilePath);
    if (readChainToFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QDataStream in (&readChainToFile);
        in.setVersion(QDataStream::Qt_5_10);
        chainFromStream(in, savedCoin);
    }

    if(savedCoin->chain.size() > 1)
    {
        std::cout << "there are saved chain in the desk, do you want to retrieve this chain?" << std::endl;
        std::cout << "1:yes, 2:no" << std::endl;
        unsigned int answer;
        std::cin >> answer;
        if(answer == 1)
        {
            Transaction* tx1 = new Transaction("xFromOne",  "xToOne", 20.0);

            Transaction* tx2  = new Transaction("Alice", "Bobe", 500);

            savedCoin->addToPendingTransactions(tx1);
            savedCoin->addToPendingTransactions(tx2);

            std::cout << "\nStarting the miner...\n";
            std::cout << "\nplease enter the address of miner\n";

            std::string xAddress ;
            std::cin >> xAddress;
            QString minerAddress = QString::fromStdString(xAddress);
            savedCoin->minePendingTransations(minerAddress);

            std::cout << "the current balance of miner address:" +
                         minerAddress.toStdString() +
                         " is:\n" << savedCoin->getBalanceOfAddress(minerAddress) << std::endl;

            Transaction* tx3 = new Transaction("xToOne","xFromOne",10.0);
            Transaction* tx4 = new Transaction("Bobe","Alice",150);
            Transaction* tx5 = new Transaction(minerAddress,"Alice",15.50);
            Transaction* tx6 = new Transaction(minerAddress,"Bobe",18.50);
            Transaction* tx7 = new Transaction(minerAddress,"Abdo",25.0);

            savedCoin->addToPendingTransactions(tx3);
            savedCoin->addToPendingTransactions(tx4);
            savedCoin->addToPendingTransactions(tx5);
            savedCoin->addToPendingTransactions(tx6);
            savedCoin->addToPendingTransactions(tx7);

            std::cout << "\nStarting the miner next block...\n";
            std::cout << "\nplease enter the address of next block miner\n";
            std::cin >> xAddress;
            minerAddress = QString::fromStdString(xAddress);

            savedCoin->minePendingTransations(minerAddress);

            std::cout << "the current balance of miner address:" +
                         minerAddress.toStdString() +
                         " is:\n" << savedCoin->getBalanceOfAddress(minerAddress) << std::endl;
            // convert blockchain to json object then print
            chainToJson(savedCoin);

            auto isChainValid = savedCoin->isChainValid() ? "true" : "false" ;
            std::cout << "Blockchain valid? " << isChainValid << std::endl;

            auto saveChainToFilePath = QDir::currentPath() + "/SimpleBlcokChainBinary.json";
            QFile saveChainToFile(saveChainToFilePath);
            if (!saveChainToFile.open(QIODevice::WriteOnly | QIODevice::Text))
                return 0;

            QDataStream out (&saveChainToFile);
            out.setVersion(QDataStream::Qt_5_10);

            // empty the old file if exist
            saveChainToFile.resize(0);

            chainToStream(out, savedCoin);

            saveChainToFile.flush();
            saveChainToFile.close();
            return 0;

        }
    }
    else
    {
        std::cout << "Please enter difficulty number between 1 to 3" << std::endl;

        uint difficulty = 1;
        std::cin >> difficulty;
        if(difficulty > 3 || difficulty < 1)
        {
            difficulty = 2; // default
        }

        std::cout << "Please enter Rewards number between 1 to 10000" << std::endl;
        int rewards = 1;
        std::cin >> rewards;
        if(rewards > 10000 || rewards < 1)
        {
            rewards = 100; // default
        }
        BlockChain* xCoin = new BlockChain(difficulty, rewards);

        std::cout << "Mining block 1..." << std::endl;

        Transaction* tx1 = new Transaction("xFromOne",  "xToOne", 20.0);

        Transaction* tx2  = new Transaction("Alice", "Bobe", 500);

        xCoin->addToPendingTransactions(tx1);
        xCoin->addToPendingTransactions(tx2);

        std::cout << "\nStarting the miner...\n";
        std::cout << "\nplease enter the address of miner\n";

        std::string xAddress ;
        std::cin >> xAddress;
        QString minerAddress = QString::fromStdString(xAddress);
        xCoin->minePendingTransations(minerAddress);

        std::cout << "the current balance of miner address:" +
                     minerAddress.toStdString() +
                     " is:\n" << xCoin->getBalanceOfAddress(minerAddress) << std::endl;

        Transaction* tx3 = new Transaction("xToOne","xFromOne",10.0);
        Transaction* tx4 = new Transaction("Bobe","Alice",150);
        Transaction* tx5 = new Transaction(minerAddress,"Alice",15.50);
        Transaction* tx6 = new Transaction(minerAddress,"Bobe",18.50);
        Transaction* tx7 = new Transaction(minerAddress,"Abdo",25.0);

        xCoin->addToPendingTransactions(tx3);
        xCoin->addToPendingTransactions(tx4);
        xCoin->addToPendingTransactions(tx5);
        xCoin->addToPendingTransactions(tx6);
        xCoin->addToPendingTransactions(tx7);

        std::cout << "\nStarting the miner next block...\n";
        std::cout << "\nplease enter the address of next block miner\n";
        std::cin >> xAddress;
        minerAddress = QString::fromStdString(xAddress);

        xCoin->minePendingTransations(minerAddress);

        std::cout << "the current balance of miner address:" +
                     minerAddress.toStdString() +
                     " is:\n" << xCoin->getBalanceOfAddress(minerAddress) << std::endl;

        // convert blockchain to json object then print
        chainToJson(xCoin);

        auto isChainValid = xCoin->isChainValid() ? "true" : "false" ;
        std::cout << "Blockchain valid? " << isChainValid << std::endl;

        auto saveChainToFilePath = QDir::currentPath() + "/SimpleBlcokChainBinary.json";
        QFile saveChainToFile(saveChainToFilePath);
        if (!saveChainToFile.open(QIODevice::WriteOnly | QIODevice::Text))
            return 0;

        QDataStream out (&saveChainToFile);
        out.setVersion(QDataStream::Qt_5_10);

        // empty the old file if exist
        saveChainToFile.resize(0);

        chainToStream(out, xCoin);

        saveChainToFile.flush();
        saveChainToFile.close();
    }


    return 0;
}

int main(int argc, char *argv[])
{
    try
    {
        std::cout << "argc: " << argc << std::endl << "argv[]:" << argv[0] << std::endl;

        initMain();
    }
    catch (std::exception &e)
    {
        std::cout << "Error :)" <<    e.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "Error <unknown>" << std::endl;
    }

    return 0;
}
