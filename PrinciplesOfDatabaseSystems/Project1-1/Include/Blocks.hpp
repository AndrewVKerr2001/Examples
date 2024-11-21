#include "Common.hpp"

#ifndef __AK_BLOCKS_HPP__
#define __AK_BLOCKS_HPP__

namespace AK
{

    template<size_t T>
    struct ExtendedEntry {
        //The buffer for the entry, can be any size given by 'T'.
        char arrBuffer[T];
        //The index of the first overflow buffer.
        BlockIndex linkToOverflowBuffer;
    };

    namespace Blocks {
        
        struct Header {
        public:
            //The 4 character 'magic' string used to identify a DB file.
            static constexpr const char MAGIC[4] = {'A', 'K', 'D', 'B'};
            static constexpr unsigned int VERSION = 0x00000000;

        public:
            //A 4 character 'magic' string, if the string is not found in the Header block the file is not treated as a valid database.
            char magic[4];
            //A 4 byte version identifier, used to tell if the current database file is compatible with this program.
            unsigned int version;
            //The index of the first student block, NULL(0) means no students are in the file currently.
            BlockIndex firstStudentBlock;
            //The index of the first course block, NULL(0) means no courses are in the file currently.
            BlockIndex firstCourseBlock;
            //The index of the first term block, NULL(0) means no terms are in the file currently.
            BlockIndex firstTermBlock;
            //The index of the first grade block, NULL(0) means no grades are in the file currently.
            BlockIndex firstGradeBlock;
            //The index of the first blank block, NULL(0) means no unused blocks are in the file currently.
            BlockIndex firstBlankBlock;
        };

        struct Blank {
        public:
            //An index to the next blank block in the chain, NULL(0) means no block is next.
            BlockIndex nextBlankBlock;
        }

        struct Student {
            //The full name of the student.
            ExtendedEntry<32> oFullName;
            //The phone number of the student.
            ExtendedEntry<18> oPhoneNumber;
            //An index to the first enrollment associated with the student.
            BlockIndex firstEnrollmentBlock;
        };

        struct Course {
            //A three letter course prefix (could be more but typically 3 letter's).
            ExtendedEntry<4> oPrefix;
            //The course number (positive number).
            unsigned int nCourseNumber;
            //A displayable name for the course, can be any length.
            ExtendedEntry<32> oDisplayName;
            //The number of credits this course is worth.
            unsigned int nCredits;
            //An index to the first enrollment array for the course, see CourseEnrollment struct for more info.
            BlockIndex firstCourseEnrollment;
        };

        struct Term {
            //A semester code used to identify the term.
            ExtendedEntry<8> oSemesterCode;
            //The year in which the term takes place.
            unsigned int nYear;
            //A displayable name for the term, can be any length.
            ExtendedEntry<16> oDisplayName;
        };

        struct Grade {
            //A displayable name for the grade, can be any length.
            ExtendedEntry<64> oGradeIdentifier;
            //A floating point number used to identify the grade.
            double nGrade;
        };

        struct Enrollment {
            //An index back to the student associated with this Enrollment.
            BlockIndex studentBlock;
            //An index back to the course associated with this Enrollment.
            BlockIndex courseBlock;
            //An index to the term associated with this Enrollment.
            BlockIndex termBlock;
            //An index to the 'letter' grade associated with the numerical grade.
            BlockIndex gradeBlock;
            //An index to the next enrollment associated with the student.
            BlockIndex nextStudentEnrollment;
            //An index to the next enrollment associated with the course.
            BlockIndex nextCourseEnrollment;
        };

    }
}

#endif//__AK_BLOCKS_HPP__