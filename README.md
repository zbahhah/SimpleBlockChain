# SimpleBlockChain
a simple implementation of blockchain concept in Qt framework

add new features:

  	1- add merkle tree implementation

  	2- block can contain more transactions

  	3- reward for mining implementation

  	3- save blockchain data as json-format to disk

  	4- save the whole chain to disk as binary (blockchain objects serialization), 
    and read it again from disk, then re-calculate the hashs (for blocks & transactions) 
    as a proof of validation merkle tree


![alt text](https://github.com/zbahhah/SimpleBlockChain/blob/master/simpleBcJsonBinaryFiles.png)


console output

![alt text](https://github.com/zbahhah/SimpleBlockChain/blob/master/simpleBcOutput.png)


Json file
![alt text](https://github.com/zbahhah/SimpleBlockChain/blob/master/simpleBcJson.png)

binary file that saves the whole blockchain data

![alt text](https://github.com/zbahhah/SimpleBlockChain/blob/master/SimpleBcBinary.png)



Many thanks to SavjeeCoin: https://github.com/SavjeeTutorials/SavjeeCoin

my reources are from this youtube:

Creating a blockchain with Javascript (Blockchain, part 1)

https://www.youtube.com/watch?v=zVqczFZr124


Implementing Proof-of-Work in Javascript (Blockchain, part 2)

https://www.youtube.com/watch?v=HneatE69814


Miner rewards & transactions - Blockchain in Javascript (part 3)

https://www.youtube.com/watch?v=fRV6cGXVQ4I&feature=youtu.be


the Javascript source : https://github.com/SavjeeTutorials/SavjeeCoin


the original implementation in Javascript, 
so I just tried to implement same concepts in C++ by using Qt framework.
