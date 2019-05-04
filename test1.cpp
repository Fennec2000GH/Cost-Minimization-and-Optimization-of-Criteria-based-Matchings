#include "Matrix.cpp"
using namespace std;

int main(){

    cout << "Let a graph G representing employee-job relationships be paritioned into two sets,";
    cout << "\nEmployee and Job. Please enter the number of each below: " << endl << endl;
    Partition LEFT("Employee");
    Partition RIGHT("Job");
    int numRows = 3, numCols = 3;
    //cin >> numRows, numCols;


    LEFT.addLeftNode("A1");
    LEFT.addLeftNode("A2");
    LEFT.addLeftNode("A3");
    RIGHT.addRightNode("B1");
    RIGHT.addRightNode("B2");
    RIGHT.addRightNode("B3");

    Matrix Cost(numRows, numCols);
    cout << "Cost Matrix (initialized to 0)" << endl << endl;
    Cost.printMatrix();
    cout << endl;

    Cost.setEntry(1, 1, 4);
    Cost.setEntry(1, 2, 4);
    Cost.setEntry(1, 3, 6);
    Cost.setEntry(2, 1, 1);
    Cost.setEntry(2, 2, 3);
    Cost.setEntry(2, 3, 4);
    Cost.setEntry(3, 1, 1);
    Cost.setEntry(3, 2, 2);
    Cost.setEntry(3, 3, 2);

    cout << "Cost Matrix" << endl << endl;
    Cost.printMatrix();
    cout << endl;

    Matrix Copy(Cost.getRow(), Cost.getCol());
    Copy = Cost;
    Matrix::Hungarian H(Copy);

    cout << endl << "A copy of the cost matrix (called Copy) to be computed on:\t" << endl;
    cout << "Copy of Cost Matrix" << endl << endl;
    Copy.printMatrix();
    cout << endl;


    cout << endl << "The Hungarian Algorithm will be applied below." << endl;

    //STEP 1
    cout << "Step 1: Subtract the smallest entry of each row from the same row." << endl;
    cout << "Results" << endl << endl;
    for(unsigned int i = 1; i <= numRows; i++)
        H.singleRowReduce(Copy, i);
    Copy.printMatrix();


    //STEP 2
    cout << endl << "Step 2: Subtract the smallest entry of each column from the same column.";
    cout << endl << "Result" << endl << endl;
    for(unsigned int j = 1; j <= numCols; j++)
        H.singleColReduce(Copy, j);
    Copy.printMatrix();

    //STEP 3
    cout << endl << "Step 3: Assign the vectors rowUZero and colUZero. Each vector records the ";
    cout << "\nnumber of zeros in the ith row or jth column, respectively." << endl << endl;


    for(unsigned int i = 1; i <= numRows; i++)
        H.setEntryRow(i, H.rowUZeroCount(Copy, i));
    for(unsigned int j = 1; j <= numCols; j++)
        H.setEntryCol(j, H.colUZeroCount(Copy, j));
    cout << left << setw(30) << "# of zeros per each row:  "; H.printVector(H.rowPtr);
    cout << left << setw(30) << "# of zeros per each column:  "; H.printVector(H.colPtr);
    cout << endl;

    //STEP 4
    cout << endl << "Step 4: Cross out rows and columns such that a minimum number of lines is ";
    cout << "\nneeded to cover the zeros." << endl;
    cout << "X's represent entries that have been crossed out: " << endl << endl;
    int n = H.crossOut(Copy, true);

    cout << endl << "This is also indicated by the two vectors indicating the number of zeros left ";
    cout << "\nper row or column. A value of -1 specially means a line crosses that row or column. ";
    cout << endl << endl;
    H.printVector(H.rowPtr);
    H.printVector(H.colPtr);
    cout << endl << "The minimum number of lines needed to cover all zero entries is "<< n << ". " << endl;


    //STEP 5
    cout << endl << "Step 5: Is the number of crossed lines equal to the number rows or ";
    cout << "\ncolumns in the matrix?" << endl;
    if(n == Cost.getRow())
        cout << endl << "Yes, proceed to Step 7 for assignment of nodes." << endl << endl;
    else
        cout << endl << "No, proceed to Step 6." << endl << endl;

    //STEP 6
    if(n != Cost.getRow()){
        cout << "Since there are less lines needed to cover all the zeros than the number of rows ";
        cout << "\n/columns of the matrix, the matrix must undergo further operations to achieve ";
        cout << "\n the same number of lines as rows/columns. " << endl << endl;
        cout << "1) Subtract the smallest uncovered entry from each uncovered entry. " << endl;
        cout << "2) Add the smallest uncovered entry to each intersection entry. " << endl;
        cout << "3) Circumstantial: If the number of lines still does not match the number of rows ";
        cout << "\n or columns, keep repeating the two previous steps until the condition is satsified. ";
        cout << endl << endl;

        int retry = 1;
        while(n != Cost.getRow()){
            cout << "RETRY #" << retry << ": " << endl;
            H.readjustment(Copy);
            for(unsigned int i = 1; i <= numRows; i++)
                H.setEntryRow(i, H.rowUZeroCount(Copy, i));
            for(unsigned int j = 1; j <= numCols; j++)
                H.setEntryCol(j, H.colUZeroCount(Copy, j));
            cout << "Reduced Matrix: " << endl << endl;
            Copy.printMatrix();
            cout << endl << endl;
            cout << "X's represent entries that have been crossed out: " << endl << endl;
            n = H.crossOut(Copy, true);
            cout << endl << "The current minimum number of lines needed to cover all zero entries is " << n << ". " << endl << endl;
            ++retry;
        }
    }

    //STEP 7
    if(n == Cost.getRow()){
        cout << "Assign each Node object in LEFT partition to only 1 Node object in RIGHT partition." << endl;
        cout << "If a row has only a single zero, assign the Node object in LEFT partition corresponding" << endl;
        cout << "to that row to the Node object in RIGHT partition first." << endl << endl;

        cout << "RESULTS: " << endl;
        cout << "------- " << endl;
        H.assignLink(Copy, LEFT, RIGHT);
        for(unsigned int i = 1; i <= numRows; i++)
            cout << (LEFT.getPtr(i) -> getID()) << " will be assigned to " << (LEFT.getPtr(i) -> getLink(1) -> getID()) << endl;
    }

    return 0;
}
