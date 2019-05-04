#ifndef NODE_H
#define NODE_H
#include <map>
#include <string>
#include <tuple>
#include <vector>
using namespace std;

//FORWARD DECLARATIONS AND REDEFINTIONS
class LeftNode;
enum Compare{
    LOWER,
    UPPER,
    LOWER_EQUAL,
    UPPER_EQUAL,
    EXACT,
    PREFERRED,
    IGNORE
};
using info = tuple<int*, double*, char*, string*, Compare>;

class Node{
public:
    friend class Partition;

    //CONSTRUCTORS
    Node(string identifier);

    //DESTRUCTORS
    virtual ~Node();

    //ACCESSORS
    string getID() const;
    void printLinks() const;
    virtual void addInfo(const string & key, const string & value, Compare comp) = 0;
    virtual string getInfo(const string & key) const = 0;
    virtual int getMapSize() const = 0;
    Node * getLink(const int & i) const;
    int linksSize() const;
    bool linkExists(const string & key) const;

    //MUTATORS
    void setID(const string & identifier);
    void setLink(Node * ptr);
    void eraseLink(Node * ptr);
    friend void Node::eraseLink(Node * ptr);


    //OPERATOR OVERLOADING
    friend ostream & operator << (ostream & out, Node * ptr);

protected:
    string id;
    vector<Node*> links;

};

class RightNode : public Node{
public:
    friend class Partition;
    friend bool qualify(const LeftNode & l, const RightNode & r);

    //CONSTRUCTORS
    RightNode(const string & identifier, const int & cap = 1);

    //DESTRUCTORS
    ~RightNode();

    //ACCESSORS
    int getMapSize() const;
    string getKey(const int & i) const;
    bool checkKey(const LeftNode & l, const string & key) const;
    string getInfo(const string & key) const;
    Compare getConstraint(const string & key) const;
    info getTuple(const string & key) const;
    double getWeight(const string & key) const;
    int getCapacity() const;

    //MUTATORS
    void addInfo(const string & key, const string & value, Compare comp);
    void setCapacity(const int & cap);
    void setWeight(const string & key, const double & w);

protected:
    map<string, info> criteria;
    map<string, double> weight;
    int capacity;

};

class LeftNode : public Node{
public:
    friend class Partition;
    friend bool qualify(const LeftNode & l, const RightNode & r);

    //CONSTRUCTORS
    LeftNode(const string & id);

    //DESTRUCTORS
    ~LeftNode();

    //ACCESSORS
    friend bool RightNode::checkKey(const LeftNode & l, const string & key) const;
    int getMapSize() const;
    string getKey(const int & i) const;
    string getInfo(const string & key) const;
    info getTuple(const string & key) const;

    //MUTATORS
    void addInfo(const string & key, const string & value, Compare comp = IGNORE);

protected:
    map<string, info> attribute;

};

#endif
