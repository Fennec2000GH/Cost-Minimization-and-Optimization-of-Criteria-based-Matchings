#ifndef MATRIX_H
#define MATRIX_H
#include <vector>
using namespace std;
class Partition;

//REDEFINTIONS
using m = vector<vector<double>>;

class Matrix {
public:
    friend class Hungarian;

    //CONSTRUCTORS
    Matrix():Matrix(3, 3){};
    Matrix(const int & r, const int & c, const int & val = 0);
    Matrix(const Matrix & obj);

    //DESTRUCTOR
    ~Matrix();

    //ACCESSORS
    int getRow() const;
    int getCol() const;
    double getEntry(const int & i, const int & j) const;
    double findMax() const;
    pair<int, int> findMaxPos() const;
    double findMin() const;
    pair<int, int> findMinPos() const;
    void printMatrix() const;

    //MUTATORS
    void setEntry(const int & i, const int & j, const double & entry);

    //NESTED CLASS
    class Hungarian {
    public:
        //CONSTRUCTORS
        Hungarian(Matrix & a, const int & val = 0);

        //DESTRUCTORS
        ~Hungarian();

        //ACCESSORS
        int getEntry(const int & i, const vector<int> & v);
        void printVector(const vector<int> * v) const;
        int zeroDensity(const Matrix & a, const int & i, const int & j) const;

        //MUTATORS
        void singleRowReduce(Matrix & a, const int & i);
        void singleColReduce(Matrix & a, const int & j);
        int rowUZeroCount(const Matrix & a, const int & i) const;
        int colUZeroCount(const Matrix & a, const int & i) const;
        void setEntryRow(const int & i, const int & val);
        void setEntryCol(const int & j, const int & val);
        int crossOut(Matrix & a, const bool & print = false);
        void readjustment(Matrix & a);
        void assignLinkPriority(const Matrix & a, Partition & l, Partition & r);
        void assignLink(const Matrix & a, Partition & left, Partition & right);

        //ITERATORS
        vector<int> * rowPtr = &rowUZero;
        vector<int> * colPtr = &colUZero;

    protected:
        vector<int> rowUZero;
        vector<int> colUZero;
    };

protected:
    int row, col;
    m matrix;

};

#endif
