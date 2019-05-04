#include <iostream>
#include <iomanip>
#include "Matrix.cpp"
using namespace std;

int main(){
    int choice = 0;
    cout << "Welcome. This program will allow you to optimize matchings between data objects from ";
    cout << "\ntwo different sets. " << endl;
    cout << "Which scenario do you have? Please choose 1 or 2." << endl;
    cout << "\t 1 - Minimum Cost" << endl;
    cout << "\t 2 - Holistic Selection" << endl;

    cin >> choice;
    cin.ignore();

    //CHOICE 1
    if(choice == 1){

        //assigns the left and right partitions each a user-given ID
        string left = "", right = "";
        cout << "Please enter an identifier name for the left partition." << endl;
        getline(cin, left);
        cout << "Please enter an identifier name for the right partition." << endl;
        getline(cin, right);
        cout << endl;
        Partition LEFT(left);
        Partition RIGHT(right);

        //setting dimensions for each partition and names for each node
        int numRows = 0, numCols = 0;
        cout << "Please enter the number of nodes for the left and right\n";
        cout << "partitions with a space in between, then press Enter." << endl;
        cin >> numRows >> numCols;
        cin.ignore();

        cout << endl;
        cout << "The left partition has " + to_string(numRows) + " nodes and the right \n";
        cout << "partition has " + to_string(numCols) + " nodes." << endl << endl;

        //adding nodes to the LEFT parition
        cout << "Please enter an identifier name for each node in the \n";
        cout << "left partition." << endl << endl;

        for(unsigned int i =1; i <= numRows; i++){
            string id = "";
            cout << "Name for the ";

            //adjusts for spelling
            switch(i){
                case 1: cout << "1st"; break;
                case 2: cout << "2nd"; break;
                case 3: cout << "3rd"; break;
                default: cout << to_string(i) + "th";
            }
            cout << " node:\t";
            getline(cin, id);
            LEFT.addLeftNode(id);
        }

        cout << endl;

        //adding nodes to the right partition
        cout << "Please enter an identifier name for each node in the \n";
        cout << "right partition." << endl << endl;

        for(unsigned int i =1; i <= numCols; i++){
            string id = "";
            cout << "Name for the ";

            //adjusts for spelling
            switch(i){
                case 1:{cout << "1st"; break;}
                case 2:{cout << "2nd"; break;}
                case 3:{cout << "3rd"; break;}
                default:{cout << to_string(i) + "th";}
            }
            cout << " node:\t";
            getline(cin, id);
            RIGHT.addRightNode(id);
        }

        cout << endl;
        cout << "Now, enter the cost of applying each left node to \n";
        cout << "each right node." << endl << endl;

        //sets up the cost matrix and assigns each entry a cost as the user enters the costs below
        Matrix Cost(numRows, numCols);


        for(unsigned int i =1; i <= numRows; i++)
            for(unsigned int j = 1; j <= numCols; j++){
                double cost = 0;
                cout << "Cost of matching " + LEFT.getNodeID(i) + " with ";
                cout << RIGHT.getNodeID(j) + ":\t";
                cin >> cost;
                cin.ignore();
                Cost.setEntry(i, j, cost);
            }

        cout << "The following matrix lists the costs for each pair of nodes \n"; //displays the cost matrix
        cout << "in the order you typed in for each partition. Each row is a \n";
        cout << "left node, and each column is a right node." << endl << endl;
        Cost.printMatrix();

        //creates copy of cost matrix to be used in the Hungarian Algorithm computations
        Matrix Copy(Cost.getRow(), Cost.getCol());
        Copy = Cost;
        Matrix::Hungarian H(Copy);

        //subtracts the smallest entry in row from all entries in the same row
        for(unsigned int i =1; i <= numRows; i++)
            H.singleRowReduce(Copy, i);

        //subtracts the smallest entry in column from all entries in the same column
        for(unsigned int j = 1; j <= numCols; j++)
            H.singleColReduce(Copy, j);

        //assigns rowUZero and colUZero for the object H - stores #of zeros per each row and column
        for(unsigned int i = 1; i <= numRows; i++)
            H.setEntryRow(i, H.rowUZeroCount(Copy, i));
        for(unsigned int j = 1; j <= numCols; j++)
            H.setEntryCol(j, H.colUZeroCount(Copy, j));

        //Cross out rows and columns where a minimum number of lines will cover all zeros
        int n = H.crossOut(Copy, true);

        //if the number of lines does not match the
        if(n != Cost.getRow()){
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

        //if the number of lines equals the number of rows
        if(n == Cost.getRow()){
            cout << "RESULTS: " << endl;
            cout << "------- " << endl;
            H.assignLink(Copy, LEFT, RIGHT);
            for(unsigned int i = 1; i <= numRows; i++)
                cout << (LEFT.getPtr(i) -> getID()) << " will be assigned to " << (LEFT.getPtr(i) -> getLink(1) -> getID()) << endl;
        }
    }
    
    else if(choice == 2){}

    return 0;
}
