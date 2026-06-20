#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course newCourse) {
        course = newCourse;
        left = nullptr;
        right = nullptr;
    }
};

class BinarySearchTree {
private:
    Node* root;

    Node* insertNode(Node* node, Course course) {
        if (node == nullptr) {
            return new Node(course);
        }

        if (course.courseNumber < node->course.courseNumber) {
            node->left = insertNode(node->left, course);
        }
        else {
            node->right = insertNode(node->right, course);
        }

        return node;
    }

    void inOrder(Node* node) {
        if (node != nullptr) {
            inOrder(node->left);
            cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;
            inOrder(node->right);
        }
    }

    Course* searchNode(Node* node, string courseNumber) {
        if (node == nullptr) {
            return nullptr;
        }

        if (node->course.courseNumber == courseNumber) {
            return &(node->course);
        }

        if (courseNumber < node->course.courseNumber) {
            return searchNode(node->left, courseNumber);
        }

        return searchNode(node->right, courseNumber);
    }

public:
    BinarySearchTree() {
        root = nullptr;
    }

    void insert(Course course) {
        root = insertNode(root, course);
    }

    void printCourseList() {
        inOrder(root);
    }

    Course* search(string courseNumber) {
        return searchNode(root, courseNumber);
    }
};

string toUpperCase(string text) {
    transform(text.begin(), text.end(), text.begin(), ::toupper);
    return text;
}

vector<string> splitLine(string line) {
    vector<string> tokens;
    string token;
    stringstream ss(line);

    while (getline(ss, token, ',')) {
        tokens.push_back(token);
    }

    return tokens;
}

void loadCourses(string fileName, BinarySearchTree& courseTree) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Error: Could not open file." << endl;
        return;
    }

    string line;

    while (getline(file, line)) {
        vector<string> tokens = splitLine(line);

        if (tokens.size() < 2) {
            cout << "Invalid file format. Skipping line." << endl;
            continue;
        }

        Course course;
        course.courseNumber = tokens[0];
        course.courseTitle = tokens[1];

        for (int i = 2; i < tokens.size(); ++i) {
            course.prerequisites.push_back(tokens[i]);
        }

        courseTree.insert(course);
    }

    file.close();

    cout << "Course data loaded successfully." << endl;
}

void printCourse(Course* course) {
    if (course == nullptr) {
        cout << "Course not found." << endl;
        return;
    }

    cout << course->courseNumber << ", " << course->courseTitle << endl;

    cout << "Prerequisites: ";

    if (course->prerequisites.empty()) {
        cout << "None";
    }
    else {
        for (int i = 0; i < course->prerequisites.size(); ++i) {
            cout << course->prerequisites[i];

            if (i < course->prerequisites.size() - 1) {
                cout << ", ";
            }
        }
    }

    cout << endl;
}

int main() {
    BinarySearchTree courseTree;
    bool dataLoaded = false;
    int choice = 0;
    string fileName;
    string courseNumber;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {
        cout << endl;
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;
        cout << endl;
        cout << "What would you like to do? ";
        cin >> choice;

        if (choice == 1) {
            cout << "Enter the file name: ";
            cin >> fileName;

            loadCourses(fileName, courseTree);
            dataLoaded = true;
        }
        else if (choice == 2) {
            if (!dataLoaded) {
                cout << "Please load the course data first." << endl;
            }
            else {
                cout << "Here is a sample schedule:" << endl;
                cout << endl;
                courseTree.printCourseList();
            }
        }
        else if (choice == 3) {
            if (!dataLoaded) {
                cout << "Please load the course data first." << endl;
            }
            else {
                cout << "What course do you want to know about? ";
                cin >> courseNumber;

                courseNumber = toUpperCase(courseNumber);
                printCourse(courseTree.search(courseNumber));
            }
        }
        else if (choice == 9) {
            cout << "Thank you for using the course planner!" << endl;
        }
        else {
            cout << choice << " is not a valid option." << endl;
        }
    }

    return 0;
}