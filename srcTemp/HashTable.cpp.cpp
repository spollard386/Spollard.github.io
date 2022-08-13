//============================================================================
// Name        : HashTable.cpp
// Author      : Marcos Gonzalez
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <algorithm>
#include <climits>
#include <iostream>
#include <string> // atoi
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

const unsigned int DEFAULT_SIZE = 179;

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

//============================================================================
// Hash Table class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a hash table with chaining.
 */
class HashTable {

private:
    // FIXME (1): Define structures to hold bids
	struct Node {
		Bid bid;
		unsigned key;
		Node* next;

		//default constructor
		Node() {
			key = UINT_MAX;
			next = nullptr;
		}

		//initialize with a bid
		Node(Bid aBid) : Node() {
			bid = aBid;
		}

		//initialize with a bid and a key
		Node(Bid aBid, unsigned aKey) : Node(aBid) {
			key = aKey;
		}
	};
	vector<Node> nodes;

	unsigned tableSize = DEFAULT_SIZE;

    unsigned int hash(int key);

public:
    HashTable();
    HashTable(unsigned size);
    virtual ~HashTable();
    void Insert(Bid bid);
    void PrintAll();
    void Remove(string bidId);
    Bid Search(string bidId);
};

/**
 * Default constructor
 */
HashTable::HashTable() {
    // FIXME (2): Initialize the structures used to hold bids
	nodes.resize(tableSize);
}
HashTable::HashTable(unsigned size) {
	this->tableSize = size;
	nodes.resize(tableSize);
}
/**
 * Destructor
 */
HashTable::~HashTable() {
    // FIXME (3): Implement logic to free storage when class is destroyed
	nodes.erase(nodes.begin());
}

/**
 * Calculate the hash value of a given key.
 * Note that key is specifically defined as
 * unsigned int to prevent undefined results
 * of a negative list index.
 *
 * @param key The key to hash
 * @return The calculated hash
 */
unsigned int HashTable::hash(int key) {
    // FIXME (4): Implement logic to calculate a hash value
	return key % tableSize;
}

/**
 * Insert a bid
 *
 * @param bid The bid to insert
 */
void HashTable::Insert(Bid bid) {
    // FIXME (5): Implement logic to insert a bid

	//calculate the key for this bid
	unsigned key = hash(atoi(bid.bidId.c_str()));

	// try and retrieve node using the key
	Node* oldNode = &(nodes.at(key));

	//if no entry found for this key
	if (oldNode == nullptr) {
		Node* newNode = new Node(bid, key);
		nodes.insert(nodes.begin() + key, (*newNode));
	} else {
		//node is found
		if (oldNode->key == UINT_MAX) {
			oldNode->key = key;
			oldNode->bid = bid;
			oldNode->next = nullptr;
		} else {
			// find the next open node(last one)
			while (oldNode->next != nullptr) {
				oldNode = oldNode->next;
			}
			oldNode->next = new Node(bid, key);
		}
	}
}

/**
 * Print all bids
 */
void HashTable::PrintAll() {
    // FIXME (6): Implement logic to print all bids
	for (auto nodeSearch = nodes.begin(); nodeSearch != nodes.end(); ++nodeSearch) {

	//Set the conditional if not UNIT_MAX and output the key and bid data:

		if (nodeSearch->key != UINT_MAX) {
			cout << "Key " << nodeSearch->key << ": " << nodeSearch->bid.bidId << " | "
				<< nodeSearch->bid.title << " | " << nodeSearch->bid.amount << " | "
	            << nodeSearch->bid.fund << endl;

	//while node not equal to nullptr output the node data:

			Node* node = nodeSearch->next;
			while (node != nullptr) {
				cout << "    " << node->key << ": " << node->bid.bidId
					<< " | " << node->bid.title << " | " << node->bid.amount
					<< " | " << node->bid.fund << endl;
			node = node->next;
			}
		}
	}
}

/**
 * Remove a bid
 *
 * @param bidId The bid id to search for
 */
void HashTable::Remove(string bidId) {
    // FIXME (7): Implement logic to remove a bid

	Node* node = &(nodes.at(key));

	if (node->bid.bidId.compare(bidId) == 0 && node->next == nullptr) {
		// if only node, erase them all
		nodes.erase(nodes.begin() + key);
		return;
	}
	//If it is not the only node check the chain:
	if (node->bid.bidId.compare(bidId) == 0 && node->next != nullptr) {
		// The first node is the only node stored within the vector
		// so we have to insert the next bid and store it in the vector

		// Create temp node from next node data
		Node* tempNode = node->next;
		//Node* tempNode = new Node(node->next->bid, node->next->key, node->next->next);

		// erase the contents of vector at key
		nodes.erase(nodes.begin() + key);

		// insert the new node
		nodes.insert(nodes.begin() + key, (*tempNode));
		return;
	}

	// If the value is in the chain but not in the vector:

	// create a pointer to the current node and skip to next node
	Node* prevNode = node;
	node = node->next;

	// loop over nodes
	while (node != nullptr) {

		// when we find the bid we're looking for
		if (node->bid.bidId.compare(bidId) == 0) {
			// point previous node to the next node
			prevNode->next = node->next;

			// free up the memory
			delete node;
			node = nullptr;
			return;
		}
		// set previous node and skip ahead
	prevNode = node;
	node = node->next;
	}
}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid HashTable::Search(string bidId) {
    Bid bid;

    // FIXME (8): Implement logic to search for and return a bid

    //calculate the key for this bid
    unsigned key = hash(atoi(bidId.c_str()));

    // try and retrieve node using the key
    Node* node = &(nodes.at(key));

    // if no entry is found that matches
    if(node == nullptr || node->key == UINT_MAX) {
        	return bid;
    }

    // if node found that matches key
    if(node != nullptr && node->key != UINT_MAX
    		&& node->bid.bidId.compare(bidId) == 0) {
    	return node->bid;
    }

    // walk the linked list to find the match
    while (node != nullptr) {
    	if (node->key != UINT_MAX && node->bid.bidId.compare(bidId) == 0) {
    		return node->bid;
       	}
    	node = node->next;
    }

    return bid;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(string csvPath, HashTable* hashTable) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            hashTable->Insert(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, searchValue;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        searchValue = "98109";
        break;
    case 3:
        csvPath = argv[1];
        searchValue = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
        searchValue = "98109";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a hash table to hold all the bids
    HashTable* bidTable;

    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            bidTable = new HashTable();

            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            loadBids(csvPath, bidTable);

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bidTable->PrintAll();
            break;

        case 3:
            ticks = clock();

            bid = bidTable->Search(searchValue);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
                cout << "Bid Id " << searchValue << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 4:
            bidTable->Remove(searchValue);
            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
