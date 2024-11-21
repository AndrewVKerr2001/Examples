/**
 * @file Database.cpp
 * @author Andrew Kerr
 * @brief A c++ file containing all of the functions used by our database to maintain and modify the database.
 * @version 0.1
 * @date 2022-08-28
 * 
 * @copyright Copyright (c) 2022
 */

#include "Database.hpp"
#include <fstream>
#include <cstring>
#include <map>
#include <list>

namespace AK
{

//===============[ IOSTREAM FUNCTIONS ]============================================================================================
    std::ostream& operator<<(std::ostream& o, const Student& a) {
        o << a.strLastname << ' ' << a.strFirstname << ' ' << a.strPhoneNumber << ' '; 
        return o;
    }

    std::istream& operator>>(std::istream& i, Student& a) {
        i >> a.strLastname >> a.strFirstname >> a.strPhoneNumber; 
        return i;
    }

    std::ostream& operator<<(std::ostream& o, const Course& a) {
        o << a.strPrefix << ' ' << a.nCourseNumber << ' ' << a.strDisplayName << ' ' << a.nCredits << ' ';
        return o;
    }

    std::istream& operator>>(std::istream& i, Course& a) {
        i >> a.strPrefix >> a.nCourseNumber >> a.strDisplayName >> a.nCredits;
        return i;
    }

    std::ostream& operator<<(std::ostream& o, const Term& a) {
        o << a.strSemesterCode << ' ' << a.nYear << ' ' << a.strDisplayName << ' ';
        return o;
    }

    std::istream& operator>>(std::istream& i, Term& a) {
        i >> a.strSemesterCode >> a.nYear >> a.strDisplayName;
        return i;
    }

    std::ostream& operator<<(std::ostream& o, const Grade& a) {
        o << a.strDisplayName << ' ' << a.nGrade << ' ';
        return o;
    }

    std::istream& operator>>(std::istream& i, Grade& a) {
        i >> a.strDisplayName >> a.nGrade;
        return i;
    }

//===============[ COMPARE FUNCTION(S) ]============================================================================================
    bool operator<(const Term& a, const Term& b) {
        if(a.nYear == b.nYear)
            return (a.strSemesterCode > b.strSemesterCode);
        return (a.nYear < b.nYear );
    }
    
//===============[ DATABASE FUNCTIONS ]=============================================================================================

