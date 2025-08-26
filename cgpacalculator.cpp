#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <map>

using namespace std;

// Structure to represent a course
struct Course {
    string name;
    string code;
    int creditHours;
    string grade;
    double gradePoint;
    
    Course(string n, string c, int ch, string g) 
        : name(n), code(c), creditHours(ch), grade(g) {
        // Convert grade to grade point (10-point system used in Indian universities)
        if (g == "O" || g == "o") gradePoint = 10.0;
        else if (g == "A+" || g == "a+") gradePoint = 9.0;
        else if (g == "A" || g == "a") gradePoint = 8.0;
        else if (g == "B+" || g == "b+") gradePoint = 7.0;
        else if (g == "B" || g == "b") gradePoint = 6.0;
        else if (g == "C" || g == "c") gradePoint = 5.0;
        else if (g == "P" || g == "p") gradePoint = 4.0;
        else gradePoint = 0.0; // F or absent
    }
};

// Structure to represent a semester
struct Semester {
    string name;
    vector<Course> courses;
    
    Semester(string n) : name(n) {}
    
    void addCourse(const Course& course) {
        courses.push_back(course);
    }
    
    double calculateSGPA() const {
        if (courses.empty()) return 0.0;
        
        double totalGradePoints = 0.0;
        int totalCreditHours = 0;
        
        for (const auto& course : courses) {
            totalGradePoints += course.gradePoint * course.creditHours;
            totalCreditHours += course.creditHours;
        }
        
        return totalGradePoints / totalCreditHours;
    }
    
    int getTotalCredits() const {
        int total = 0;
        for (const auto& course : courses) {
            total += course.creditHours;
        }
        return total;
    }
};

// Class to manage student academic record
class StudentRecord {
private:
    string studentName;
    string registerNumber;
    string program;
    string department;
    vector<Semester> semesters;
    
public:
    StudentRecord(string name, string regNo, string prog, string dept) 
        : studentName(name), registerNumber(regNo), program(prog), department(dept) {}
    
    void addSemester(const Semester& semester) {
        semesters.push_back(semester);
    }
    
    // Find a semester by name, returns nullptr if not found
    Semester* findSemester(const string& semesterName) {
        for (auto& semester : semesters) {
            if (semester.name == semesterName) {
                return &semester;
            }
        }
        return nullptr;
    }
    
    double calculateCGPA() const {
        if (semesters.empty()) return 0.0;
        
        double totalGradePoints = 0.0;
        int totalCreditHours = 0;
        
        for (const auto& semester : semesters) {
            for (const auto& course : semester.courses) {
                totalGradePoints += course.gradePoint * course.creditHours;
                totalCreditHours += course.creditHours;
            }
        }
        
        return totalGradePoints / totalCreditHours;
    }
    
    void displaySemesterResults(const string& semesterName) const {
        for (const auto& semester : semesters) {
            if (semester.name == semesterName) {
                cout << "\n=== " << semesterName << " Results ===\n";
                cout << "----------------------------------------------------------------------------\n";
                cout << setw(12) << "Course Code" << setw(30) << "Course Name" 
                     << setw(8) << "Credits" << setw(8) << "Grade" << setw(10) << "Grade Points" << endl;
                cout << "----------------------------------------------------------------------------\n";
                
                for (const auto& course : semester.courses) {
                    cout << setw(12) << course.code << setw(30) << course.name 
                         << setw(8) << course.creditHours << setw(8) << course.grade 
                         << setw(10) << fixed << setprecision(2) << course.gradePoint << endl;
                }
                
                cout << "----------------------------------------------------------------------------\n";
                cout << "Semester SGPA: " << fixed << setprecision(2) << semester.calculateSGPA() << endl;
                cout << "Total Credits: " << semester.getTotalCredits() << endl;
                return;
            }
        }
        cout << "Semester not found!\n";
    }
    
    void displayTranscript() const {
        cout << "\n======================================== ACADEMIC TRANSCRIPT ========================================\n";
        cout << "Student Name: " << studentName << endl;
        cout << "Register Number: " << registerNumber << endl;
        cout << "Program: " << program << endl;
        cout << "Department: " << department << endl;
        cout << "====================================================================================================\n";
        
        for (const auto& semester : semesters) {
            cout << "\nSemester: " << semester.name << endl;
            cout << "----------------------------------------------------------------------------\n";
            cout << setw(12) << "Course Code" << setw(30) << "Course Name" 
                 << setw(8) << "Credits" << setw(8) << "Grade" << setw(10) << "Grade Points" << endl;
            cout << "----------------------------------------------------------------------------\n";
            
            for (const auto& course : semester.courses) {
                cout << setw(12) << course.code << setw(30) << course.name 
                     << setw(8) << course.creditHours << setw(8) << course.grade 
                     << setw(10) << fixed << setprecision(2) << course.gradePoint << endl;
            }
            
            cout << "----------------------------------------------------------------------------\n";
            cout << "Semester SGPA: " << fixed << setprecision(2) << semester.calculateSGPA() << endl;
            cout << "Total Credits: " << semester.getTotalCredits() << endl;
        }
        
        cout << "\n====================================================================================================\n";
        cout << "Cumulative GPA (CGPA): " << fixed << setprecision(2) << calculateCGPA() << endl;
        cout << "====================================================================================================\n";
    }
    
