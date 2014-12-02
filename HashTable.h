//DO NOT CHANGE THIS FILE
//Author: Bo Brinkman
//Date: 2013/07/24
#include "USet.h"

/*
* Note: Just above your template declaration when you use this class, you
* must define method called "hash" that takes a Key as input, and returns
* an unsigned long (which is the hash value)
*
* For example, you might do:
* unsigned long hash(char c){ return 10*((unsigned long)c)%13; }
* HashTable<char,int> mySillyTable;
*
* If you don't define an appropriate hash function, the class won't compile.
*/
template <class Key, class T>
class HashTable : public USet <Key, T> {
private:
	class HashRecord {
	public:
		Key k;
		T x;

		//If the slot in the hash table is totally empty, set this to true.
		bool isNull;

		//If the slot used to have something in it, but doesn't now, set
		// isDel to true, and isNull to false. isNull is only for slots
		// that have never been used
		bool isDel;

		HashRecord() { isNull = true; isDel = false; };
	};

public:
	//See USet.h for documentation of these methods
	virtual unsigned long size();
	virtual void add(Key k, T x);
	virtual void remove(Key k);
	virtual T find(Key k);
	virtual bool keyExists(Key k);

	//Initialize all private member variables.
	HashTable();
	//Delete any dynamically allocated memory.
	virtual ~HashTable();

private:
	//A pointer to the array that holds the hash table data
	HashRecord* backingArray;

	//Whenever numItems + numRemoved >= backingArraySize/2, call
	// grow(). grow() should make a new backing array that is twice the
	// size of the old one, similar to what we did in the ArrayQueue
	// lab.
	//Note: You cannot just loop through the old array and copy it to the
	// new one! Since the backing array size has changed, each item will likely
	// map to a different slot in the array. You may just want to use add()
	// after initializing the new array.
	void grow();

	//This helper method should take a key, and return the index for that
	// item within the hash table. If the item already exists, return the
	// index of the existing item. If the item doesn't exist, return the index
	// where it OUGHT to be. This function can then be used as a helper method in
	// your other methods.
	unsigned long calcIndex(Key k);

	unsigned long numItems; //Number of items in the hash table

	//Note: Ordinarily, these OUGHT to be private. In this case I have
	// made them public for easy of testing.
public:
	unsigned long numRemoved; //Number of slots that have been removed but not re-used. Those that have isDel == true
	unsigned long backingArraySize;
};

//Author: Sam Bowdler
//Date: 2 October 2014
#include <string>

template <class Key, class T>
HashTable<Key,T>::HashTable(){
	backingArray = new HashRecord[hashPrimes[0]];
	backingArraySize = hashPrimes[0];
	numRemoved = 0;
	numItems = 0;
}

template <class Key, class T>
HashTable<Key,T>::~HashTable() {
	delete[] backingArray;
}

template <class Key, class T>
unsigned long HashTable<Key,T>::calcIndex(Key k){
	for (unsigned long i = hash(k); true; i++)
		if ((backingArray[i % backingArraySize].k == k && !(backingArray[i % backingArraySize].isDel)) || backingArray[i % backingArraySize].isNull)
			return (i % backingArraySize);
	return numItems;
}

template <class Key, class T>
void HashTable<Key,T>::add(Key k, T x){
	unsigned long dex = hash(k);

	if (keyExists(k)) {
		for (dex; true; dex++) {
			if (backingArray[dex % backingArraySize].k == k){
				dex = (dex % backingArraySize);
				numRemoved--;
				break;
			}
		}
	} else {
		if ((numItems + numRemoved) >= backingArraySize / 2)
			grow();

		for (dex; true; dex++) {
			if (backingArray[dex % backingArraySize].isNull || backingArray[dex % backingArraySize].k == k || backingArray[dex % backingArraySize].isDel){
				dex = (dex % backingArraySize);
				break;
			}
		}
	}
	
	backingArray[dex].k = k;
	backingArray[dex].x = x;
	backingArray[dex].isNull = false;
	backingArray[dex].isDel = false;
	numItems++;
}

template <class Key, class T>
void HashTable<Key,T>::remove(Key k){
	if (keyExists(k)){
		backingArray[calcIndex(k)].isDel = true;
		numRemoved++;
		numItems--;
	}
}

template <class Key, class T>
T HashTable<Key,T>::find(Key k){
	if (keyExists(k))
		return (backingArray[calcIndex(k)].x);
	else
		throw std::string("ERROR: Key does not exist in hash table.");
}

template <class Key, class T>
bool HashTable<Key,T>::keyExists(Key k){
	unsigned long dex = calcIndex(k);
	if (backingArray[dex].k == k && !backingArray[dex].isDel && !backingArray[dex].isNull)
		return false;
	return true;
}

template <class Key, class T>
unsigned long HashTable<Key,T>::size(){
	return numItems;
}

template <class Key, class T>
void HashTable<Key,T>::grow(){
	HashRecord* ray = backingArray;
	int raySize = backingArraySize;
	
	for (int i = 0; true; i++){
		if (hashPrimes[i] == backingArraySize){
			backingArray = new HashRecord[hashPrimes[i + 1]];
			backingArraySize = hashPrimes[i + 1];
			break;
		}
	}

	numItems = 0;
	numRemoved = 0;
	for (int i = 0; i < raySize; i++)
		if (!ray[i].isNull && !ray[i].isDel)
			add(ray[i].k, ray[i].x);
	delete[] ray;
}