    int Database::run(int argc, char** argv) {

        //This tidbit is meant only to allow me to test a bunch of instructions but not type them out.
        std::istream* input = &std::cin;
        bool bFile = false;
        if(argc > 2){
            if(!strncmp(argv[1], "-f", 2)){ //Check if we are given a file, if so lets open it and then use it as our input.
                std::ifstream* finput = new std::ifstream(argv[2]);
                input = (std::istream*)finput;
                bFile = true;
                if(!input){
                    FATAL_MSG("Failed to allocate memory for the auto file input.");
                    exit(-1);
                }
                if(!finput->is_open()){
                    ERROR_MSG("Usage: " << argv[0] << " -f filename");
                    FATAL_MSG("Failed to execute the given file, file stream failed to open.");
                    exit(-1);
                }
            }
        }
        std::istream& in = *input;


        //Loop through the input and try/execute commands.
        char cCommand, cSubCommand;
        while(true){
            std::cout << ">>> ";
            in >> cCommand;
            if(bFile) std::cout << cCommand << ' ';
            if(cCommand == 'q') break;
            else if(cCommand == 'n') { //Debugger Command, simply adds in a spacer to divide up the output.
                if(bFile) std::cout << '\n';
                std::cout << "=============================================================================\n";
            }
            else if(cCommand == 't') {
                std::string strFirstname, strLastname;
                in >> strLastname;
                in >> strFirstname;
                if(bFile) std::cout << strLastname << ' ' << strFirstname << '\n';
                this->showTranscript(strLastname.c_str(), strFirstname.c_str());
            }
            else { 
                if(cCommand != 'a' && cCommand != 'l') {
                    std::cout << "Unknown command, '" << cCommand << "'.\n";
                    continue;
                }

                in >> cSubCommand;
                if(bFile) std::cout << cSubCommand << ' ';
                if(cCommand == 'a') {
                    if(cSubCommand == 's') {
                        Student student;
                        in >> student.strLastname;
                        in >> student.strFirstname;
                        in >> student.strPhoneNumber;
                        if(bFile) std::cout << student.strLastname << ' ' << student.strFirstname << ' ' << student.strPhoneNumber << '\n';
                        if(!this->addStudent(student))
                            std::cout << L"Duplicate entry for student, what do you think this is SQL. (┛ಠ_ಠ)┛彡┻━┻" << std::endl;
                    }
                    else if(cSubCommand == 'c') {
                        Course course;
                        in >> course.strPrefix;
                        in >> course.nCourseNumber;
                        in >> course.strDisplayName;
                        in >> course.nCredits;
                        if(bFile) std::cout << course.strPrefix << ' ' << course.nCourseNumber << ' ' << course.strDisplayName << ' ' << course.nCredits << '\n';
                        if(!this->addCourse(course))
                            std::cout << L"Duplicate entry for course, what do you think this is SQL. (┛ಠ_ಠ)┛彡┻━┻" << std::endl;
                    }
                    else if(cSubCommand == 'm') {
                        Term term;
                        in >> term.strSemesterCode;
                        in >> term.nYear;
                        in >> term.strDisplayName;
                        if(bFile) std::cout << term.strSemesterCode << ' ' << term.nYear << ' ' << term.strDisplayName << '\n';
                        if(!this->addTerm(term))
                            std::cout << L"Duplicate entry for term, what do you think this is SQL. (┛ಠ_ಠ)┛彡┻━┻" << std::endl;
                    }
                    else if(cSubCommand == 'g') {
                        Grade grade;
                        in >> grade.strDisplayName;
                        in >> grade.nGrade;
                        if(bFile) std::cout << grade.strDisplayName << ' ' << grade.nGrade << '\n';
                        if(!this->addGrade(grade))
                            std::cout << L"Duplicate entry for grade, what do you think this is SQL. (┛ಠ_ಠ)┛彡┻━┻" << std::endl;
                    }
                    else if(cSubCommand == 't') {
                        std::string strLastname, strFirstname, strPrefix, strTerm, strGrade;
                        unsigned int nCourseID;
                        in >> strLastname;
                        in >> strFirstname;
                        in >> strPrefix;
                        in >> nCourseID;
                        in >> strGrade;
                        in >> strTerm;
                        if(bFile) std::cout << strLastname << ' ' << strFirstname << ' ' << strPrefix << ' ' << nCourseID << ' ' << strGrade << ' ' << strTerm << '\n';
                        
                        {
                            Student student;
                            Course course;
                            Term term;
                            Grade grade;
                            if(!this->getStudent(strLastname.c_str(), strFirstname.c_str(), &student)){
                                std::cout << L"No student with that last/first name combo. (┛ಠ_ಠ)┛彡┻━┻" << std::endl;
                                continue;
                            }
                            if(!this->getCourse(strPrefix.c_str(), nCourseID, &course)){
                                std::cout << L"No course with that prefix/id combo. (┛ಠ_ಠ)┛彡┻━┻" << std::endl;
                                continue;
                            }
                            if(!this->getGrade(strGrade.c_str(), &grade)){
                                std::cout << L"No grade with label. (┛ಠ_ಠ)┛彡┻━┻" << std::endl;
                                continue;
                            }
                            if(!this->getTerm(strTerm.c_str(), &term)) {
                                std::cout << L"No term with that code. (┛ಠ_ಠ)┛彡┻━┻" << std::endl;
                                continue;
                            }

                            if(!this->addEnrollment(student, course, term, grade))
                                std::cout << L"Duplicate entry for enrollment, what do you think this is SQL. (┛ಠ_ಠ)┛彡┻━┻" << std::endl;
                        }
                    }
                    else {
                        std::cout << "Unknown add command '" << cSubCommand << "'.\n";
                        continue;
                    }
                }
                else {
                    if(bFile) std::cout << '\n';
                    if(cSubCommand == 'c'){
                        this->listCourses();
                    }
                    else if(cSubCommand == 'g') {
                        this->listGrades();
                    }
                    else if(cSubCommand == 'm') {
                        this->listTerms();
                    }
                    else if(cSubCommand == 's') {
                        this->listStudents();
                    }
                    else if(cSubCommand == 't') {
                        this->listEnrollments();
                    }
                    else {
                        std::cout << "Unknown list command '" << cSubCommand << "'.\n";
                        continue;
                    }
                }
            }
        }
        return 0;
    }

