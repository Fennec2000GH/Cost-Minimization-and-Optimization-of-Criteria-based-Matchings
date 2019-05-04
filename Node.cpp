#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <string>
#include "Node.h"

//G L O B A L    F U N C T I O N S
//OPERATOR OVERLOADING
ostream & operator << (ostream & out, Node * ptr){
    out << ptr -> getID();
    return out;
}

//tests whether the whole string contains only letters in the alphabet
bool IsAlpha(const string & str){
    bool b = true;
    for(auto item : str)
        if(isalpha(item) == false){
            b = false;
            break;
        }
    return b;
}

//tests whether the whole string contains only numerical digits (no decimal points)
bool IsInt(const string & str){
    bool b = true;
    for(auto item : str)
        if(isdigit(item) == false){
            b = false;
            break;
        }
    return b;
}

//tests whether the string represents a double
bool IsDouble(const string & str){
    bool b = true;
    unsigned int periodCount = count(str.begin(), str.end(), '.');
    if(periodCount != 1)
        b = false;
    for(auto item : str)
        if(item != '.' && isdigit(item) == false){
            b = false;
            break;
        }
    return b;
}

//removes all trailing zeroes to the right of last non-zero digit, works for both int and double
string RemoveTrailZeros(const string & str){
    string temp = str;
    unsigned int initSize = temp.size();
    for(unsigned int i = initSize - 1; temp[i] == '0'; i--)
        temp.resize(temp.size() - 1);
    return temp;
}

//N O D E     CL A S S
//CONSTRUCTORS
Node::Node(string identifier):id(identifier){}

//DESTRUCTORS
Node::~Node(){}

//ACCESSORS
//finds the identifier for the Node object
string Node::getID() const{
    return id;
}

//prints the ID of each Node object in the vector links
void Node::printLinks() const{
    for(Node * item : links)
        cout << item -> getID() << "\t";
    cout << endl;
}

//returns the ith pointer in the vector "links"
Node * Node::getLink(const int & i) const {
    return links.at(i - 1);
}

//returns the current size of the vector "links"
int Node::linksSize() const{
    return links.size();
}

//checks whether the vector "links" contains a pointer to any Node object with ID "key"
bool Node::linkExists(const string & key) const {
    return any_of(links.begin(), links.end(), [&key](Node * ptr){return ptr -> getID() == key;});
}

//MUTATORS
//sets a new id for the Node object
void Node::setID(const string & identifier){
    id = identifier;
    return;
}

//forms an edge between two vertices (objects of Node)
void Node::setLink(Node * ptr){
    if(none_of(links.begin(), links.end(), [ptr](Node * i){return i == ptr;})){
        links.push_back(ptr);
        links.back() -> setLink(this);
    }
    return;
}

//erases the edge between two vertices (objects of nodes)
void Node::eraseLink(Node * ptr){
    for(vector<Node*>::iterator it = links.begin(); it != links.end(); it++)
        if(*it == ptr){
            auto it2 = find(((*it) -> links).begin(), ((*it) -> links).end(), this);
            delete (*it2);
            ((*it) -> links).erase(it2);
            delete (*it);
            links.erase(it);
        }

    return;
}

//R I G H T    N O D E    C L A S S
//CONSTRUCTORS
RightNode::RightNode(const string & identifier, const int & cap):Node(identifier), capacity(cap){}

//DESTRUCTORS
RightNode::~RightNode(){
    criteria.clear();
}

//ACCESSORS
//returns the size of the map "criteria"
int RightNode::getMapSize() const{
    return criteria.size();
}

//returns the ith key in map "criteria"
string RightNode::getKey(const int & i) const{
    string empty = "";
    unsigned int counter = 1;
    for(map<string, info>::const_iterator it = criteria.begin(); it != criteria.end(); it++){
        if(counter == i)
            return it -> first;
        ++counter;
    }
    return empty;
}

//returns a copy of an attribute from a LeftNode object
bool RightNode::checkKey(const LeftNode & l, const string & key) const{
    bool check = true;
    if(l.attribute.find(key) == l.attribute.end())
        check = false;
    return check;
}

