#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

// STRUCTURE PRESENTATION OF ALL  THE COURS
struct Course {
    string courseNumber;
    string title;
    vector<string> prerequisites;
};

// StructurE NODE BST
struct Node {
    Course course;
    Node* left;
    Node* right;

    // DEFAULT CONSTRUCTOR
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // ConstructOR WITH ONE COURSE 
    Node(Course aCourse) : Node() {
        this->course = aCourse;
    }
};

// Class FOR BST
class BinarySearchTree {

private:
    Node* root;
    void inOrder(Node* node);
    void destroyTree(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    Course Search(string courseNumber);
};

/**
 *DDEFAULT CONSTRUCTOR
 */
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

/**
 * DESTRUCTOR
 */
BinarySearchTree::~BinarySearchTree() {
    destroyTree(root);
}

/**
 * CLEAN RECURSIVE MEMORY BST
 */
void BinarySearchTree::destroyTree(Node* node) {
    if (node != nullptr) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

/**
 * DSIPLAY BST ALPHANUMERIC
 */
void BinarySearchTree::InOrder() {
    this->inOrder(root);
}

/**
 *  InOrder
 */
void BinarySearchTree::inOrder(Node* node) {
    if (node != nullptr) {
        inOrder(node->left);
        cout << node->course.courseNumber << ", " << node->course.title << endl;
        inOrder(node->right);
    }
}

/**
 * InsERT COURSE IN BST
 */
void BinarySearchTree::Insert(Course course) {
    Node* newNode = new Node(course);

    if (root == nullptr) {
        root = newNode;
    }
    else {
        Node* current = root;
        while (true) {
            
            if (course.courseNumber < current->course.courseNumber) {
                if (current->left == nullptr) {
                    current->left = newNode;
                    break;
                }
                else {
                    current = current->left;
                }
            }
            else {
                if (current->right == nullptr) {
                    current->right = newNode;
                    break;
                }
                else {
                    current = current->right;
                }
            }
        }
    }
}

/**
 * RESEARCH SPECIFIC COURSE IN BST
 */
Course BinarySearchTree::Search(string courseNumber) {
    Node* current = root;

    while (current != nullptr) {
        if (current->course.courseNumber == courseNumber) {
            return current->course;
        }
        if (courseNumber < current->course.courseNumber) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    // RETURN COURSE EMPTY IF COURSE NOT FOUND
    Course emptyCourse;
    emptyCourse.courseNumber = "";
    return emptyCourse;
}

/**
 * UPPERCASE
 */
string toUpperCase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}


void loadCourses(string fileName, BinarySearchTree* bst, vector<string>& allCourseNumbers) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Error cannot open the file'" << fileName << "'." << endl;
        return;
    }

    string line;
    vector<Course> tempCourses; 

    
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string token;
        vector<string> tokens;

        while (getline(ss, token, ',')) {
            
            token.erase(token.find_last_not_of(" \r\n\t") + 1);
            token.erase(0, token.find_first_not_of(" \r\n\t"));
            tokens.push_back(token);
        }

        if (tokens.size() < 2) {
            cout << "Error missing something line -> " << line << endl;
            continue;
        }

        Course course;
        course.courseNumber = tokens[0];
        course.title = tokens[1];

        for (size_t i = 2; i < tokens.size(); ++i) {
            course.prerequisites.push_back(tokens[i]);
        }

        tempCourses.push_back(course);
        allCourseNumbers.push_back(course.courseNumber);
    }
    file.close();

   
    int loadedCount = 0;
    for (const auto& course : tempCourses) {
        bool isValid = true;
        for (const string& prereq : course.prerequisites) {
            if (find(allCourseNumbers.begin(), allCourseNumbers.end(), prereq) == allCourseNumbers.end()) {
                cout << "Error: prérequisit'" << prereq
                    << "' course " << course.courseNumber
                    << " doesnt exist" << endl;
                isValid = false;
            }
        }

     
        if (isValid) {
            bst->Insert(course);
            loadedCount++;
        }
    }

    cout << "your process was succesful " << loadedCount << endl;
}


void displayCourse(Course course) {
    cout << "Code  : " << course.courseNumber << endl;
    cout << "Tittle: " << course.title << endl;
    cout << "Prérequisit : ";
    if (course.prerequisites.empty()) {
        cout << "Aucun" << endl;
    }
    else {
        for (size_t i = 0; i < course.prerequisites.size(); ++i) {
            cout << course.prerequisites[i];
            if (i < course.prerequisites.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
}


int main() {
    BinarySearchTree* bst = new BinarySearchTree();
    vector<string> allCourseNumbers;
    string fileName;

    string defaultFileName = "ABCU_Advising_Program_Input.txt";

    int choice = 0;
    cout << "welcome to the orientaton system ABCU." << endl;

    while (choice != 9) {
        cout << "\n================ Menu ================" << endl;
        cout << "1. load the data sturcture" << endl;
        cout << "2. disolay sorted listy" << endl;
        cout << "3. Research and display course" << endl;
        cout << "9. exit" << endl;
        cout << "your choice : ";

        if (!(cin >> choice)) {
            cout << "invalid number please type a valid one" << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
        case 1:
            cout << "Enteer the file name(then press enter for'" << defaultFileName << "') : ";
            cin.ignore();
            getline(cin, fileName);
            if (fileName.empty()) {
                fileName = defaultFileName;
            }
            delete bst;
            bst = new BinarySearchTree();
            allCourseNumbers.clear();

            loadCourses(fileName, bst, allCourseNumbers);
            break;

        case 2:
            if (allCourseNumbers.empty()) {
                cout << "Error please upload file (Option 1)." << endl;
            }
            else {
                cout << "\n--- course list ---" << endl;
                bst->InOrder();
            }
            break;

        case 3:
            if (allCourseNumbers.empty()) {
                cout << "Error please upload a file(Option 1)." << endl;
            }
            else {
                string targetCode;
                cout << "what are you looking for ? (ex: CSCI200) : ";
                cin >> targetCode;
                targetCode = toUpperCase(targetCode);

                cout << "\n----------------------------------------" << endl;
                Course foundCourse = bst->Search(targetCode);
                if (!foundCourse.courseNumber.empty()) {
                    displayCourse(foundCourse);
                }
                else {
                    cout << "error:code'" << targetCode << "' cannot be found" << endl;
                }
                cout << "----------------------------------------" << endl;
            }
            break;

        case 9:
            cout << "thank you for using our system ,see you" << endl;
            break;

        default:
            cout << "invalid option please choose between 1, 2, 3 ou 9." << endl;
            break;
        }
    }

    delete bst;
    return 0;
}