    bool Database::addStudent(const Student& refStudent) {
        if(this->getStudent(refStudent.strLastname.c_str(), refStudent.strFirstname.c_str(), NULL))
            return false;
        
        std::ofstream file("./students.txt", std::ios::out | std::ios::ate | std::ios::app);
        file << refStudent;
        file.flush();
        file.close();
        return true;
    }

    bool Database::addCourse(const Course& refCourse) {
        if(this->getCourse(refCourse.strPrefix.c_str(), refCourse.nCourseNumber, NULL))
            return false;
        
        std::ofstream file("./courses.txt", std::ios::out | std::ios::ate | std::ios::app);
        file << refCourse;
        file.flush();
        file.close();
        return true;
    }

    bool Database::addTerm(const Term& refTerm) {
        if(this->getTerm(refTerm.strSemesterCode.c_str(), NULL))
            return false;
        
        std::ofstream file("./terms.txt", std::ios::out | std::ios::ate | std::ios::app);
        file << refTerm;
        file.flush();
        file.close();
        return true;
    }

    bool Database::addGrade(const Grade& refGrade) {
        if(this->getGrade(refGrade.strDisplayName.c_str(), NULL))
            return false;
        
        std::ofstream file("./grades.txt", std::ios::out | std::ios::ate | std::ios::app);
        file << refGrade;
        file.flush();
        file.close();
        return true;
    }

    bool Database::addEnrollment(const Student& refStudent, const Course& refCourse, const Term& refTerm, const Grade& refGrade) {
        if(this->hasEnrollment(refStudent, refCourse, refTerm))
            return false;

        std::string filename = std::string("./enrollments/") + refStudent.strLastname.c_str() + ' ' + refStudent.strFirstname.c_str() + ' ' + refTerm.strSemesterCode.c_str() + ".txt";
        std::ofstream file(filename, std::ios::out | std::ios::ate | std::ios::app);
        file << refCourse << ' ' << refGrade;
        file.flush();
        file.close();
        return true;
    }

    bool Database::getStudent(const char* lpLastname, const char* lpFirstname, Student* lpStudent) {
        std::ifstream file("./students.txt", std::ios::in);
        if(file.is_open()){
            Student student;
            while(!file.eof()) {
                file >> student;
                if(student.strFirstname == lpFirstname && student.strLastname == lpLastname) {
                    if(lpStudent) *lpStudent = student;
                    return true;
                }
            }
        }
        return false;
    }

    bool Database::getCourse(const char* lpPrefix, unsigned int nCourseID, Course* lpCourse) {
        std::ifstream file("./courses.txt", std::ios::in);
        if(file.is_open()){
            Course course;
            while(!file.eof()) {
                file >> course;
                if(course.strPrefix == lpPrefix && course.nCourseNumber == nCourseID) {
                    if(lpCourse) *lpCourse = course;
                    return true;
                }
            }
        }
        return false;
    }

    bool Database::getTerm(const char* lpSemesterCode, Term* lpTerm) {
        std::ifstream file("./terms.txt", std::ios::in);
        if(file.is_open()){
            Term term;
            while(!file.eof()) {
                file >> term;
                if(term.strSemesterCode == lpSemesterCode) {
                    if(lpTerm) *lpTerm = term;
                    return true;
                }
            }
        }
        return false;
    }

    bool Database::getGrade(const char* lpGradeString, Grade* lpGrade) {
        std::ifstream file("./grades.txt", std::ios::in);
        if(file.is_open()){
            Grade grade;
            while(!file.eof()) {
                file >> grade;
                if(grade.strDisplayName == lpGradeString) {
                    if(lpGrade) *lpGrade = grade;
                    return true;
                }
            }
        }
        return false;
    }

    bool Database::hasEnrollment(const Student& refStudent, const Course& refCourse, const Term& refTerm) {
        std::string filename = std::string("./enrollments/") + refStudent.strLastname.c_str() + ' ' + refStudent.strFirstname.c_str() + ' ' + refTerm.strSemesterCode.c_str() + ".txt";
        std::ifstream file(filename, std::ios::in);
        if(file.is_open()){
            Course course;
            Grade grade;
            while(!file.eof()){
                file >> course >> grade;
                if(refCourse.strPrefix == course.strPrefix && refCourse.nCourseNumber == course.nCourseNumber)
                    return true;
            }
        }
        return false;
    }

    void Database::listStudents() {
        std::ifstream file("./students.txt", std::ios::in);
        if(file.is_open()){
            Student student;
            while(!file.eof()) {
                file >> student;
                std::cout << student.strLastname << ", " << student.strFirstname << ' ' << student.strPhoneNumber << std::endl;
            }
        }
    }

