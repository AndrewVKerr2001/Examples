#include "Common.hpp"
#include "Lock.hpp"

#include <typeinfo>

#ifndef BLOCK_SIZE
#define BLOCK_SIZE 128
#endif//BLOCK_SIZE

#include <fstream>
#include <time.h>

#ifndef __AK_BLOCK_HPP__
#define __AK_BLOCK_HPP__

namespace AK
{

    class Block {
        friend class BlockFile;
    protected:
        //Make a block for memory and keep it aligned to a 1 byte boundary (possible to omit this but could misalign data).
        #pragma pack(push, 1)
        struct _BLOCK {
            struct Attributes {
                time_t firstCreated;
                time_t lastModified;
            } oAttributes;
            struct Data {
                char arrBytes[BLOCK_SIZE - sizeof(Attributes)];
            } oData;
        } oBlock;
        #pragma pack(pop)
        BlockIndex nIndex;

    public:
        //The exact size of each block's data allocated in a file.
        static constexpr size_t SizeOfData = sizeof(_BLOCK::Data);

    public:
        //Creates a block at the given index.
        Block(BlockIndex index);

    public:
        //The timestamp that identifies when this block was put into use.
        inline time_t firstCreationTime() { return this->oBlock.oAttributes.firstCreated; };
        //The timestamp that identifies when this block was last modified.
        inline time_t lastModifiedTime() { return this->oBlock.oAttributes.firstCreated; };
        template<typename T>
        T& data(){
            #ifdef  _DEBUG
            if(sizeof(T) > SizeOfData){
                FATAL_MSG("Failed to convert a block data section to '" << typeid(T).name() << "' due to the size exceeding " << SizeOfData << " in bytes, size of class is " << sizeof(T) << ".");
                exit(-1);
            }
            #endif//_DEBUG
            return *(T*)(&this->oBlock.oData);
        }

    protected:
        friend std::fstream& operator<<(std::fstream&, Block&);
        friend std::fstream& operator>>(std::fstream&, Block&);
    };

    class BlockFile {
    private:
        const char* lpFilename;
        LockGroup& refLockGroup;

    private:
        std::fstream oStream;
        Block oHeaderBlock;

    public:
        BlockFile(const char* lpFilename, LockGroup& refLockGroup);
        ~BlockFile();

    public:
        //Opens the underlying file for read/write, should only be doen by the function that aquires the lock.
        Block& Open();
        //Closes the underlying file, should only be done by the function that aquires the lock.
        void Close();

    public:
        /**
         * @brief Allocates a new block at the returned index within the file.
         * @return BlockIndex The index at which this allocated block resides.
         */
        BlockIndex AllocateBlock();
        /**
         * @brief Once called the following block, identified by 'index' will be removed from active use.
         * Essentially it will be added to the blank blocks list in the header. All other relationships must
         * be destroyed before this function is called.
         * @param index The index at which the block resides.
         */
        void FreeBlock(BlockIndex index);

    public:
        //Reads from the file and places the block's data into the given block.
        void ReadBlock(Block&);
        //Writes to the file and places the given block's data into the file.
        void WriteBlock(const Block&);
        //Validates a given Block using its timestamps; Returns True if the block is still considered valid, otherwise False.
        bool ValidateBlock(const Block&);

    public:
        BlockFile(const BlockFile&) = delete;
        BlockFile& operator=(const BlockFile&) = delete;
    };

    // template<unsigned int T>
    // struct ExtendedEntry {
    // public:
    //     unsigned int nSize;
    //     union {
    //         char arrData[T];
    //         BlockIndex firstBlockIndex;
    //     };

