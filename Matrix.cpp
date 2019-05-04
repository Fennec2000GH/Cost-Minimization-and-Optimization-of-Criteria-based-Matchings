#include <algorithm>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>
#include "Matrix.h"
#include "Partition.cpp"
using namespace std;

//C L A S S    M A T R I X    M E M B E R S
//CONSTRUCTORS
Matrix::Matrix(const int & r, const int & c, const int & val):row(r), col(c) {
    matrix.clear();
    matrix.resize(r);
    for(unsigned int i = 1; i <= r; i++)
        matrix.at(i - 1).resize(c, val);
}

//copy constructor
Matrix::Matrix(const Matrix & obj){
    row = obj.getRow();
    col = obj.getCol();
    for(unsigned int i = 1; i <= row; i++)
        for(unsigned int j = 1; j <= col; j++)
            setEntry(i, j, obj.getEntry(i, j));
}

//DESTRUCTORS
Matrix::~Matrix(){
    matrix.clear();
}
//ACCESSORS
//returns the number of rows in matrix
int Matrix::getRow() const {
    return row;
}

//returns the number of columns in matrix
int Matrix::getCol() const {
    return col;
}

//gets the value stored at ith row and jth column
double Matrix::getEntry(const int & i, const int & j) const {
    return matrix[i - 1][j - 1];
}

//finds the largest value in all the matrix entries
double Matrix::findMax() const{
    double max = matrix[0][0];
    for(unsigned int i = 1; i <= row; i++)
        for(unsigned int j = 1; j <= col; j++)
            if(getEntry(i, j) > max)
                max = getEntry(i, j);
    return max;
}

//returns position of the largest value in all the matrix entries, first occurrence
pair<int, int> Matrix::findMaxPos() const{
    for(unsigned int i = 1; i <= row; i++)
        for(unsigned int j = 1; j <= col; j++)
            if(getEntry(i, j) == findMax())
                return pair<int, int>(i, j);
    return pair<int, int>(0, 0);
}

//finds the smallest value in all the matrix entries
double Matrix::findMin() const{
    double min = matrix[0][0];
    for(unsigned int i = 1; i <= row; i++)
        for(unsigned int j = 1; j <= col; j++)
            if(getEntry(i, j) < min)
                min = getEntry(i, j);
    return min;
}

//returns position of the smallest value in all the matrix entries,  first occurrence
pair<int, int> Matrix::findMinPos() const{
    for(unsigned int i = 1; i <= row; i++)
        for(unsigned int j = 1; j <= col; j++)
            if(getEntry(i, j) == findMin())
                return pair<int, int>(i, j);
    return pair<int, int>(0, 0);
}

//prints out the matrix (without any brackets)
void Matrix::printMatrix() const {
    for(unsigned int i = 1; i <= row; i++)
        for(unsigned int j = 1; j <= col; j++){
            cout << getEntry(i, j) << "\t";
            if(j == col)
                cout << endl;
        }
    return;
}

//MUTATORS
//modifies a specific entry at ith row and jth column
void Matrix::setEntry(const int & i, const int & j, const double & entry) {
    matrix[i - 1][j - 1] = entry;
    return;
}

//C L A S S    H U N G A R I A N    M E M B E R S
//CONSTRUCTORS
Matrix::Hungarian::Hungarian(Matrix & a, const int & val){
    rowUZero.resize(a.matrix.size(), val);
    colUZero.resize(a.matrix[0].size(), val);
}

//DESTRUCTORS
Matrix::Hungarian::~Hungarian(){}

//ACCESSORS
//gets the ith entry in rowUZero or colUZero
int Matrix::Hungarian::getEntry(const int & i, const vector<int> & v){
    return v.at(i - 1);
}

//prints specified vector member of class Hungarian
void Matrix::Hungarian::printVector(const vector<int> * v) const {
    cout << "[";
    for(unsigned int i = 1; i <= (*v).size(); i++){
        cout << (*v)[i - 1];
        if(i != (*v).size())
            cout << "\t";
    }
    cout << "]" << endl;
    return;
}