    void saveToFile(const string& filename) const {
        ofstream outFile(filename);
        if (!outFile) {
            cerr << "Error opening file for writing!\n";
            return;
        }
        
        outFile << studentName << endl;
        outFile << registerNumber << endl;
        outFile << program << endl;
        outFile << department << endl;
        
        for (const auto& semester : semesters) {
            outFile << "SEMESTER:" << semester.name << endl;
            for (const auto& course : semester.courses) {
                outFile << course.code << "," << course.name << "," 
                        << course.creditHours << "," << course.grade << endl;
            }
            outFile << "ENDSEMESTER" << endl;
        }
        
        outFile.close();
        cout << "Data saved to " << filename << " successfully!\n";
    }
    
    void loadFromFile(const string& filename) {
        ifstream inFile(filename);
        if (!inFile) {
            cerr << "Error opening file for reading!\n";
            return;
        }
        
        semesters.clear();
        getline(inFile, studentName);
        getline(inFile, registerNumber);
        getline(inFile, program);
        getline(inFile, department);
        
        string line;
        Semester* currentSemester = nullptr;
        
        while (getline(inFile, line)) {
            if (line.find("SEMESTER:") == 0) {
                string semesterName = line.substr(9);
                semesters.push_back(Semester(semesterName));
                currentSemester = &semesters.back();
            } else if (line == "ENDSEMESTER") {
                currentSemester = nullptr;
            } else if (currentSemester != nullptr) {
                size_t pos1 = line.find(',');
                size_t pos2 = line.find(',', pos1 + 1);
                size_t pos3 = line.find(',', pos2 + 1);
                
                if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos) {
                    string code = line.substr(0, pos1);
                    string name = line.substr(pos1 + 1, pos2 - pos1 - 1);
                    int credits = stoi(line.substr(pos2 + 1, pos3 - pos2 - 1));
                    string grade = line.substr(pos3 + 1);
                    
                    currentSemester->addCourse(Course(name, code, credits, grade));
                }
            }
        }
        
        inFile.close();
        cout << "Data loaded from " << filename << " successfully!\n";
    }
    
    // Search for a course by code across all semesters
    void searchCourse(const string& courseCode) const {
        bool found = false;
        
        for (const auto& semester : semesters) {
            for (const auto& course : semester.courses) {
                if (course.code == courseCode) {
                    if (!found) {
                        cout << "\nCourse Found:\n";
                        cout << "----------------------------------------------------------------------------\n";
                        cout << setw(12) << "Course Code" << setw(30) << "Course Name" 
                             << setw(8) << "Credits" << setw(8) << "Grade" 
                             << setw(10) << "Grade Points" << setw(15) << "Semester" << endl;
                        cout << "----------------------------------------------------------------------------\n";
                        found = true;
                    }
                    
                    cout << setw(12) << course.code << setw(30) << course.name 
                         << setw(8) << course.creditHours << setw(8) << course.grade
                         << setw(10) << fixed << setprecision(2) << course.gradePoint
                         << setw(15) << semester.name << endl;
                }
            }
        }
        
        if (!found) {
            cout << "Course with code " << courseCode << " not found!\n";
        } else {
            cout << "----------------------------------------------------------------------------\n";
        }
    }
    
    // Get all semester names
    vector<string> getSemesterNames() const {
        vector<string> names;
        for (const auto& semester : semesters) {
            names.push_back(semester.name);
        }
        return names;
    }
    
    // Display grade system information
    static void displayGradeSystem() {
        cout << "\n========== SRM UNIVERSITY GRADE SYSTEM ==========\n";
        cout << "O  - Outstanding (10 points)\n";
        cout << "A+ - Excellent (9 points)\n";
        cout << "A  - Very Good (8 points)\n";
        cout << "B+ - Good (7 points)\n";
        cout << "B  - Above Average (6 points)\n";
        cout << "C  - Average (5 points)\n";
        cout << "P  - Pass (4 points)\n";
        cout << "F  - Fail (0 points)\n";
        cout << "=================================================\n";
    }
};

// Function to display main menu
void displayMenu() {
    cout << "\n========== SRM UNIVERSITY CGPA CALCULATOR ==========\n";
    cout << "1. Add Semester with Courses\n";
    cout << "2. Calculate Semester SGPA\n";
    cout << "3. Calculate Overall CGPA\n";
    cout << "4. Display Full Transcript\n";
    cout << "5. Search for a Course\n";
    cout << "6. Save Data to File\n";
    cout << "7. Load Data from File\n";
    cout << "8. View Grade System\n";
    cout << "9. Exit\n";
    cout << "====================================================\n";
    cout << "Enter your choice: ";
}