    // public:
    //     std::string GetBuffer(BlockFile& refFile) {
    //         if(nSize < T)
    //             return std::string(arrData);
    //         return refFile.GetCharacterBuffer(firstBlockIndex, nSize);
    //     }
    //     void SetBuffer(BlockFile& refFile, const char* lpBuffer, size_t nBufferSize) {
    //         if(nBufferSize < T) {
    //             for(size_t i = 0; i < nBufferSize; i++)
    //                 this->arrData[i] = lpBuffer[i];
    //         }
    //         else {
    //             Block tmp(firstBlockIndex = refFile.GetAvailable());
    //             size_t j = 0;
    //             for(size_t i = 0; i < ((nBufferSize + BLOCK_SIZE - (nBufferSize % BLOCK_SIZE)) / BLOCK_SIZE); i++){
    //                 if(i != 0){
    //                     tmp.oBlock.oAttributes.nextBlockIndex = refFile.GetAvailable(); //Get the next available block.
    //                     refFile.oStream << tmp;
    //                     tmp.nIndex = tmp.oBlock.oAttributes.nextBlockIndex;
    //                     time_t cached = tmp.oBlock.oAttributes.firstCreated;
    //                     ZeroMemory(&tmp.oBlock, sizeof(Block::_BLOCK));
    //                     tmp.oBlock.oAttributes.firstCreated = cached;
    //                 }
    //                 char* txt = tmp.oBlock.oData.arrBytes;
    //                 for(; j < nBufferSize && txt != tmp.oBlock.oData.arrBytes + sizeof(Block::_BLOCK::Data::arrBytes); j++){
    //                     if(*lpBuffer)
    //                         *txt++ = *lpBuffer++;
    //                     else break;
    //                 }
    //                 refFile.oStream << tmp;
    //             }
    //         }
    //         this->nSize = nBufferSize;
    //     }
    // };

    // namespace Blocks {

    //     struct Header {
    //     public:
    //         //A 4 character 'magic' string, if the string is not found in the Header block the file is not treated as a valid database.
    //         char magic[4];
    //         //A 4 byte version identifier, used to tell if the current database file is compatible with this program.
    //         unsigned int version;
    //         //The number of student entries, ie how many entries are currently in the linked list.
    //         size_t nStudentEntries;
    //         //The index of the first student block, NULL(0) means no students are in the file currently.
    //         BlockIndex firstStudentBlock;
    //         //The index of the first course block, NULL(0) means no courses are in the file currently.
    //         BlockIndex firstCourseBlock;
    //         //The index of the first term block, NULL(0) means no courses are in the file currently.
    //         BlockIndex firstTermBlock;
    //         //The index of the first grade block, NULL(0) means no courses are in the file currently.
    //         BlockIndex firstGradeBlock;
    //     };

    //     struct Student {
    //         //The full name of the student.
    //         ExtendedEntry<32> oFullName;
    //         //The phone number of the student.
    //         ExtendedEntry<18> oPhoneNumber;
    //         //An index to the first enrollment associated with the student.
    //         BlockIndex firstEnrollmentBlock;
    //     };

    //     struct Course {
    //         //A three letter course prefix (could be more but typically 3 letter's).
    //         ExtendedEntry<4> oPrefix;
    //         //The course number (positive number).
    //         unsigned int nCourseNumber;
    //         //A displayable name for the course, can be any length.
    //         ExtendedEntry<32> oDisplayName;
    //         //The number of credits this course is worth.
    //         unsigned int nCredits;
    //         //An index to the first enrollment array for the course, see CourseEnrollment struct for more info.
    //         BlockIndex firstCourseEnrollment;
    //     };

    //     struct CourseEnrollment {
    //         //An array of enrollments that belong to the associated course.
    //         BlockIndex arrIndexs[Block::SizeOfData / sizeof(BlockIndex)];
    //     };

    //     struct Term {
    //         //A semester code used to identify the term.
    //         ExtendedEntry<8> oSemesterCode;
    //         //The year in which the term takes place.
    //         unsigned int nYear;
    //         //A displayable name for the term, can be any length.
    //         ExtendedEntry<16> oDisplayName;
    //     };

    //     struct Grade {
    //         //A displayable name for the grade, can be any length.
    //         ExtendedEntry<64> oGradeIdentifier;
    //         //A floating point number used to identify the grade.
    //         double nGrade;
    //     };

    //     struct Enrollment {
    //         //An index back to the student associated with this Enrollment.
    //         BlockIndex studentBlock;
    //         //An index back to the course associated with this Enrollment.
    //         BlockIndex courseBlock;
    //         //An index to the term associated with this Enrollment.
    //         BlockIndex termBlock;
    //         //An index to the 'letter' grade associated with the numerical grade.
    //         BlockIndex gradeBlock;
    //     };

    // }
}

#endif//__AK_BLOCK_HPP__