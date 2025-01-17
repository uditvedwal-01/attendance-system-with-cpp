#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <ctime>
using namespace std;




bool authenticateTeacher(const string &teacherID, const string &password, string &section, string &course)
{
    ifstream file("teachers_database.txt");
    if (!file.is_open())
    {
        cout << "Error: Teachers database file not found!" << endl;
        return false;
    }
    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string id, pass, sec, crs;
        getline(ss, id, ',');
        getline(ss, pass, ',');
        getline(ss, sec, ',');
        getline(ss, crs, ',');
        if (id == teacherID && pass == password)
        {
            section = sec;
            course = crs;
            return true;
        }
    }
    return false;
}




vector<string> getStudentDetails(const string &rollNumber)
{
    ifstream file("students_database.txt");
    if (!file.is_open())
    {
        cout << "Error: Student database file not found!" << endl;
        return {};
    }
    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        vector<string> details;
        string token;
        while (getline(ss, token, ','))
        {
            details.push_back(token);
        }
        if (!details.empty() && details[0] == rollNumber)
        {
            return details;
        }
    }
    return {};
}





void viewAttendanceHistory(const string &rollNumber)
{
    ifstream studentDB("students_database.txt");
    if (!studentDB.is_open())
    {
        cout << "Error: Students database file not found!" << endl;
        return;
    }
    string line;
    string section, course, name;
    while (getline(studentDB, line))
    {
        stringstream ss(line);
        vector<string> details;
        string token;
        while (getline(ss, token, ','))
        {
            details.push_back(token);
        }
        if (details[0] == rollNumber)
        {
            section = details[3];
            course = details[4];
            name = details[2];
            break;
        }
    }
    studentDB.close();
    if (section.empty() || course.empty())
    {
        cout << "Error: Roll Number not found in the database!" << endl;
        return;
    }
    cout << "Attendance History for " << name << " (Roll Number: " << rollNumber << "):" << endl;
    cout << "-----------------------------------------" << endl;
    for (int month = 01; month <= 12; month++)
    {
        for (int day = 01; day <= 31; day++)
        {
            stringstream date;
            date << (day < 10 ? "0" : "") << day << "-" << (month < 10 ? "0" : "") << month << "-2025"; 
            string attendanceFile = section + "_" + course + "_" + date.str() + ".txt";
            ifstream file(attendanceFile);
            if (file.is_open())
            {
                bool found = false;
                string attendanceLine;
                while (getline(file, attendanceLine))
                {
                    if (attendanceLine.find(rollNumber) != string::npos)
                    {
                        cout << date.str() << " - Present" << endl;
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    cout << date.str() << " - Absent" << endl;
                }
                file.close();
            }
        }
    }
    cout << "-----------------------------------------" << endl;
}



string getCurrentDate()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    stringstream date;
    date << (ltm->tm_mday < 10 ? "0" : "") << ltm->tm_mday << "-"
         << (1 + ltm->tm_mon < 10 ? "0" : "") << (1 + ltm->tm_mon) << "-"
         << 1900 + ltm->tm_year;
    return date.str();
}




void markAttendance(const string &rollNumber)
{
    vector<string> studentDetails = getStudentDetails(rollNumber);
    if (studentDetails.empty())
    {
        cout << "Error: Roll Number " << rollNumber << " not found in the database!" << endl;
        return;
    }
    string enteredPassword;
    cout << "Enter your password: ";
    cin >> enteredPassword;
    if (enteredPassword != studentDetails[6])
    {
        cout << "Error: Incorrect password!" << endl;
        return;
    }
    cout << "1. Mark Attendance" << endl;
    cout << "2. View Attendance History" << endl;
    cout << "Enter your choice: ";
    int choice;
    cin >> choice;
    if (choice == 1)
    {
        string section = studentDetails[3];
        string course = studentDetails[4];
        string date = getCurrentDate();
        string attendanceFile = section + "_" + course + "_" + date + ".txt";
        ifstream inFile(attendanceFile);
        if (inFile.is_open())
        {
            string line;
            while (getline(inFile, line))
            {
                if (line.find(rollNumber) != string::npos)
                {
                    cout << "Attendance already marked for Roll Number " << rollNumber << " on " << date << endl;
                    inFile.close(); 
                    return;
                }
            }
            inFile.close(); 
        }
        ofstream outFile(attendanceFile, ios::app);
        if (!outFile.is_open())
        {
            cout << "Error: Unable to open or create attendance file!" << endl;
            return;
        }
        outFile << studentDetails[0] << "," << studentDetails[1] << "," << studentDetails[2] << endl;
        cout << "Attendance marked successfully for Roll Number " << studentDetails[0] << " in file " << attendanceFile << endl;
        outFile.close();
    }
    else if (choice == 2)
    {
        viewAttendanceHistory(rollNumber);
    }
    else
    {
        cout << "Invalid choice. Returning to main menu." << endl;
    }
}





void markAttendanceByTeacher(const string &rollNumber)
{
    vector<string> studentDetails = getStudentDetails(rollNumber);
    if (studentDetails.empty())
    {
        cout << "Error: Roll Number " << rollNumber << " not found in the database!" << endl;
        return;
    }
    string section = studentDetails[3];
    string course = studentDetails[4];
    string date = getCurrentDate();
    string attendanceFile = section + "_" + course + "_" + date + ".txt";
    ifstream inFile(attendanceFile);
    if (inFile.is_open())
    {
        string line;
        while (getline(inFile, line))
        {
            if (line.find(rollNumber) != string::npos)
            {
                cout << "Attendance already marked for Roll Number " << rollNumber << " on " << date << endl;
                inFile.close();
                return;
            }
        }
        inFile.close(); 
    }
    ofstream outFile(attendanceFile, ios::app);
    if (!outFile.is_open())
    {
        cout << "Error: Unable to open or create attendance file!" << endl;
        return;
    }
    outFile << studentDetails[0] << "," << studentDetails[1] << "," << studentDetails[2] << endl;
    cout << "Attendance marked successfully for Roll Number " << studentDetails[0] << " in file " << attendanceFile << endl;
    outFile.close();
}