//retunrs the number of zeros above, below, left, and right of the a_ij entry, not including the entry itself
int Matrix::Hungarian::zeroDensity(const Matrix & a, const int & i, const int & j) const{
    int zeros = 0;
    if(a.matrix[i - 1][j - 1] == 0)
        zeros = rowUZeroCount(a, i) + colUZeroCount(a, j) - 2;
    else
        zeros = rowUZeroCount(a, i) + colUZeroCount(a, j);
    return zeros;
}

//MUTATORS
//in the ith row, the minimum entry is subtracted from each row entry
void Matrix::Hungarian::singleRowReduce(Matrix & a, const int & i){
    double minimum = *min_element(a.matrix.at(i - 1).begin(), a.matrix.at(i - 1).end());
    for(unsigned int j = 1; j <= a.col; j++)
        a.setEntry(i, j, a.getEntry(i, j) - minimum);
    return;
}

//in the jth column, the minimum entry is subtracted from each column entry
void Matrix::Hungarian::singleColReduce(Matrix & a, const int & j){
    vector<double> jth_column = {};
    for(unsigned int i = 1; i <= a.row; i++)
        jth_column.push_back(a.getEntry(i, j));
    double minimum = *min_element(jth_column.begin(), jth_column.end());
    for(unsigned int i = 1; i <= a.row; i++)
        a.setEntry(i, j, a.getEntry(i, j) - minimum);
    return;
}

//finds the current number of uncrossed zeros in the ith row
int Matrix::Hungarian::rowUZeroCount(const Matrix & a, const int & i) const {
    return count(a.matrix.at(i - 1).begin(), a.matrix.at(i - 1).end(), 0);
}

//finds the current number of uncrossed zeros in the jth column
int Matrix::Hungarian::colUZeroCount(const Matrix & a, const int & j) const {
    vector<double> jth_column = {};
    for(unsigned int i = 1; i <= a.row; i++)
        jth_column.push_back(a.matrix[i - 1][j - 1]);
    return count(jth_column.begin(), jth_column.end(), 0);
}

//sets the ith entry in rowUZero vector
void Matrix::Hungarian::setEntryRow(const int & i, const int & val){
    rowUZero.at(i - 1) = val;
    return;
}

//sets the ith entry in colUZero vector
void Matrix::Hungarian::setEntryCol(const int & j, const int & val){
    colUZero.at(j - 1) = val;
    return;
}

//finds the minimal number of lines needed to crossout all zeros in the matrix
int Matrix::Hungarian::crossOut(Matrix & a, const bool & print){
    int numCrosses = 0;                                                         //shows the current count of crossed out rows/columns
    vector<int>::iterator maxR;                                                 //stores iterator pointing to maximum element of rowUZero
    vector<int>::iterator maxC;                                                 //stores iterator pointing to maximum element of colUZero
    while(!(all_of(rowUZero.begin(), rowUZero.end(), [](int i){return i <= 0;}) //continues while loop if there are remaining zeros still not crossed out in any row or column
    && all_of(colUZero.begin(), colUZero.end(), [](int i){return i <= 0;}))){
        maxR = max_element(rowUZero.begin(), rowUZero.end());
        maxC = max_element(colUZero.begin(), colUZero.end());
        if(*maxR > *maxC || (*maxR == *maxC &&
            count(rowUZero.begin(), rowUZero.end(), *maxR) >=
            count(colUZero.begin(), colUZero.end(), *maxC))){
            *maxR = -1;                                                         //-1 indicates that the row maxR currently points to is crossed out
            ++numCrosses;
            int index = distance(rowUZero.begin(), maxR);
            for(unsigned int j = 1; j <= a.col; j++)
                if(a.matrix[index][j - 1] == 0 && colUZero.at(j - 1) != -1)     //if # of zeros in this row is 0, the corresponding entry in colUZero
                    colUZero.at(j - 1) -= 1;                                    //is decremented to indicate one less uncrossed zero in column j
        } else if(*maxR < *maxC || (*maxR == *maxC &&
                count(rowUZero.begin(), rowUZero.end(), *maxR) <
                count(colUZero.begin(), colUZero.end(), *maxC))){
            *maxC = -1;                                                         //-1 indicates that the row maxC currently points to is crossed out
            ++numCrosses;
            int index = distance(colUZero.begin(), maxC);
            for(unsigned int i = 1; i <= a.row; i++)
                if(a.matrix[i - 1][index] == 0 && rowUZero.at(i - 1) != 0)      //if # of zeros in this column is 0, the corresponding entry in rowUZero
                    rowUZero.at(i - 1) -= 1;                                    //is decremented to indicate one less uncrossed zero in row i
        }
    }

    if(print != false){                                                         //prints a new matrix indicating rows and columns crossed if print = true
        for(unsigned int i = 1; i <= a.row; i++){
            for(unsigned int j = 1; j <= a.col; j++){
                if(rowUZero.at(i - 1) == -1 || colUZero.at(j - 1) == -1)
                    cout << left << setw(8) << "X";
                else
                    cout << left << setw(8) << a.matrix[i - 1][j - 1];
            }
            cout << endl;
        }
    }

    return numCrosses;
}

