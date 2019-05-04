#include "Matrix.cpp"
using namespace std;

int main(){

    //initializes the left and right Partition objects
    Partition Applicant("Applicant");
    Partition Business("Business");

    //list of names and business stored in vectors
    vector<string> applicant_vec = {"Adam", "Tom", "Hector", "Ethan", "Nathan", "Alexa"};
    vector<string> business_vec = {"Google", "Facebook", "Microsoft"};

    //inserting entries into each Partition object
    for(string item : applicant_vec)
        Applicant.addLeftNode(item);
    for(string item : business_vec)
        Business.addRightNode(item);

    //setting the number of open spots available per business
    dynamic_cast<RightNode*>(Business.getPtr("Google")) -> setCapacity(2); //the rest have a capacity of 1 by defaul parameter in constructor

    //A T T R I B U T E S
    //GPA
    Applicant.getPtr("Adam") -> addInfo("GPA", "3.89", IGNORE);
    Applicant.getPtr("Tom") -> addInfo("GPA", "3.81", IGNORE);
    Applicant.getPtr("Hector") -> addInfo("GPA", "3.75", IGNORE);
    Applicant.getPtr("Ethan") -> addInfo("GPA", "3.68", IGNORE);
    Applicant.getPtr("Nathan") -> addInfo("GPA", "3.56", IGNORE);
    Applicant.getPtr("Alexa") -> addInfo("GPA", "4.00", IGNORE);

    //years of experience with any programming language
    Applicant.getPtr("Adam") -> addInfo("Experience", "0", IGNORE);
    Applicant.getPtr("Tom") -> addInfo("Experience", "2", IGNORE);
    Applicant.getPtr("Hector") -> addInfo("Experience", "1", IGNORE);
    Applicant.getPtr("Ethan") -> addInfo("Experience", "2", IGNORE);
    Applicant.getPtr("Nathan") -> addInfo("Experience", "1", IGNORE);
    Applicant.getPtr("Alexa") -> addInfo("Experience", "6", IGNORE);

    //geographic Region of residence
    Applicant.getPtr("Adam") -> addInfo("Region", "Southeast", IGNORE);
    Applicant.getPtr("Tom") -> addInfo("Region", "Southeast", IGNORE);
    Applicant.getPtr("Hector") -> addInfo("Region", "Northeast", IGNORE);
    Applicant.getPtr("Ethan") -> addInfo("Region", "Midwest", IGNORE);
    Applicant.getPtr("Nathan") -> addInfo("Region", "West", IGNORE);
    Applicant.getPtr("Alexa") -> addInfo("Region", "Northwest", IGNORE);

    //primary skill
    Applicant.getPtr("Adam") -> addInfo("Skill", "Typing", IGNORE);
    Applicant.getPtr("Tom") -> addInfo("Skill", "Software", IGNORE);
    Applicant.getPtr("Hector") -> addInfo("Skill", "Software", IGNORE);
    Applicant.getPtr("Ethan") -> addInfo("Skill", "Cybersecurity", IGNORE);
    Applicant.getPtr("Nathan") -> addInfo("Skill", "Cloud Computing", IGNORE);
    Applicant.getPtr("Alexa") -> addInfo("Skill", "Cloud Computing", IGNORE);

    //C R I T E R I A
    //GPA
    Business.getPtr("Google") -> addInfo("GPA", "3.50", LOWER_EQUAL);
    Business.getPtr("Facebook") -> addInfo("GPA", "3.75", LOWER_EQUAL);
    Business.getPtr("Microsoft") -> addInfo("GPA", "3.75", LOWER_EQUAL);

    //years of experience with any programming language
    Business.getPtr("Google") -> addInfo("Experience", "1", LOWER_EQUAL);
    Business.getPtr("Facebook") -> addInfo("Experience", "1", LOWER_EQUAL);
    Business.getPtr("Microsoft") -> addInfo("Experience", "2", LOWER_EQUAL);

    //geographic Region of residence
    Business.getPtr("Google") -> addInfo("Region", "West", PREFERRED);
    Business.getPtr("Facebook") -> addInfo("Region", "West", PREFERRED);
    Business.getPtr("Microsoft") -> addInfo("Region", "Northwest", PREFERRED);

    //desired skills
    Business.getPtr("Google") -> addInfo("Skill", "Software", PREFERRED);
    Business.getPtr("Facebook") -> addInfo("Skill", "Software", PREFERRED);
    Business.getPtr("Microsoft") -> addInfo("Skill", "Cloud Computing", PREFERRED);

    //filters out any unqualified LeftNode objects - unqualified means LeftNode object fails
    //to meet at least one criteria for each of the RightNode objects
    vector<Node*> qualifiedApp;
    vector<Node*> qualifiedBus;

    for(unsigned int i = 1; i <= Applicant.currentSize(); i++)
        if(qualifyForPartition(*dynamic_cast<LeftNode*>(Applicant.getPtr(i)), Business) == true)
            qualifiedApp.push_back(Applicant.getPtr(i));

    for(unsigned int a = 1; a <= qualifiedApp.size(); a++)
        for(unsigned int b = 1; b <= Business.currentSize(); b++)
            if(qualify(*dynamic_cast<LeftNode*>(qualifiedApp.at(a - 1)), *dynamic_cast<RightNode*>(Business.getPtr(b))) != false)
                qualifiedApp.at(a - 1) -> setLink(Business.getPtr(b));

    for(unsigned int i = 1; i <= Business.currentSize(); i++)
        if(Business.getPtr(i) -> linksSize() > 0)
            qualifiedBus.push_back(Business.getPtr(i));

    cout << "The applicants below are qualified to apply for at least one business:" << endl << endl;
    for(Node * & item : qualifiedApp)
        cout << item -> getID() << "\t";

    cout << endl << endl;

    //for each criteria, assign a weight of importance to each criteria in the RightNode map called "weight"
    dynamic_cast<RightNode*>(Business.getPtr("Google")) -> setWeight("GPA", 8);
    dynamic_cast<RightNode*>(Business.getPtr("Google")) -> setWeight("Experience", 2);
    dynamic_cast<RightNode*>(Business.getPtr("Google")) -> setWeight("Region", 1);
    dynamic_cast<RightNode*>(Business.getPtr("Google")) -> setWeight("Skill", 4);

    dynamic_cast<RightNode*>(Business.getPtr("Facebook")) -> setWeight("GPA", 6);
    dynamic_cast<RightNode*>(Business.getPtr("Facebook")) -> setWeight("Experience", 6);
    dynamic_cast<RightNode*>(Business.getPtr("Facebook")) -> setWeight("Region", 1);
    dynamic_cast<RightNode*>(Business.getPtr("Facebook")) -> setWeight("Skill", 3);

    dynamic_cast<RightNode*>(Business.getPtr("Microsoft")) -> setWeight("GPA", 9);
    dynamic_cast<RightNode*>(Business.getPtr("Microsoft")) -> setWeight("Experience", 3);
    dynamic_cast<RightNode*>(Business.getPtr("Microsoft")) -> setWeight("Region", 1);
    dynamic_cast<RightNode*>(Business.getPtr("Microsoft")) -> setWeight("Skill", 2);

    //for each criteria, store the weighted score for each LeftNode object against each RightNode object in a matrix
    //weighted score  = user-given weight * LeftNode.getInfo(key) or weighted score = weight
    int numRows = 0, numCols = 0;
    numRows = qualifiedApp.size();
    for(unsigned int i = 1; i <= Business.currentSize(); i++)
        if(Business.getPtr(i) -> linksSize() > 0)
            ++numCols;

    cout << "Qualified Applicants: " << numRows << "\n" << "Qualified Businesses: " << numCols << endl << endl;

    //a vector of score matrices
    vector<Matrix*> scores;

    for(unsigned int i = 1; i <= (dynamic_cast<RightNode*>(Business.getPtr(1)) -> getMapSize()); i++){
        scores.push_back(new Matrix(numRows, numCols));
        string key = dynamic_cast<RightNode*>(Business.getPtr(1)) -> getKey(i);

        //categorizes the score matrix based on return type
        if(get<0>(dynamic_cast<RightNode*>(Business.getPtr(1)) -> getTuple(key)) != nullptr){
            for(unsigned int app = 1; app <= numRows; app++)
                for(unsigned int bus = 1; bus <= numCols; bus++){
                    double weight = dynamic_cast<RightNode*>(Business.getPtr(bus)) -> getWeight(key);
                    if(qualifiedApp.at(app - 1) -> linkExists(Business.getPtr(bus) -> getID())){
                        int criteriaValue = stoi(qualifiedApp.at(app - 1) -> getInfo(key));
                        scores.back() -> setEntry(app, bus, weight * criteriaValue);
                    }
                }
        } else if(get<1>(dynamic_cast<RightNode*>(Business.getPtr(1)) -> getTuple(key)) != nullptr){
            for(unsigned int app = 1; app <= numRows; app++)
                for(unsigned int bus = 1; bus <= numCols; bus++){
                    double weight = dynamic_cast<RightNode*>(Business.getPtr(bus)) -> getWeight(key);
                    if(qualifiedApp.at(app - 1) -> linkExists(Business.getPtr(bus) -> getID())){
                        double criteriaValue = stod(qualifiedApp.at(app - 1) -> getInfo(key));
                        scores.back() -> setEntry(app, bus, weight * criteriaValue);
                    }
                }
        } else if(get<2>(dynamic_cast<RightNode*>(Business.getPtr(1)) -> getTuple(key)) != nullptr){
            for(unsigned int app = 1; app <= numRows; app++)
                for(unsigned int bus = 1; bus <= numCols; bus++){
                    double weight = dynamic_cast<RightNode*>(Business.getPtr(bus)) -> getWeight(key);
                    char criteriaValue = (qualifiedApp.at(app - 1) -> getInfo(key)).front();
                    if(qualifiedApp.at(app - 1) -> linkExists(Business.getPtr(bus) -> getID()) &&
                        criteriaValue == (dynamic_cast<RightNode*>(Business.getPtr(bus)) -> getInfo(key)).front())
                        scores.back() -> setEntry(app, bus, weight);
                }
        } else if(get<3>(dynamic_cast<RightNode*>(Business.getPtr(1)) -> getTuple(key)) != nullptr){
            for(unsigned int app = 1; app <= numRows; app++)
                for(unsigned int bus = 1; bus <= numCols; bus++){
                    double weight = dynamic_cast<RightNode*>(Business.getPtr(bus)) -> getWeight(key);
                    string criteriaValue = qualifiedApp.at(app - 1) -> getInfo(key);
                    if(qualifiedApp.at(app - 1) -> linkExists(Business.getPtr(bus) -> getID()) &&
                        criteriaValue == dynamic_cast<RightNode*>(Business.getPtr(bus)) -> getInfo(key))
                        scores.back() -> setEntry(app, bus, weight);
                }
        }
    }

    //adds the score matrices for each criteria
    Matrix totalScore(numRows, numCols);
    for(unsigned int q = 1; q <= scores.size(); q++)
        for(unsigned int i = 1; i <= numRows; i++)
            for(unsigned int j = 1; j <= numCols; j++)
                totalScore.setEntry(i, j, totalScore.getEntry(i, j) + (scores.at(q - 1) -> getEntry(i, j)));

    cout << "The matrix below tells the total weighted score of each qualified applicant based on each";
    cout << "\nbusiness's own set of criteria: " << endl << endl;
    totalScore.printMatrix();

    //assigning applicants to each business - remember, some businesses can take more than one applicant
    Matrix Copy(numRows, numCols);
    Copy = totalScore;
    Matrix::Hungarian H(Copy, 1);

    int numAssign = 0;
    int numOpenSpots = 0;
    for(Node * & item : qualifiedBus)
        numOpenSpots += dynamic_cast<RightNode*>(item) -> getCapacity();

    //n assignments will be made, where n is the lower of the two: # of qualified applicants vs. # of open spots to fill in
    numAssign = (numRows <= numOpenSpots) ? numRows : numOpenSpots;

    //actually assigning LeftNode objects to RightNode objects
    vector<int> currentOpenSpots;
    for(Node * & item : qualifiedBus)
        currentOpenSpots.push_back(dynamic_cast<RightNode*>(item) -> getCapacity());

    for(unsigned int i = 1; i <= numAssign; i++){
        int rowPos = get<0>(Copy.findMaxPos());
        int colPos = get<1>(Copy.findMaxPos());
        while(H.getEntry(rowPos, *H.rowPtr) == -1 || H.getEntry(colPos, *H.colPtr) == -1 || currentOpenSpots.at(colPos - 1) == 0){
            Copy.setEntry(rowPos, colPos, -1000);
            rowPos = get<0>(Copy.findMaxPos());
            colPos = get<1>(Copy.findMaxPos());
        }

        Copy.setEntry(rowPos, colPos, -1);
        currentOpenSpots.at(colPos - 1) -= 1;
        H.setEntryRow(rowPos, -1);
        if(currentOpenSpots.at(colPos - 1) == 0)
            H.setEntryCol(colPos, -1);
    }

    cout << endl << endl;
    cout << "The resulting matrix after computations is below. Note that -1 marks assignments between";
    cout << "a LeftNode object and a RightNode object." << endl << endl;
    Copy.printMatrix();

    //leaving only 1 pointer for each LeftNode object once a RightNode has been assigned to it
    //all other links are erased

    cout << "Results" << endl;
    cout << "-------" << endl;
    for(unsigned int i = 1; i <= numRows; i++)
        for(unsigned int j = 1; j <= numCols; j++)
            if(Copy.getEntry(i ,j) == -1)
                cout << qualifiedApp.at(i - 1) -> getID() << " is assigned to " << qualifiedBus.at(j - 1) -> getID() << endl;

    return 0;
}