//returns the value of a criteria and its Compare value if the key exists
string RightNode::getInfo(const string & key) const {
    info temp = criteria.at(key);
    string empty = "";
    if(get<0>(temp) != nullptr)
        return to_string(*get<0>(temp));
    else if(get<1>(temp) != nullptr)
        return RemoveTrailZeros(to_string(*get<1>(temp)));
    else if(get<2>(temp) != nullptr)
        return to_string(*get<2>(temp));
    else if(get<3>(temp) != nullptr)
        return *get<3>(temp);
    else
        return empty;
}

//returns the specific type of comparison to be made with attributes of LeftNode objects
//ex. LOWER is lower bound exclusive, LOWER_EQUAL is lower bound inclusive, EXACT is exactly matching
Compare RightNode::getConstraint(const string & key) const{
    info temp = criteria.at(key);
    return get<4>(temp);
}

//returns the tuple in the map attribute corresponding to a key
info RightNode::getTuple(const string & key) const{
    return criteria.at(key);
}

//returns the number of LeftNode objects that can point to *this
int RightNode::getCapacity() const{
    return capacity;
}

//returns the weight of importance in deciding which LeftNode object points to which RightNode object
double RightNode::getWeight(const string & key) const{
    return weight.at(key);
}

//MUTATORS
//appends a new attribute to the map attribute
void RightNode::addInfo(const string & key, const string & value, Compare comp){
    if(IsInt(value) != false){
        int val = stoi(value);
        criteria.insert({key, info(new int(val), nullptr, nullptr, nullptr, comp)});
    } else if(IsDouble(value) != false){
        double val = stod(value);
        criteria.insert({key, info(nullptr, new double(val), nullptr, nullptr, comp)});
    } else if(value.length() == 1){
        char val = char(value[0]);
        criteria.insert({key, info(nullptr, nullptr, new char(val), nullptr, comp)});
    } else {
        string val = value;
        criteria.insert({key, info(nullptr, nullptr, nullptr, new string(val), comp)});
    }
    return;
}

//sets the capacity of LeftNode objects that can have pointers pointing to *this
void RightNode::setCapacity(const int & cap){
    if(cap >= 1)
        capacity = cap;
    return;
}

//sets the weight for a specific attribute in LeftNode if the corresponding criteria in RightNode is satisfied
void RightNode::setWeight(const string & key, const double & w){
    if(criteria.find(key) != criteria.end())
        weight.insert({key, w});
    return;
}

//L E F T    N O D E    C L A S S
//CONSTRUCTORS
LeftNode::LeftNode(const string & id):Node(id){}

//DESTRUCTORS
LeftNode::~LeftNode(){
    attribute.clear();
}

//ACCESSORS
//returns the size of the map "attribute"
int LeftNode::getMapSize() const{
    return attribute.size();
}

//returns the ith key in map "attribute"
string LeftNode::getKey(const int & i) const{
    string empty = "";
    unsigned int counter = 1;
    for(map<string, info>::const_iterator it = attribute.begin(); it != attribute.end(); it++){
        if(counter == i)
            return it -> first;
        ++counter;
    }
    return empty;
}

//returns the value of an attribute if the key exists
string LeftNode::getInfo(const string & key) const{
    info temp = attribute.at(key);
    string empty = "";
    if(get<0>(temp) != nullptr)
        return to_string(*get<0>(temp));
    else if(get<1>(temp) != nullptr)
        return RemoveTrailZeros(to_string(*get<1>(temp)));
    else if(get<2>(temp) != nullptr)
        return to_string(*get<2>(temp));
    else if(get<3>(temp) != nullptr)
        return *get<3>(temp);
    else
        return empty;
}

//returns the tuple in the map attribute corresponding to a key
info LeftNode::getTuple(const string & key) const {
    return attribute.at(key);
}

//MUTATORS
//appends a new attribute to the map attribute
void LeftNode::addInfo(const string & key, const string & value, Compare comp){
    if(IsInt(value) != false){
        int val = stoi(value);
        attribute.insert({key, info(new int(val), nullptr, nullptr, nullptr)});
    } else if(IsDouble(value) != false){
        double val = stod(value);
        attribute.insert({key, info(nullptr, new double(val), nullptr, nullptr)});
    } else if(value.length() == 1){
        char val = char(value.front());
        attribute.insert({key, info(nullptr, nullptr, new char(val), nullptr)});
    } else {
        string val = value;
        attribute.insert({key, info(nullptr, nullptr, nullptr, new string(val))});
    }
    return;
}
