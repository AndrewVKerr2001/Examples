#include <stdio.h>
#include <iostream>

#ifndef __AK_COMMON_HPP__
#define __AK_COMMON_HPP__

#define FATAL_MSG(msg) std::cerr << "[" << __FILE__ << ":" << __LINE__ << "|" << __FUNCTION__ << "] Fatal:" << std::endl << msg << std::endl << std::endl

#ifdef _DEBUG
#define ERROR_MSG(msg) std::cerr << "[" << __FILE__ << ":" << __LINE__ << "|" << __FUNCTION__ << "] Error:" << std::endl << msg << std::endl << std::endl
#define DEBUG_MSG(msg) std::cerr << "[" << __FILE__ << ":" << __LINE__ << "|" << __FUNCTION__ << "] Debug:" << std::endl << msg << std::endl << std::endl
#define VEBROSE_MSG(msg) std::cerr << "[" << __FILE__ << ":" << __LINE__ << "|" << __FUNCTION__ << "] Verbose:" << std::endl << msg << std::endl << std::endl
#define LOG_MSG(msg) std::cerr << "[" << __FILE__ << ":" << __LINE__ << "|" << __FUNCTION__ << "] Log:" << std::endl << msg << std::endl << std::endl
#else
#define ERROR_MSG(msg)
#define DEBUG_MSG(msg) 
#define VEBROSE_MSG(msg)
#define LOG_MSG(msg)
#endif

namespace AK {
    typedef unsigned int BlockIndex;

    class DatabaseObject {
    public:
        BlockIndex indexToAssociatedBlock;
    };

    class Student : public DatabaseObject {
    public:
        std::string strLastname;
        std::string strFirstname;
        std::string strPhoneNumber;
    };

    class Course : public DatabaseObject {
    public:
        std::string strPrefix;
        unsigned int nCourseNumber;
        std::string strDisplayName;
        unsigned int nCredits;
    };

    class Term : public DatabaseObject {
    public:
        //A semester code used to identify the term.
        std::string strSemesterCode;
        //The year in which the term takes place.
        unsigned int nYear;
        //A displayable name for the term, can be any length.
        std::string strDisplayName;
    };

    class Grade : public DatabaseObject {
    public:
        std::string strDisplayName;
        double nGrade;
    };

    class Enrollment : public DatabaseObject {
    public:
        Student oStudent;
        Course oCourse;
        Grade oGrade;
        Term oTerm;
    };
}

#endif//__AK_COMMON_HPP__