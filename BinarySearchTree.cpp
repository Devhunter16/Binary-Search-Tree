//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : Devin Hunter
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

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

// Internal structure for tree node
struct Node {
    Bid bid;
    Node* left;
    Node* right;

    // this is a default constructor for nodes
    Node() {
        // this makes it so when we add a new node the left and right pointers point to null initially
        left = nullptr;
        right = nullptr;
    }

    // second constructor that we initialize with a bid
    Node(Bid aBid) : Node() {
        this->bid = aBid;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Bid bid);
    void inOrder(Node* node);
    Node* removeNode(Node* node, string bidId);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Bid bid);
    void Remove(string bidId);
    Bid Search(string bidId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {

    // calls inOrder fuction and passes root 
    this->inOrder(root);
}

/**
 * Insert a bid
 */
void BinarySearchTree::Insert(Bid bid) {

    // if there is nothing in the tree... 
    if (root == nullptr) {

        // create a new node which holds a bid and make it the root
        root = new Node(bid);
    }

    // otherwise pass the bid to addNode()
    else {
        this->addNode(root, bid);
    }
}

/**
 * Remove a bid
 */
void BinarySearchTree::Remove(string bidId) {
    
    // remove node root bidID 
    this->removeNode(root, bidId);
}

/**
 * Search for a bid
 */
Bid BinarySearchTree::Search(string bidId) {

    // we start searching from the root node
    Node* current = root;

    // ...and we keep looping to traverse the tree downward until either we reach the end or the correct node is found
    while (current != nullptr) {

        // if the current node matches, return it
        if (current->bid.bidId.compare(bidId) == 0) {
            return current->bid;
        }

        // if the bid is smaller than the current node's bidId then traverse to the left
        if (bidId.compare(current->bid.bidId) < 0) {
            current = current->left; 
        }

        // otherwise, if the bid is larger then traverse to the right
        else {
            current = current->right; 
        }
    }

    Bid bid;
    return bid;
}

void BinarySearchTree::addNode(Node* node, Bid bid) {

    // if node's bidId is larger than the bidId of the one we're adding...
    if (node->bid.bidId.compare(bid.bidId) > 0) {

        // if the left node is empty add the bid there
        if (node->left == nullptr) {
            node->left = new Node(bid);
        }

        // otherwise keep traversing to the left the list with this recursive call to addNode()
        else {
            this->addNode(node->left, bid);
        }
    }

    // otherwise (if bidId is smaller than the bidId of the one we're adding) add to the right subtree
    else {

        // if right node is empty add the bid there
        if (node->right == nullptr) {
            node->right = new Node(bid);
        }

        // otherwise keep traversing to the right the list with this recursive call to addNode()
        else {
            this->addNode(node->right, bid);
        }
    }
}

void BinarySearchTree::inOrder(Node* node) {

    // if the the node is not empty
    if (node != nullptr) {
        inOrder(node->left);

        // print bid info
        cout << node->bid.bidId << ": " 
             << node->bid.title << " | " 
             << node->bid.amount << " | "
             << node->bid.fund << endl;
        inOrder(node->right);

    }
}

Node* BinarySearchTree::removeNode(Node* node, string bidId) {

    // if this node is null, then return (avoid crashing)
    if (node == nullptr) {
        return node;
    }

    // if the bidId is smaller then recurse down left subtree
    if (bidId.compare(node->bid.bidId) < 0) {
        node->left = removeNode(node->left, bidId);
    }

    // if the bidId is larger then recurse down right subtree
    else if (bidId.compare(node->bid.bidId) > 0) {
        node->right = removeNode(node->right, bidId); 
    }

    // otherwise the nodes match and the matching node is removed
    else {

        // if the node is a leaf node (meaning the node we're removing has no children)
        if (node->left == nullptr && node->right == nullptr) {

            // we delete the node and do not have to worry about children
            delete node;
            node = nullptr;
        }

        // if the node has one left child and nor right child
        else if (node->left != nullptr && node->right == nullptr) {

            // we create a copy of the node as temp
            Node* temp = node;

            // our pointer is moved to the left child node
            node = node->left;

            // then we delete the node 
            delete temp;
        }


        // if the node has one right child and no left child
        else if (node->left == nullptr && node->right != nullptr) {
            Node* temp = node;
            node = node->right;
            delete temp;

        }

        // if the node has two children
        else {

            // we temporarily move the pointer to the right child node
            Node* temp = node->right;

            while (temp->left != nullptr) {
                temp = temp->left;
            }

            node->bid = temp->bid;
            node->right = removeNode(node->right, temp->bid.bidId);

        }
    }
    return node;
}

//============================================================================
// Static methods used for testing
//============================================================================

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
void loadBids(string csvPath, BinarySearchTree* bst) {
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
            bst->Insert(bid);
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
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98188";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
        bidKey = "98188";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a binary search tree to hold all bids
    BinarySearchTree* bst;
    bst = new BinarySearchTree();

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
            //bst = new BinarySearchTree();

            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            loadBids(csvPath, bst);

            //cout << bst->Size() << " bids read" << endl;

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bst->InOrder();
            break;

        case 3:
            ticks = clock();

            bid = bst->Search(bidKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
            	cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 4:
            bst->Remove(bidKey);
            break;
        }
    }

    cout << "Good bye." << endl;

	return 0;
}
