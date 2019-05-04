#include <iostream>
#include <vector>
#include "Node.cpp"
#include "Partition.h"
using namespace std;

//G L O B A L    F U N C T I O N S
bool qualify(const LeftNode & l, const RightNode & r){
    bool answer = true;
    if(l.attribute.size() < r.criteria.size())
        return false;
    // cout << l.attribute.size() << "\t" << r.criteria.size() << endl;
    for(map<string, info>::const_iterator it = r.criteria.begin(); it != r.criteria.end(); it++){
        if(l.attribute.find(it -> first) == l.attribute.end())
            return false;
        if(answer == false)
            break;
        string key = it -> first;
        string attributeValue = l.getInfo(key);
        string criteriaValue = r.getInfo(key);
        // cout << "key = " << key << "\nattributeValue = " << attributeValue << "\ncriteriaValue = " << criteriaValue << endl;
        //if type is int
        if(get<0>(l.getTuple(key)) != nullptr){
            // cout << "int" << endl;
            switch(r.getConstraint(key)){
                case LOWER: {answer = (stoi(attributeValue) > stoi(criteriaValue)) ? true : false; break;}
                case UPPER: {answer = (stoi(attributeValue) < stoi(criteriaValue)) ? true : false; break;}
                case LOWER_EQUAL: {answer = (stoi(attributeValue) >= stoi(criteriaValue)) ? true : false; break;}
                case UPPER_EQUAL: {answer = (stoi(attributeValue) <= stoi(criteriaValue)) ? true : false; break;}
                case EXACT: {answer = (stoi(attributeValue) == stoi(criteriaValue)) ? true : false; break;}
                case PREFERRED: {answer = true; break;}
                default: {answer = false;}
            }
            // if(r.getConstraint(key) == LOWER_EQUAL)
                // cout << "LOWER_EQUAL" << endl;
            // cout << "stoi(attributeValue) = " << stoi(attributeValue) << "\nstoi(criteriaValue) = " << stoi(criteriaValue) << endl;
            // cout << (stoi(attributeValue) >= stoi(criteriaValue)) << endl << endl;
        //if type is double
        } else if(get<1>(l.getTuple(key)) != nullptr){
                // cout << "double" << endl;
                switch(r.getConstraint(key)){
                    case LOWER: {answer = (stod(attributeValue) > stod(criteriaValue)) ? true : false; break;}
                    case UPPER: {answer = (stod(attributeValue) < stod(criteriaValue)) ? true : false; break;}
                    case LOWER_EQUAL: {answer = (stod(attributeValue) >= stod(criteriaValue)) ? true : false; break;}
                    case UPPER_EQUAL: {answer = (stod(attributeValue) <= stod(criteriaValue)) ? true : false; break;}
                    case EXACT: {answer = (stod(attributeValue) == stod(criteriaValue)) ? true : false; break;}
                    default: {answer = false;}
                }
            // if(r.getConstraint(key) == LOWER_EQUAL)
            //     cout << "LOWER_EQUAL" << endl;
            // cout << "stod(attributeValue) = " << stod(attributeValue) << "\nstod(criteriaValue) = " << stod(criteriaValue) << endl;
            // cout << (stod(attributeValue) >= stod(criteriaValue)) << endl << endl;
            //if type is char
        } else if(get<2>(l.getTuple(key)) != nullptr){
            switch(r.getConstraint(key)){
                case EXACT:
                {
                    if(attributeValue.front() != criteriaValue.front()) answer = false;
                    break;
                }
                case PREFERRED: {answer = true; break;}
                default: {answer = false;}
            }
            //if type is string
        } else if(get<3>(l.getTuple(key)) != nullptr){
            // cout << "string" << endl;
            switch(r.getConstraint(key)){
                case EXACT:
                {
                    if(attributeValue != criteriaValue) answer = false;
                    break;
                }
                case PREFERRED: {answer = true; break;}
                default: {answer = false;}
            }
            // if(r.getConstraint(key) == PREFERRED)
            //     cout << "PREFERRED" << endl;
        } else {
            answer = false;
        }
    }
    // cout << "answer = " << answer << endl;
    return answer;
}

//tests whether a LeftNode object satisfies all criteria for at least one RightNode object in a Partition object
bool qualifyForPartition(const LeftNode & l, const Partition & p){
    return any_of(p.vec.begin(), p.vec.end(), [&l](Node * ptr){return qualify(l, *dynamic_cast<RightNode*>(ptr));});
}

//CONSTRUCTORS
Partition::Partition(const string & identifier, unsigned int size):
id(identifier){
    vec.resize(size, nullptr);
}

//DESTRUCTORS
Partition::~Partition(){
    for(Node * & item : vec)
        item = nullptr;
    vec.clear();
}

//ACCESSORS
//returns the current size of the vector vec
int Partition::currentSize() const{
    return vec.size();
}

//finds the name (ID) of the nth node stored in Partition::vec
string Partition::getNodeID(const int & n) const{
    return vec.at(n - 1) -> id;
}

//returns the nth element of vec, which is a pointer to a Node object
Node * Partition::getPtr(const int & n) const{
    return vec.at(n - 1);
}

//returns the nth element of vec if a matching ID is found
Node * Partition::getPtr(const string & identifier) const{
    auto it = find_if(vec.begin(), vec.end(), [identifier](Node * ptr){return ptr -> getID() == identifier;});
    return (*it);
}

//MUTATORS
//appends new LeftNode object to the partition vector vec
void Partition::addLeftNode(const string & identifier){
    if(vec.size() > 0 && count(vec.begin(), vec.end(), nullptr) > 0){
        vector<Node*>::iterator it = find(vec.begin(), vec.end(), nullptr);
        *it = new LeftNode(identifier);
    } else {
        vec.push_back(new LeftNode(identifier));
    }
    return;
}

//appends new RightNode object to the partition vector vec
void Partition::addRightNode(const string & identifier){
    if(vec.size() > 0 && count(vec.begin(), vec.end(), nullptr) > 0){
        vector<Node*>::iterator it = find(vec.begin(), vec.end(), nullptr);
        *it = new RightNode(identifier);
    } else {
        vec.push_back(new RightNode(identifier));
    }
    return;
}

//deletes the nth node from vec
void Partition::deleteNode(const int & n){
    delete vec.at(n - 1);
    vec.erase(next(vec.begin(), n - 1));
    return;
}

//deletes the node with id = a from vec
void Partition::deleteNode(const string & identifier){
    vector<Node*>::iterator it = find_if(vec.begin(), vec.end(), [identifier](Node * ptr){return ptr -> getID() == identifier;});
    vec.erase(it);
    return;
}
