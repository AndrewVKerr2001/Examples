#include <string.h>
#include "Common.hpp"
#include "Lock.hpp"
#include "Block.hpp"
#include <map>

#ifndef __AK_DATABASE_HPP__
#define __AK_DATABASE_HPP__

namespace AK
{

    class Database {
    private:
        LockGroup lockGroup;
        BlockFile blockFile;
        std::map<BlockIndex, Block*> mapBlocks;

    protected:
        Block& GetBlock(BlockIndex index);
        void Decache(BlockIndex index);

    public:
        Database(const char* lpFilename, const char* lpLockName);

    public:
        bool addStudent(const Student& refStudent);
        bool addCourse(const Course& refCourse);
        bool addTerm(const Term& refTerm);
        bool addGrade(const Grade& refGrade);
        bool addEnrollment(const Student& refStudent, const Course& refCourse, const Term& refTerm, const Grade& refGrade);

    public:
        bool getStudent(const char* lpLastname, const char* lpFirstname, Student* lpStudent);
        bool getCourse(const char* lpPrefix, unsigned int nCourseID, Course* lpCourse);
        bool getTerm(const char* lpSemesterCode, Term* lpTerm);
        bool getGrade(const char* lpGradeString, Grade* lpGrade);
        bool getEnrollment(const Student& refStudent, Enrollment* lpEnrollment);
        bool getEnrollment(const Course& refCourse, Enrollment* lpEnrollment);

    public:
        void listStudents();
        void listCourses();
        void listTerms();
        void listGrades();
        void listEnrollments();
        void showTranscript(const char* lpLastname, const char* lpFirstname);

    public:
        int run(int argc, char** argv);

    public:
        Database(const Database&) = delete;
        Database& operator=(const Database&) = delete;
    };

}

#endif//__AK_DATABASE_HPP__