void viewAttendanceForDate(const string &section, const string &course, const string &date)
{
    string attendanceFile = section + "_" + course + "_" + date + ".txt";
    ifstream file(attendanceFile);
    if (!file.is_open())
    {
        cout << "No attendance file found for the specified date (" << date << ") for Section: " << section << ", Course: " << course << endl;
        return;
    }
    cout << "\nAttendance for Section: " << section << ", Course: " << course << " on " << date << ":\n";
    cout << "------------------------------------------" << endl;
    string line;
    while (getline(file, line))
    {
        cout << line << endl;
    }
    cout << "------------------------------------------" << endl;
    file.close();
}





void viewAttendance(const string &section, const string &course)
{
    string date = getCurrentDate();
    viewAttendanceForDate(section, course, date);
}





void viewAbsentStudents(const string &section, const string &course)
{
    string date = getCurrentDate();
    string attendanceFile = section + "_" + course + "_" + date + ".txt";
    map<string, bool> attendanceMap;
    ifstream attendance(attendanceFile);
    if (attendance.is_open())
    {
        string line;
        while (getline(attendance, line))
        {
            stringstream ss(line);
            string rollNumber;
            getline(ss, rollNumber, ',');
            attendanceMap[rollNumber] = true;
        }
        attendance.close();
    }
    ifstream studentDB("students_database.txt");
    if (!studentDB.is_open())
    {
        cout << "Error: Students database file not found!" << endl;
        return;
    }
    cout << "\nAbsent Students for Section: " << section << ", Course: " << course << " on " << date << ":\n";
    cout << "------------------------------------------" << endl;
    bool foundAbsent = false;
    string line;
    while (getline(studentDB, line))
    {
        stringstream ss(line);
        vector<string> details;
        string token;
        while (getline(ss, token, ','))
        {
            details.push_back(token);
        }
        if (details.size() >= 7 && details[3] == section && details[4] == course)
        {
            string rollNumber = details[0];
            if (attendanceMap.find(rollNumber) == attendanceMap.end())
            {
                cout << rollNumber << " - " << details[2] << endl;
                foundAbsent = true;
            }
        }
    }
    if (!foundAbsent)
    {
        cout << "All students are present." << endl;
    }
    cout << "------------------------------------------" << endl;
    studentDB.close();
}





void manuallyMarkAttendance(const string &section, const string &course)
{
    string date = getCurrentDate();
    string attendanceFile = section + "_" + course + "_" + date + ".txt";
    cout << "Enter the roll number of the student: ";
    string rollNumber;
    cin >> rollNumber;
    cout << "Do you want to mark the student as Present (P) or Absent (A)? ";
    char status;
    cin >> status;
    if (toupper(status) == 'P')
    {
        markAttendanceByTeacher(rollNumber);
    }
    else if (toupper(status) == 'A')
    {
        ifstream inFile(attendanceFile);
        if (!inFile.is_open())
        {
            cout << "Error: Attendance file not found for marking absent!" << endl;
            return;
        }
        vector<string> lines;
        string line;
        while (getline(inFile, line))
        {
            if (line.find(rollNumber) == string::npos)
            {
                lines.push_back(line);
            }
        }
        inFile.close();
        ofstream outFile(attendanceFile);
        for (const string &l : lines)
        {
            outFile << l << endl;
        }
        outFile.close();
        cout << "Marked Roll Number " << rollNumber << " as absent for " << date << endl;
    }
    else
    {
        cout << "Invalid input. No changes made." << endl;
    }
}




int main()
{
    while (true)
    {
        cout << "1. Student Login " << endl;
        cout << "2. Teacher Login " << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;
        if (choice == 1)
        {
            cout << "Enter your University Roll Number: ";
            string rollNumber;
            cin >> rollNumber;
            markAttendance(rollNumber);
        }
        else if (choice == 2)
        {
            cout << "Enter Teacher ID: ";
            string teacherID;
            cin >> teacherID;
            cout << "Enter Password: ";
            string password;
            cin >> password;
            string section, course;
            if (authenticateTeacher(teacherID, password, section, course))
            {
                cout << "\n1. View Today's Attendance\n2. View Absent Students\n3. View Past Date Attendance\n4. Manually Mark Attendance\nEnter your choice: ";
                int subChoice;
                cin >> subChoice;
                if (subChoice == 1)
                {
                    viewAttendance(section, course);
                }
                else if (subChoice == 2)
                {
                    viewAbsentStudents(section, course);
                }
                else if (subChoice == 3)
                {
                    cout << "Enter the date (DD-MM-YYYY): ";
                    string date;
                    cin >> date;
                    viewAttendanceForDate(section, course, date);
                }
                else if (subChoice == 4)
                {
                    manuallyMarkAttendance(section, course);
                }
                else
                {
                    cout << "Invalid choice. Returning to main menu.\n";
                }
            }
            else
            {
                cout << "Error: Invalid Teacher ID or Password!" << endl;
            }
        }
        else if (choice == 3)
        {
            break;
        }
        else
        {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
    return 0;
}