// Function to add a semester with all its courses at once
void addSemesterWithCourses(StudentRecord& student) {
    string semesterName;
    cout << "Enter semester name (e.g., Semester I, Semester II, etc.): ";
    getline(cin, semesterName);
    
    int numCourses;
    cout << "Enter number of courses in " << semesterName << ": ";
    cin >> numCourses;
    cin.ignore(); // Clear input buffer
    
    if (numCourses <= 0) {
        cout << "Invalid number of courses. Semester not added.\n";
        return;
    }
    
    Semester newSemester(semesterName);
    
    cout << "\nEnter details for each course:\n";
    for (int i = 0; i < numCourses; i++) {
        string courseCode, courseName, grade;
        int creditHours;
        
        cout << "\nCourse " << i + 1 << ":\n";
        cout << "Enter course code (e.g., CSE101, MAT201): ";
        getline(cin, courseCode);
        cout << "Enter course name: ";
        getline(cin, courseName);
        cout << "Enter credit hours: ";
        cin >> creditHours;
        cout << "Enter grade (O, A+, A, B+, B, C, P, F): ";
        cin >> grade;
        cin.ignore(); // Clear input buffer
        
        // Convert grade to uppercase for consistency
        for (char &c : grade) {
            c = toupper(c);
        }
        
        // Validate grade
        if (grade != "O" && grade != "A+" && grade != "A" && 
            grade != "B+" && grade != "B" && grade != "C" && 
            grade != "P" && grade != "F") {
            cout << "Invalid grade. Using 'F' as default.\n";
            grade = "F";
        }
        
        newSemester.addCourse(Course(courseName, courseCode, creditHours, grade));
    }
    
    student.addSemester(newSemester);
    cout << "Semester '" << semesterName << "' with " << numCourses << " courses added successfully!\n";
}

int main() {
    string name, regNo, program, dept;
    cout << "========== SRM UNIVERSITY CGPA CALCULATOR ==========\n";
    cout << "Enter student name: ";
    getline(cin, name);
    cout << "Enter register number: ";
    getline(cin, regNo);
    cout << "Enter program (e.g., B.Tech CSE, MCA, etc.): ";
    getline(cin, program);
    cout << "Enter department: ";
    getline(cin, dept);
    
    StudentRecord student(name, regNo, program, dept);
    int choice;
    
    do {
        displayMenu();
        cin >> choice;
        cin.ignore(); // Clear input buffer
        
        switch (choice) {
            case 1: {
                addSemesterWithCourses(student);
                break;
            }
            case 2: {
                // Display available semesters
                vector<string> semesters = student.getSemesterNames();
                if (semesters.empty()) {
                    cout << "No semesters available. Please add a semester first.\n";
                    break;
                }
                
                cout << "Available semesters:\n";
                for (size_t i = 0; i < semesters.size(); i++) {
                    cout << i + 1 << ". " << semesters[i] << endl;
                }
                
                string semesterName;
                cout << "Enter semester name to calculate SGPA: ";
                getline(cin, semesterName);
                student.displaySemesterResults(semesterName);
                break;
            }
            case 3: {
                double cgpa = student.calculateCGPA();
                cout << "\n========================================\n";
                cout << "Cumulative GPA (CGPA): " << fixed << setprecision(2) << cgpa << endl;
                
                // Add some interpretation
                if (cgpa >= 9.0) cout << "Classification: First Class with Distinction\n";
                else if (cgpa >= 8.0) cout << "Classification: First Class\n";
                else if (cgpa >= 7.0) cout << "Classification: Second Class\n";
                else if (cgpa >= 6.0) cout << "Classification: Third Class\n";
                else cout << "Classification: Need Improvement\n";
                
                cout << "========================================\n";
                break;
            }
            case 4: {
                student.displayTranscript();
                break;
            }
            case 5: {
                string courseCode;
                cout << "Enter course code to search: ";
                getline(cin, courseCode);
                student.searchCourse(courseCode);
                break;
            }
            case 6: {
                string filename;
                cout << "Enter filename to save data: ";
                getline(cin, filename);
                student.saveToFile(filename);
                break;
            }
            case 7: {
                string filename;
                cout << "Enter filename to load data: ";
                getline(cin, filename);
                student.loadFromFile(filename);
                break;
            }
            case 8: {
                StudentRecord::displayGradeSystem();
                break;
            }
            case 9: {
                cout << "Exiting SRM University CGPA Calculator. Goodbye!\n";
                break;
            }
            default: {
                cout << "Invalid choice! Please try again.\n";
                break;
            }
        }
    } while (choice != 9);
    
    return 0;
}