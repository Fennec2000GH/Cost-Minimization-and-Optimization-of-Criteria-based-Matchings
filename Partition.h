#ifndef PARTITION_H
#define PARTITION_H
#include "Node.h"
#include <vector>
using namespace std;

class Partition {
public:
    friend class Matrix;
    friend bool qualifyForPartition(const LeftNode & l, const Partition & p);

    //CONSTRUCTORS
    Partition(const string & identifier, unsigned int size = 0);

    //DESTRUCTOR
    ~Partition();

    //ACCESSORS
    int currentSize() const;
    string getNodeID(const int & n) const;
    Node * getPtr(const int & n) const;
    Node * getPtr(const string & identifier) const;


    //MUTATORS
    void addRightNode(const string & identifier);
    void addLeftNode(const string & identifier);
    void deleteNode(const int & n);
    void deleteNode(const string & identifier);


protected:
    string id;
    vector<Node*> vec;

};

#endif