void Matrix::Hungarian::readjustment(Matrix & a){                               //further row/column operations in attempt to achieve a minimum number of lines                                                                                //to cover all zeros to be the same as the # of rows/columns
    vector<double> uncovered = {};                                              //stores all uncovered entries after crossed lines have been drawn
    for(unsigned int i = 1; i <= a.row; i++)
        for(unsigned int j = 1; j <= a.col; j++)
            if(rowUZero.at(i - 1) != -1 && colUZero.at(j - 1) != -1)
                uncovered.push_back(a.matrix[i - 1][j - 1]);
    double minimum = *min_element(uncovered.begin(), uncovered.end());
    for(unsigned int i = 1; i <= a.row; i++)
        for(unsigned int j = 1; j <= a.col; j++){
            if(rowUZero.at(i - 1) != -1 && colUZero.at(j - 1) != -1)
                a.setEntry(i, j, a.getEntry(i, j) - minimum);
            else if(rowUZero.at(i - 1) == -1 && colUZero.at(j - 1) == -1)
                a.setEntry(i, j, a.getEntry(i, j) + minimum);
        }
    return;
}

//assigns links between nodes where each node is the only assignable option of the other node
void Matrix::Hungarian::assignLinkPriority(const Matrix & a, Partition & l, Partition & r){
    while(any_of(rowUZero.begin(), rowUZero.end(), [](int i){return i == 1;})   //continues as long as any row or column has only one zero
       || any_of(colUZero.begin(), colUZero.end(), [](int i){return i == 1;})){ //meaning there is only one assignment option for this LEFT or RIGHT node
        while(find(rowUZero.begin(), rowUZero.end(), 1) != rowUZero.end()){     //crosses out all rows with only one zero entry
            int rowIndex = distance(rowUZero.begin(), find(rowUZero.begin(), rowUZero.end(), 1));
            rowUZero.at(rowIndex) = -1;
            int colIndex = 0;
            for(unsigned int j = 1; j <= a.col; j++)
                if(a.matrix[rowIndex][j - 1] == 0 && colUZero.at(j - 1) != -1){
                    colIndex = j - 1;
                    break;
                }
            colUZero[colIndex] = -1;

            for(unsigned int i = 1; i <= a.row; i++)                          //crosses out all other potential LeftNode objects vying for the RightNode object at position colIndex
                if(a.matrix[i - 1][colIndex] == 0 && rowUZero.at(i - 1) != -1)
                    rowUZero.at(i - 1) -= 1;
            for(unsigned int j = 1; j <= a.col; j++)                          //crosses out all other potential RIGHTNode objects vying for the LeftNode object at position colIndex
                if(a.matrix[rowIndex][j - 1] == 0 && colUZero.at(j - 1) != -1)
                    colUZero.at(j - 1) -= 1;

            l.getPtr(rowIndex + 1) -> setLink(r.getPtr(colIndex + 1));       //actually sets the link

        }

        while(find(colUZero.begin(), colUZero.end(), 1) != colUZero.end()){   //crosses out all columns with only one column entry
            int colIndex = distance(colUZero.begin(), find(colUZero.begin(), colUZero.end(), 1));
            colUZero[colIndex] = -1;
            int rowIndex = 0;
            for(unsigned int i = 1; i <= a.row; i++)
                if(a.matrix[i - 1][colIndex] == 0 && rowUZero.at(i - 1) != -1){
                    rowIndex = i - 1;
                    break;
                }
            rowIndex = -1;

            for(unsigned int i = 1; i <= a.row; i++)
                if(a.matrix[i - 1][colIndex] == 0 && rowUZero.at(i - 1) != -1)
                    rowUZero.at(i - 1) -= 1;
            for(unsigned int j = 1; j <= a.col; j++)
                if(a.matrix[rowIndex][j - 1] == 0 && colUZero.at(j - 1) != -1)
                    colUZero.at(j - 1) -= 1;

            l.getPtr(rowIndex + 1) -> setLink(r.getPtr(colIndex + 1));         //actually sets the link

        }
    }

    return;
}