    void Database::listCourses() {
        std::ifstream file("./courses.txt", std::ios::in);
        if(file.is_open()){
            Course course;
            while(!file.eof()) {
                file >> course;
                std::cout << course.strPrefix << course.nCourseNumber << ' ' << course.strDisplayName << ' ' << course.nCredits << std::endl;
            }
        }
    }

    void Database::listTerms() {
        std::ifstream file("./terms.txt", std::ios::in);
        if(file.is_open()){
            Term term;
            while(!file.eof()) {
                file >> term;
                std::cout << term.strSemesterCode << ' ' << term.nYear << ' ' << term.strDisplayName << std::endl;
            }
        }
    }

    void Database::listGrades() {
        std::ifstream file("./grades.txt", std::ios::in);
        if(file.is_open()){
            Grade grade;
            while(!file.eof()) {
                file >> grade;
                std::cout << grade.strDisplayName << ' ' << grade.nGrade << std::endl;
            }
        }
    }

    void Database::listEnrollments() {
        std::ifstream fileTerm("./terms.txt", std::ios::in);
        if(fileTerm.is_open()) {
            Term term;
            while(!fileTerm.eof()) {
                fileTerm >> term;

                Student student;
                std::ifstream fileStudents("./students.txt");
                if(fileStudents.is_open()){

                    std::ifstream tmp;
                    while(!fileStudents.eof()){
                        fileStudents >> student;
                        tmp.open(std::string("./enrollments/") + student.strLastname + ' ' + student.strFirstname + ' ' + term.strSemesterCode + ".txt");
                        if(tmp.is_open()){
                            Course course;
                            Grade grade;
                            while(!tmp.eof()){
                                tmp >> course >> grade;
                                if(!tmp.good()) break;
                                std::cout << student.strLastname << ' ' << student.strFirstname << ' ' << term.strDisplayName << ' ' << course.strPrefix << course.nCourseNumber << ' ' << course.strDisplayName << ' ' << grade.strDisplayName << std::endl;
                            }
                            tmp.close();
                        }

                    }
                }
            }
        }
    }

    void Database::showTranscript(const char* lpLastname, const char* lpFirstname) {
        std::ifstream file("./terms.txt", std::ios::in);
        if(file.is_open()) {
            std::map<Term, std::list<std::pair<Course, Grade>>> mapEnrollments;

            //Load in all of our terms and throw the subsequent enrollments into the mapEnrollments object (auto sorted for us).
            {
                Term term;
                std::ifstream tmp;
                while(!file.eof()) {
                    file >> term;
                    tmp.open(std::string("./enrollments/") + lpLastname + ' ' + lpFirstname + ' ' + term.strSemesterCode.c_str() + ".txt");
                    if(tmp.is_open()){
                        std::pair<Course, Grade> tmpPairEnrollment;
                        std::list<std::pair<Course,Grade>> tmpListEnrollments;
                        while(!tmp.eof()){
                            tmp >> tmpPairEnrollment.first >> tmpPairEnrollment.second;
                            if(!tmp.good() && tmpListEnrollments.size()) break;
                            tmpListEnrollments.push_back(tmpPairEnrollment);
                        }
                        mapEnrollments.insert({term, tmpListEnrollments});
                        tmp.close();
                    }
                }
            }

            //Loop through and print out the transcript while generating the credit / gpa info.
            double nGPA = 0;
            double nCREDITS = 0;
            for(auto term = mapEnrollments.begin(); term != mapEnrollments.end(); term++) {
                std::cout << "============ Semester: " << term->first.strDisplayName << ' ' << term->first.nYear << " ============" << std::endl;
                for(auto& pair : term->second) {
                    std::cout << pair.first.strPrefix << pair.first.nCourseNumber << ' ' << pair.first.strDisplayName << " (" << pair.first.nCredits << ") " << pair.second.strDisplayName << std::endl;
                    nGPA += pair.second.nGrade * (double)pair.first.nCredits;
                    nCREDITS += pair.first.nCredits;
                }
            }
            std::cout << "STUDENT HOURS COMPLETED: " << nCREDITS << std::endl;
            std::cout << "STUDENT GPA: " << (nGPA / nCREDITS) << std::endl;
        }
    }

}

int main(int argc, char* argv[]) {
    AK::Database db;
    return db.run(argc, argv); //Simply goto our prompt.
}