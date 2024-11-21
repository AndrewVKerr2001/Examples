/**
 * @file Database.hpp
 * @author Andrew Kerr
 * @brief An h++ file used to define the structure of our database.
 * @version 0.1
 * @date 2022-08-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <string>
#include <Common.hpp>

#ifndef __AK_DATABASE_HPP__
#define __AK_DATABASE_HPP__

namespace AK
{

    //A student object, contains a first and last name along with a phone number.
    class Student {
    public:
        //The last name of the student.
        std::string strLastname;
        //The first name of the student.
        std::string strFirstname;
        //The phone number for the student.
        std::string strPhoneNumber;

    public:
        friend std::ostream& operator<<(std::ostream&, const Student&);
        friend std::istream& operator>>(std::istream&, Student&);
    };

    //A course object, contains a prefix w/couse number, a displayable name and the number of credits.
    class Course {
    public:
        //A prefix code for the course.
        std::string strPrefix;
        //The course number for the course.
        unsigned int nCourseNumber;
        //A name for displaying the course.
        std::string strDisplayName;
        //The number of credits for the course.
        unsigned int nCredits;

    public:
        friend std::ostream& operator<<(std::ostream&, const Course&);
        friend std::istream& operator>>(std::istream&, Course&);
    };

    //A term object, contaisn a semester code, the year and a displayable name.
    class Term {
    public:
        //A semester code used to identify the term.
        std::string strSemesterCode;
        //The year in which the term takes place.
        unsigned int nYear;
        //A displayable name for the term, can be any length.
        std::string strDisplayName;

    public:
        friend std::ostream& operator<<(std::ostream&, const Term&);
        friend std::istream& operator>>(std::istream&, Term&);
        //friend bool operator>(const Term& a, const Term& b);
        friend bool operator<(const Term& a, const Term& b);
        //friend bool operator==(const Term& a, const Term& b);
    };

    //A grade object, contains a numerical grade along with a displayable name.
    class Grade {
    public:
        //A name for displaying the grade.
        std::string strDisplayName;
        //The actual numerical grade.
        double nGrade;
    public:
        friend std::ostream& operator<<(std::ostream&, const Grade&);
        friend std::istream& operator>>(std::istream&, Grade&);
    };

    //A database object, contains methods used to interact with the database.
    class Database {
    public:
        //A simple inline constructor, essentially only here cause I disallow copy constructors.
        inline Database() {};

    public:
        /**
         * @brief Adds a student to the database.
         * @param refStudent The student object to add.
         * @return true If the student was successfully added to the database.
         * @return false If the student was not added to the database, possible a duplicate entry exists.
         */
        bool addStudent(const Student& refStudent);
        /**
         * @brief Adds a course to the database.
         * @param refCourse The course object to add.
         * @return true If the course was successfully added to the database.
         * @return false If the course was not added to the database, possible a duplicate entry exists.
         */
        bool addCourse(const Course& refCourse);
        /**
         * @brief Adds a term to the database.
         * @param refTerm The term object to add.
         * @return true If the term was successfully added to the database.
         * @return false If the term was not added to the database, possible a duplicate entry exists.
         */
        bool addTerm(const Term& refTerm);
        /**
         * @brief Adds a grade to the database.
         * @param refGrade The grade object to add.
         * @return true If the grade was successfully added to the database.
         * @return false If the grade was not added to the database, possible a duplicate entry exists.
         */
        bool addGrade(const Grade& refGrade);
        /**
         * @brief Adds an enrollment to a student into a particular course during a term with the grade.
         * @param refStudent The student object to add the enrollment to.
         * @param refCourse The course object that the student is being enrolled in.
         * @param refTerm The term in which the student took the course.
         * @param refGrade The grade the student earned in the course.
         * @return true If the enrollment was successfull.
         * @return false If the enrollment had failed, possible a duplicate entry exists.
         */
        bool addEnrollment(const Student& refStudent, const Course& refCourse, const Term& refTerm, const Grade& refGrade);

    public:
        /**
         * @brief Get the Student object
         * @param lpLastname The last name of the student.
         * @param lpFirstname The first name of the student.
         * @param lpStudent An optional pointer to a student object that will be filled with the student information.
         * @return true If the student was found (the optional pointer is filled only in this case).
         * @return false If the student doesn't exist.
         */
        bool getStudent(const char* lpLastname, const char* lpFirstname, Student* lpStudent);
        /**
         * @brief Get the Course object
         * 
         * @param lpPrefix The prefix for the course.
         * @param nCourseID The id number for the course.
         * @param lpCourse An optional pointer to a course object that will be filled with course information.
         * @return true If the course was found (the optional pointer is filled only in this case).
         * @return false If sthe course doesn't exist.
         */
        bool getCourse(const char* lpPrefix, unsigned int nCourseID, Course* lpCourse);
        /**
         * @brief Get the Term object
         * @param lpSemesterCode The semester code for the term.
         * @param lpTerm An optional pointer to a term object that will be filled with the term information.
         * @return true If the term was found (the optional pointer is filled only in this case).
         * @return false If the term doesn't exist.
         */
        bool getTerm(const char* lpSemesterCode, Term* lpTerm);
        /**
         * @brief Get the Grade object
         * @param lpGradeString The grade string for the grade.
         * @param lpGrade An optional pointer to a grade object that will be filled with the grade information.
         * @return true If the grade was found (the optional pointer is filled only in this case).
         * @return false If the grade doesn't exist.
         */
        bool getGrade(const char* lpGradeString, Grade* lpGrade);
        /**
         * @brief Checks to see if the student has been enrolled into a course during a specific term.
         * @param refStudent The student to check enrollment for.
         * @param refCourse The course to find.
         * @param refTerm The term in which to find the course in.
         * @return true If the student is already enrolled in the course during the specific term.
         * @return false If the student is not enrolled in the course during the specific term.
         */
        bool hasEnrollment(const Student& refStudent, const Course& refCourse, const Term& refTerm);

    public:
        //Lists all students in the database.
        void listStudents();
        //Lists all courses in the database.
        void listCourses();
        //Lists all terms in the database.
        void listTerms();
        //Lists all grades in the database.
        void listGrades();
        //Lists all enrollments in the database.
        void listEnrollments();
        /**
         * @brief Displays the transcript of the student with the given first/last name combo.
         * @param lpLastname The lastname of the student.
         * @param lpFirstname The firstname of the student.
         */
        void showTranscript(const char* lpLastname, const char* lpFirstname);

    public:
        /**
         * @brief Executes the database prompt which is responsible for prompting the user.
         * @param argc The number of arguments being passed.
         * @param argv The arguments being passed.
         * @return int The return code, Zero(0) on success.
         */
        int run(int argc, char** argv);

    public: //I don't like to enable copy constructors by default (mostly because pointers can be messed up).
        Database(const Database&) = delete;
        Database& operator=(const Database&) = delete;
    };

}

#endif//__AK_DATABASE_HPP__