//main function to assign links using smaller, more detailed assignLink functions
void Matrix::Hungarian::assignLink(const Matrix & a, Partition & l, Partition & r){
    for(unsigned int i = 1; i <= a.row; i++)                                    //renews the rowUZero vector to account the number of zeros per row in the matrix
        setEntryRow(i, rowUZeroCount(a, i));
    for(unsigned int j = 1; j <= a.col; j++)                                    //renews the rowUZero vector to account the number of zeros per row in the matrix
        setEntryCol(j, colUZeroCount(a, j));

    assignLinkPriority(a, l, r);

    //the row index of the most sparse entry in the matrix
    int sparseRowIndex = 0;

    //the column index of the most sparse entry in the matrix
    int sparseColIndex = 0;

    //the number of zeros above, below, left, and right of the entry, not including the entry itself
    int currentDensity = 0;

    while(any_of(rowUZero.begin(), rowUZero.end(), [](int i){return i > 0;})){
        sparseRowIndex = distance(rowUZero.begin(), find_if(rowUZero.begin(), rowUZero.end(), [](int i){return i > 0;}));
        sparseColIndex = distance(colUZero.begin(), find_if(colUZero.begin(), colUZero.end(), [](int i){return i > 0;}));
        currentDensity = zeroDensity(a, sparseRowIndex + 1, sparseColIndex + 1);
        for(unsigned int i = sparseRowIndex + 1; i <= a.row; i++)
            for(unsigned int j = sparseColIndex + 1; j <= a.col; j++)
                if(a.matrix[i - 1][j - 1] == 0 && rowUZero.at(i - 1) > 0 && colUZero.at(j - 1) > 0
                && currentDensity > zeroDensity(a, i ,j)){
                    sparseRowIndex = i - 1;
                    sparseColIndex = j - 1;
                    currentDensity = zeroDensity(a, i, j);
                }
        rowUZero.at(sparseRowIndex) = -1;
        colUZero.at(sparseColIndex) = -1;

        for(unsigned int i = 1; i <= a.row; i++)
            if(a.matrix[i - 1][sparseColIndex] == 0 && rowUZero.at(i - 1) != -1)
                rowUZero.at(i - 1) -= 1;
        for(unsigned int j = 1; j <= a.col; j++)
            if(a.matrix[sparseRowIndex][j - 1] == 0 && colUZero.at(j - 1) != -1)
                colUZero.at(j - 1) -= 1;

        //actually sets the link
        l.getPtr(sparseRowIndex + 1) -> setLink(r.getPtr(sparseColIndex + 1));

        assignLinkPriority(a, l, r);

    }

    return;
}
