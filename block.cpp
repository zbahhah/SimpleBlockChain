#include <iostream>
#include "block.h"

namespace SBC
{

Block::Block(qint64 pTimestamp, const std::vector<Transaction*> &pTransactions,  QByteArray pPreviousHash) :
    timestamp(pTimestamp),
    transactions(pTransactions),
    previousHash(pPreviousHash),
    hash(calculateHash()),
    nonce(0)
{
}

Block::Block(qint64 pTimestamp, const std::vector<Transaction*> &pTransactions,  QByteArray pPreviousHash, QByteArray pHash, uint pNonce) :
    timestamp(pTimestamp),
    transactions(pTransactions),
    previousHash(pPreviousHash),
    hash(pHash),
    nonce(pNonce)
{
}


Block::Block(QObject *parent) :
    QObject(parent)
{
}


qint64 Block::getGenesisBlockTimestamp()
{
    return QDateTime::fromString(QString("2018-01-13T02:17:00"), Qt::ISODate).toSecsSinceEpoch();
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

void Block::addTransaction(Transaction *value)
{
    transactions.push_back(value);
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

    bool mutated = false;
    dataStream << previousHash
               << timestamp
               << BlockMerkleRoot(&mutated)
               << nonce;
    crypto.addData(inputByteArray);

    return crypto.result();
}

void Block::mineBlock(int difficulty)
{
    QByteArray zeros ;
    zeros.append(difficulty, 0);

    while (hash.left(difficulty) != zeros) {
        nonce++;
        hash = calculateHash();
    }

    std::cout << "\nBLOCK MINED NONCE: " << nonce << std::endl;
    std::cout << "Previous Block HASH: " << getPreviousHash().toHex().toStdString() << std::endl ;
    std::cout << "BLOCK MINED HASH: " << getHash().toHex().toStdString() << std::endl << std::endl;
}

 QByteArray Block::getGenesisHash()
{
    QCryptographicHash crypto(QCryptographicHash::Sha3_256);
    QByteArray inputByteArray;
    QDataStream dataStream(&inputByteArray, QIODevice::WriteOnly);
    dataStream << "noHash" << 1010101010 << "from:To:Amount:10000" << 0;
    crypto.addData(inputByteArray);

    return crypto.result();
 }

 /*     WARNING! If you're reading this because you're learning about crypto
        and/or designing a new system that will use merkle trees, keep in mind
        that the following merkle tree algorithm has a serious flaw related to
        duplicate txids, resulting in a vulnerability (CVE-2012-2459).
        The reason is that if the number of hashes in the list at a given time
        is odd, the last one is duplicated before computing the next level (which
        is unusual in Merkle trees). This results in certain sequences of
        transactions leading to the same merkle root. For example, these two
        trees:
                     A               A
                   /  \            /   \
                 B     C         B       C
                / \    |        / \     / \
               D   E   F       D   E   F   F
              / \ / \ / \     / \ / \ / \ / \
              1 2 3 4 5 6     1 2 3 4 5 6 5 6
        for transaction lists [1,2,3,4,5,6] and [1,2,3,4,5,6,5,6] (where 5 and
        6 are repeated) result in the same root hash A (because the hash of both
        of (F) and (F,F) is C).
        The vulnerability results from being able to send a block with such a
        transaction list, with the same merkle root, and the same block hash as
        the original without duplication, resulting in failed validation. If the
        receiving node proceeds to mark that block as permanently invalid
        however, it will fail to accept further unmodified (and thus potentially
        valid) versions of the same block. We defend against this by detecting
        the case where we would hash two identical hashes at the end of the list
        together, and treating that identically to the block having an invalid
        merkle root. Assuming no double-SHA256 collisions, this will detect all
        known ways of changing the transactions without affecting the merkle
        root.
 */

 /* This implements a constant-space merkle root/path calculator, limited to 2^32 leaves. */
 void Block::MerkleComputation(const std::vector<QByteArray> &leaves, QByteArray* proot, bool* pmutated)
 {
     bool mutated = false;
     // count is the number of leaves processed so far.
     uint32_t count = 0;
     // inner is an array of eagerly computed subtree hashes, indexed by tree
     // level (0 being the leaves).
     // For example, when count is 25 (11001 in binary), inner[4] is the hash of
     // the first 16 leaves, inner[3] of the next 8 leaves, and inner[0] equal to
     // the last leaf. The other inner entries are undefined.
     QByteArray inner[32];

     QCryptographicHash crypto(QCryptographicHash::Sha256);

     // Which position in inner is a hash that depends on the matching leaf.
     // First process all leaves into 'inner' values.
     while (count < leaves.size())
     {
         QByteArray h = leaves[count];
         count++;
         int level;

         // For each of the lower bits in count that are 0, do 1 step. Each
         // corresponds to an inner value that existed before processing the
         // current leaf, and each needs a hash to combine it.
         crypto.reset();
         for (level = 0; !(count & (((uint32_t)1) << level)); level++) {
             mutated |= (inner[level] == h); // mutated = mutated || (inner[level] == h)
             QByteArray inputByteArray;
             QDataStream dataStream(&inputByteArray, QIODevice::WriteOnly);
             dataStream << inner[level] << h;
             crypto.addData(inputByteArray);
             h = crypto.result();
         }

         // Store the resulting hash at inner position level.
         inner[level] = h;
     }

     // Do a final 'sweep' over the rightmost branch of the tree to process
     // odd levels, and reduce everything to a single top value.
     // Level is the level (counted from the bottom) up to which we've sweeped.
     int level = 0;

     // As long as bit number level in count is zero, skip it. It means there
     // is nothing left at this level.
     while (!(count & (((uint32_t)1) << level)))
     {
         level++;
     }

     QByteArray h = inner[level];

     crypto.reset();
     while (count != (((uint32_t)1) << level))
     {
         // If we reach this point, h is an inner value that is not the top.
         // We combine it with itself (Bitcoin's special rule for odd levels in
         // the tree) to produce a higher level one.
         QByteArray inputByteArray;
         QDataStream dataStream(&inputByteArray, QIODevice::WriteOnly);
         dataStream << h << h;
         crypto.addData(inputByteArray);
         h = crypto.result();
         // Increment count to the value it would have if two entries at this
         // level had existed.
         count += (((uint32_t)1) << level);
         level++;
         // And propagate the result upwards accordingly.
         while (!(count & (((uint32_t)1) << level)))
         {
             QCryptographicHash crypto(QCryptographicHash::Sha256);
             QByteArray inputByteArray;
             QDataStream dataStream(&inputByteArray, QIODevice::WriteOnly);
             dataStream << inner[level] << h;
             crypto.addData(inputByteArray);
             h = crypto.result();
             level++;
         }
     }
     // Return result.
     if (pmutated) *pmutated = mutated;
     if (proot) *proot = h;
 }

 QByteArray Block::ComputeMerkleRoot(const std::vector<QByteArray> &leaves, bool* mutated) {
     QByteArray hash;
     MerkleComputation(leaves, &hash, mutated);
     return hash;
 }

 QByteArray Block::BlockMerkleRoot(bool* mutated)
 {
     std::vector<QByteArray> leaves;
     leaves.resize(transactions.size());
     for (size_t s = 0; s < transactions.size(); s++) {
         leaves[s] = transactions[s]->getTxHash();
     }
     return ComputeMerkleRoot(leaves, mutated);
 }

}
