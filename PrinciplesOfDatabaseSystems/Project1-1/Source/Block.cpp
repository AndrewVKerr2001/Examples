#include "Block.hpp"
#include "Blocks.hpp"
#include <sstream>

#include <sys/types.h>
#include <sys/stat.h>

#ifndef __WIN32__
#include <unistd.h>
#else
#define stat _stat
#endif

namespace AK
{

    Block::Block(BlockIndex index) : nIndex(index) {
        ZeroMemory(&oBlock, sizeof(_BLOCK));
        { // Set the current time, only valid if written to the file prior to read.
            time_t current;
            time(&current);
            this->oBlock.oAttributes.firstCreated = current;
        }
    }

    std::fstream& operator<<(std::fstream& f, Block& b) {
        if(f.is_open()){
            { //Update the time of last modification to reflect the current time.
                time_t current;
                time(&current);
                b.oBlock.oAttributes.lastModified = current;
            }

            f.seekg(b.nIndex * BLOCK_SIZE);
            f.write((const char*)&b.oBlock, sizeof(Block::_BLOCK));
            f.flush();
        }
        return f;
    }

    std::fstream& operator>>(std::fstream& f, Block& b){
        if(f.is_open()){
            f.seekg(b.nIndex * BLOCK_SIZE);
            f.read((char*)&b.oBlock, sizeof(Block::_BLOCK));
        }
        return f;   
    }

    BlockFile::BlockFile(const char* lpFilename, LockGroup& refLockGroup) : lpFilename(lpFilename), refLockGroup(refLockGroup), oStream(), oHeaderBlock(0) {}

    BlockFile::~BlockFile() {

    }

    Block& BlockFile::Open() {
        //Close the existing stream.
        if(this->oStream.is_open())
            this->oStream.close();

        //Attempt to get the file information, only succeeds if the file exists and is accessable.
        struct stat fInfo;
        if(stat(lpFilename, &fInfo)==0){ //File exists!
            this->oStream.open(lpFilename, std::ios::binary | std::ios::out | std::ios::in);
            if(!this->oStream.is_open()){
                ERROR_MSG("Failed to open the database file!");
                exit(E_ACCESSDENIED);
            }

            { // Read the header information from the file.                
                this->oStream >> oHeaderBlock;

                Blocks::Header& header = oHeaderBlock.data<Blocks::Header>();
                if(*((int*)header.magic) != *((int*)Blocks::Header::MAGIC)) {
                    ERROR_MSG("The database magic doesn't match, delete the current database and restart!");
                    exit(E_ACCESSDENIED);
                }

                if(header.version != Blocks::Header::VERSION) {
                    ERROR_MSG("The database version doesn't match, delete the current database and restart!");
                    exit(E_ACCESSDENIED);
                }
            }
        }
        else { //File doesn't exist yet!
            this->oStream.open(lpFilename, std::ios::binary | std::ios::out);
            if(!this->oStream.is_open()){
                ERROR_MSG("Failed to create the database file!");
                exit(E_ACCESSDENIED);
            }

            { // Write the header information to the file.
                time_t current;
                time(&current);

                //Setup the block data.
                oHeaderBlock.oBlock.oAttributes.firstCreated = current;

                //Update the data section to contain our header information.
                Blocks::Header& header = oHeaderBlock.data<Blocks::Header>();
                memcpy_s(header.magic, 4, Blocks::Header::MAGIC, 4);
                header.version = Blocks::Header::VERSION;
                header.firstStudentBlock = 0;
                header.firstGradeBlock = 0;
                header.firstCourseBlock = 0;
                header.firstTermBlock = 0;
                header.firstBlankBlock = 0;

                //Write to the stream with the block.
                this->oStream << oHeaderBlock;
            }

            //Close the stream and reopen in both read and write mode.
            this->oStream.close();
            this->oStream.open(lpFilename, std::ios::binary | std::ios::out | std::ios::in);
            if(!this->oStream.is_open()){
                ERROR_MSG("Failed to open the database file after creation.");
                exit(E_ACCESSDENIED);
            }
        }
        return oHeaderBlock;
    }

    void BlockFile::Close() {
        if(this->oStream.is_open())
            this->oStream.close();
    }

    BlockIndex BlockFile::GetAvailable() {
        //NOTE: This function was used to allow for the ability to deallocate some blocks in the file and then
        //      reallocate them when this function is called. As we don't delete anything as of yet we can just
        //      keep appending new blocks to the end of the file.
        if(this->oStream.is_open()){
            this->oStream.seekg(0, std::ios::end);
            return (BlockIndex)(this->oStream.tellg() / BLOCK_SIZE);
        }
        return 0;
    }

    BlockIndex BlockFile::AllocateBlock() { 
        Lock lock(this->refLockGroup);
        if(lock.isInitialLock())
            this->Open();

        if(!this->ValidateBlock(this->oHeaderBlock))
            this->ReadBlock(this->oHeaderBlock);

        auto& header = this->oHeaderBlock.data<Blocks::Header>();
        auto& allocated_index = header.firstBlankBlock;
        if(allocated_index) {
            Block tmp(allocated_index);
            this->ReadBlock(allocated_index);
            auto& blank = tmp.data<Blocks::Blank>();
            header.firstBlankBlock = blank.nextBlankBlock;
            this->WriteBlock(this->oHeaderBlock);
        }
        else {
            this->oStream.seekg(0, std::ios::end);
            Block tmp(allocated_index = (BlockIndex)(this->oStream.tellg() / BLOCK_SIZE));
            this->WriteBlock(tmp);
        }

        if(lock.isInitialLock())
            this->Close();
        return allocated_index;
    }


    void BlockFile::FreeBlock(BlockIndex index) {
        Lock lock(this->refLockGroup);
        if(lock.isInitialLock())
            this->Open();

        if(!this->ValidateBlock(this->oHeaderBlock))
            this->ReadBlock(this->oHeaderBlock);

        auto& header = this->oHeaderBlock.data<Blocks::Header>();

        Block tmp(index);
        this->ReadBlock(tmp);
        tmp.data<Blocks::Blank>().nextBlankBlock = header.firstBlankBlock;
        this->WriteBlock(tmp);
        header.firstBlankBlock = index;
        this->WriteBlock(this->oHeaderBlock);

        if(lock.isInitialLock())
            this->Close();
    }

    //Reads from the file and places the block's data into the given block.
    void BlockFile::ReadBlock(Block& refBlock) {
        this->oStream >> refBlock;
    }

    //Writes to the file and places the given block's data into the file.
    void BlockFile::WriteBlock(const Block& refBlock) {
        this->oStream << refBlock;
    }

    //Validates a given Block using its timestamps; Returns True if the block is still considered valid, otherwise False.
    bool BlockFile::ValidateBlock(const Block& refBlock) {
        Block::_BLOCK::Attributes attribs;
        this->oStream.seekg(refBlock.nIndex * BLOCK_SIZE);
        this->oStream.read((char*)&attribs, sizeof(attribs));
        
        if(refBlock.oBlock.oAttributes.firstCreated != attribs.firstCreated){
            ERROR_MSG("While validating a block the firstCreated timestamps didn't match, possible that some code isn't reading prior to writing blocks.");
        }

        return (refBlock.oBlock.oAttributes.lastModified == attribs.lastModified);
    }


    // int compare(const char* a, const char* b){
    //     size_t i = 0;
    //     for(; a[i] && b[i] && a[i] == b[i]; i++);
    //     return a[i] > b[i];
    // }

    // bool BlockFile::GetStudent(const char* lpFullName, Student* lpStudent) {
    //     Lock lock(this->refLockGroup);
    //     if(lock.isOriginalAcquirer()){
    //         //Since we are the first 'lock' we need to ensure we have a clean slate to work with.
    //         this->Open();
    //         if(lpStudent) lpStudent->indexToAssociatedBlock = 0;
    //     }

    //     Blocks::Header& header = oHeaderBlock.convert<Blocks::Header>();
    //     if(header.firstStudentBlock){
    //         Block tmp(header.firstStudentBlock); tmp.oBlock.oAttributes.nextBlockIndex = tmp.nIndex;
    //         Blocks::Student& student = tmp.convert<Blocks::Student>();
    //         do {
    //             //Read in the next student block.
    //             tmp.nIndex = tmp.oBlock.oAttributes.nextBlockIndex;
    //             this->oStream >> tmp;
    //             std::string strFullName = student.oFullName.GetBuffer(*this);

    //             //Check if we have a 1:1 match or not.
    //             //std::cout << "Comparing '" << strFullName << "' with '" << lpFullName << "'\n";
    //             if(strFullName == lpFullName){
    //                 if(lpStudent){
    //                     lpStudent->strFullName = move(strFullName);
    //                     lpStudent->strPhoneNumber = move(student.oPhoneNumber.GetBuffer(*this));
    //                     if(!lock.isOriginalAcquirer()) lpStudent->indexToAssociatedBlock = tmp.nIndex;
    //                 }

    //                 //Release our file since we aren't using it anymore.
    //                 if(lock.isOriginalAcquirer())
    //                     this->Close();
    //                 return true;
    //             }
    //             else if(compare(strFullName.c_str(), lpFullName)) break;
    //         } while(tmp.oBlock.oAttributes.nextBlockIndex);
    //     }

    //     //Release our file since we aren't using it anymore.
    //     if(lock.isOriginalAcquirer())
    //         this->Close();
    //     return false;
    // }

    // void BlockFile::SetStudent(const Student& refStudent) {
    //     Lock lock(this->refLockGroup);
    //     if(lock.isOriginalAcquirer()){
    //         //Since we are the first 'lock' we need to ensure we have a clean slate to work with.
    //         this->Open();
    //     }
    //     { //Check if we have a student already, if we do lets bail.
    //         if(GetStudent(refStudent.strFullName.c_str(), NULL)) return;
    //     }

    //     //std::cout << "Attempting to add \"" << lpFullName << "\" to the list." << std::endl; 

    //     Block tmp(GetAvailable());
    //     if(!tmp.nIndex){
    //         ERROR_MSG("Failed to aquire a new block to place a student object in.");
    //         exit(E_ABORT);
    //     }
    //     this->oStream << tmp;

    //     auto& student = tmp.convert<Blocks::Student>();
    //     student.oFullName.SetBuffer(*this, refStudent.strFullName.c_str(), refStudent.strFullName.length());
    //     student.oPhoneNumber.SetBuffer(*this, refStudent.strPhoneNumber.c_str(), refStudent.strPhoneNumber.length());

    //     this->oStream << tmp;
        
    //     Blocks::Header& header = oHeaderBlock.convert<Blocks::Header>();
    //     if(header.firstStudentBlock){
    //         Block prv(0);
    //         Block crt(0);
    //         crt.oBlock.oAttributes.nextBlockIndex = header.firstStudentBlock;

    //         auto& crt_student = crt.convert<Blocks::Student>();
    //         do {
    //             prv = crt;
    //             crt.nIndex = crt.oBlock.oAttributes.nextBlockIndex;
    //             //Read in the next student block.
    //             this->oStream >> crt;
    //             std::string strFullName = crt_student.oFullName.GetBuffer(*this);

    //             //Check if this entry is past our current entry, if it is we have found our new placement.
    //             if(compare(strFullName.c_str(), refStudent.strFullName.c_str())) { //todo: Check if the logic needs to be inverted?
    //                 //std::cout << "Inserted '" << lpFullName << "' before '" << strFullName << "'.\n";
    //                 if(prv.nIndex) {
    //                     prv.oBlock.oAttributes.nextBlockIndex = tmp.nIndex;
    //                     tmp.oBlock.oAttributes.nextBlockIndex = crt.nIndex;
    //                     this->oStream << prv;
    //                     this->oStream << tmp;
    //                 }
    //                 else { //We are messing with the header, be careful...
    //                     header.firstStudentBlock = tmp.nIndex; //This should allow us to remedy this issue.
    //                     tmp.oBlock.oAttributes.nextBlockIndex = crt.nIndex;
    //                     this->oStream << oHeaderBlock;
    //                     this->oStream << tmp;
    //                 }
    //                 break;
    //             }
    //         } while(prv.oBlock.oAttributes.nextBlockIndex);
    //         if(prv.oBlock.oAttributes.nextBlockIndex == 0){
    //             //std::cout << "Appended '" << lpFullName << "' to the end of the list.\n";
    //             prv.oBlock.oAttributes.nextBlockIndex = tmp.nIndex;
    //             this->oStream << prv;
    //         }
    //     }
    //     else {
    //         header.firstStudentBlock = tmp.nIndex;
    //         this->oStream << oHeaderBlock;
    //     }

    //     if(lock.isOriginalAcquirer()){
    //         //Since we are the first 'lock' we need to ensure we have a clean slate to work with.
    //         this->Close();
    //     }
    // }

    // void BlockFile::ListStudents() {
    //     Lock lock(this->refLockGroup);
    //     if(lock.isOriginalAcquirer()){
    //         //Since we are the first 'lock' we need to ensure we have a clean slate to work with.
    //         this->Open();
    //     }

    //     Blocks::Header& header = oHeaderBlock.convert<Blocks::Header>();
    //     if(header.firstStudentBlock){
    //         Block tmp(header.firstStudentBlock); tmp.oBlock.oAttributes.nextBlockIndex = tmp.nIndex;
    //         Blocks::Student& student = tmp.convert<Blocks::Student>();
    //         do {
    //             //Read in the next student block.
    //             tmp.nIndex = tmp.oBlock.oAttributes.nextBlockIndex;
    //             this->oStream >> tmp;
    //             std::string strFullName = student.oFullName.GetBuffer(*this);
    //             std::string strPhoneNumber = student.oPhoneNumber.GetBuffer(*this);
    //             std::cout << strFullName << " " << strPhoneNumber << std::endl;
    //         } while(tmp.oBlock.oAttributes.nextBlockIndex);
    //     }

    //     if(lock.isOriginalAcquirer()){
    //         //Since we are the first 'lock' we need to ensure we have a clean slate to work with.
    //         this->Close();
    //     }
    // }

    // bool BlockFile::GetCourse(const char* lpPrefix, unsigned int nCourse, Course* lpCourse) {
    //     Lock lock(this->refLockGroup);
    //     if(lock.isOriginalAcquirer()){
    //         //Since we are the first 'lock' we need to ensure we have a clean slate to work with.
    //         this->Open();
    //         if(lpCourse) lpCourse->indexToAssociatedBlock = 0;
    //     }

    //     Blocks::Header& header = oHeaderBlock.convert<Blocks::Header>();
    //     if(header.firstCourseBlock){
    //         Block tmp(header.firstCourseBlock); tmp.oBlock.oAttributes.nextBlockIndex = tmp.nIndex;
    //         Blocks::Course& course = tmp.convert<Blocks::Course>();
    //         do {
    //             //Read in the next course block.
    //             tmp.nIndex = tmp.oBlock.oAttributes.nextBlockIndex;
    //             this->oStream >> tmp;
    //             std::string strPrefix = course.oPrefix.GetBuffer(*this);

    //             //Check if we have a 1:1 match or not.
    //             //std::cout << "Comparing '" << strPrefix << "' with '" << lpPrefix << "'\n";
    //             if(strPrefix == lpPrefix){
    //                 //Check if the course number is the same.
    //                 if(course.nCourseNumber == nCourse){
    //                     if(lpCourse){
    //                         lpCourse->strPrefix = move(strPrefix);
    //                         lpCourse->nCourseNumber = course.nCourseNumber;
    //                         lpCourse->nCredits = course.nCredits;
    //                         lpCourse->strDisplayName = move(course.oDisplayName.GetBuffer(*this));
    //                         if(!lock.isOriginalAcquirer()) lpCourse->indexToAssociatedBlock = tmp.nIndex;
    //                     }

    //                     //Release our file since we aren't using it anymore.
    //                     if(lock.isOriginalAcquirer())
    //                         this->Close();
    //                     return true;
    //                 }
    //             }
    //             else if(compare(strPrefix.c_str(), lpPrefix)) break;
    //         } while(tmp.oBlock.oAttributes.nextBlockIndex);
    //     }

    //     //Release our file since we aren't using it anymore.
    //     if(lock.isOriginalAcquirer())
    //         this->Close();
    //     return false;
    // }

    
    // void BlockFile::SetCourse(const Course& refCourse) {
    //     Lock lock(this->refLockGroup);
    //     if(lock.isOriginalAcquirer()){
    //         //Since we are the first 'lock' we need to ensure we have a clean slate to work with.
    //         this->Open();
    //     }
    //     { //Check if we have a course already, if we do lets bail.
    //         if(GetCourse(refCourse.strPrefix.c_str(), refCourse.nCourseNumber, NULL)) return;
    //     }

    //     //std::cout << "Attempting to add \"" << lpFullName << "\" to the list." << std::endl; 
    //     Block tmp(GetAvailable());
    //     if(!tmp.nIndex){
    //         ERROR_MSG("Failed to aquire a new block to place a student object in.");
    //         exit(E_ABORT);
    //     }
    //     this->oStream << tmp;

    //     auto& course = tmp.convert<Blocks::Course>();
    //     course.nCredits = refCourse.nCredits;
    //     course.nCourseNumber = refCourse.nCourseNumber;
    //     course.oDisplayName.SetBuffer(*this, refCourse.strDisplayName.c_str(), refCourse.strDisplayName.length());
    //     course.oPrefix.SetBuffer(*this, refCourse.strPrefix.c_str(), refCourse.strPrefix.length());
    //     this->oStream << tmp;
        
    //     Blocks::Header& header = oHeaderBlock.convert<Blocks::Header>();
    //     if(header.firstCourseBlock){
    //         Block prv(0);
    //         Block crt(0);
    //         crt.oBlock.oAttributes.nextBlockIndex = header.firstCourseBlock;

    //         auto& crt_course = crt.convert<Blocks::Course>();
    //         do {
    //             prv = crt;
    //             crt.nIndex = crt.oBlock.oAttributes.nextBlockIndex;
    //             //Read in the next student block.
    //             this->oStream >> crt;
    //             std::string strPrefix = crt_course.oPrefix.GetBuffer(*this);

    //             //Check if this entry is past our current entry, if it is we have found our new placement.
    //             if(compare(strPrefix.c_str(), refCourse.strPrefix.c_str()) || (strPrefix == refCourse.strPrefix && crt_course.nCourseNumber > refCourse.nCourseNumber)) { //todo: Check if the logic needs to be inverted?
    //                 //std::cout << "Inserted '" << lpFullName << "' before '" << strFullName << "'.\n";
    //                 if(prv.nIndex != 0) { //Check if we found the head block, ie we are at the header, gotta be careful here.
    //                     prv.oBlock.oAttributes.nextBlockIndex = tmp.nIndex;
    //                     tmp.oBlock.oAttributes.nextBlockIndex = crt.nIndex;
    //                     this->oStream << prv;
    //                     this->oStream << tmp;
    //                 }
    //                 else { //We are messing with the header, be careful...
    //                     header.firstCourseBlock = tmp.nIndex; //This should allow us to remedy this issue.
    //                     tmp.oBlock.oAttributes.nextBlockIndex = crt.nIndex;
    //                     this->oStream << oHeaderBlock;
    //                     this->oStream << tmp;
    //                 }
    //                 break;
    //             }
    //         } while(prv.oBlock.oAttributes.nextBlockIndex);
    //         if(prv.oBlock.oAttributes.nextBlockIndex == 0){
    //             //std::cout << "Appended '" << lpFullName << "' to the end of the list.\n";
    //             prv.oBlock.oAttributes.nextBlockIndex = tmp.nIndex;
    //             this->oStream << prv;
    //         }
    //     }
    //     else {
    //         header.firstCourseBlock = tmp.nIndex;
    //         this->oStream << oHeaderBlock;
    //     }

    //     if(lock.isOriginalAcquirer()){
    //         //Since we are the first 'lock' we need to ensure we have a clean slate to work with.
    //         this->Close();
    //     }
    // }

    // void BlockFile::ListCourses() {
    //     Lock lock(this->refLockGroup);
    //     if(lock.isOriginalAcquirer()){
    //         //Since we are the first 'lock' we need to ensure we have a clean slate to work with.
    //         this->Open();
    //     }

    //     Blocks::Header& header = oHeaderBlock.convert<Blocks::Header>();
    //     if(header.firstCourseBlock){
    //         Block tmp(header.firstCourseBlock); tmp.oBlock.oAttributes.nextBlockIndex = tmp.nIndex;
    //         Blocks::Course& course = tmp.convert<Blocks::Course>();
    //         do {
    //             //Read in the next student block.
    //             tmp.nIndex = tmp.oBlock.oAttributes.nextBlockIndex;
    //             this->oStream >> tmp;
    //             std::string strPrefix = course.oPrefix.GetBuffer(*this);
    //             std::string strDisplayName = course.oDisplayName.GetBuffer(*this);
    //             std::cout << strPrefix << " " << course.nCourseNumber << " " << strDisplayName << " " << course.nCredits << std::endl;
    //         } while(tmp.oBlock.oAttributes.nextBlockIndex);
    //     }

    //     if(lock.isOriginalAcquirer()){
    //         //Since we are the first 'lock' we need to ensure we have a clean slate to work with.
    //         this->Close();
    //     }
    // }

    // bool BlockFile::GetTerm(const char* lpSemesterCode, Term* lpTerm) {
    //     Lock lock(this->refLockGroup);
    //     if(lock.isOriginalAcquirer()){
    //         //Since we are the first 'lock' we need to ensure we have a clean slate to work with.
    //         this->Open();
    //         if(lpTerm) lpTerm->indexToAssociatedBlock = 0;
    //     }

    //     Blocks::Header& header = oHeaderBlock.convert<Blocks::Header>();
    //     if(header.firstTermBlock){
    //         Block tmp(header.firstTermBlock); tmp.oBlock.oAttributes.nextBlockIndex = tmp.nIndex;
    //         Blocks::Term& term = tmp.convert<Blocks::Term>();
    //         do {
    //             //Read in the next course block.
    //             tmp.nIndex = tmp.oBlock.oAttributes.nextBlockIndex;
    //             this->oStream >> tmp;
    //             std::string strSemesterCode = term.oSemesterCode.GetBuffer(*this);

    //             //Check if we have a 1:1 match or not.
    //             //std::cout << "Comparing '" << strPrefix << "' with '" << lpPrefix << "'\n";
    //             if(strSemesterCode == lpSemesterCode){
    //                 if(lpTerm){ //Copy over if we have an object to dump data into.
    //                     lpTerm->nYear = term.nYear;
    //                     lpTerm->strDisplayName = term.oDisplayName.GetBuffer(*this);
    //                     lpTerm->strSemesterCode = move(strSemesterCode);
    //                     if(!lock.isOriginalAcquirer()) lpTerm->indexToAssociatedBlock = tmp.nIndex;
    //                 }
    //                 //Release our file since we aren't using it anymore.
    //                 if(lock.isOriginalAcquirer())
    //                     this->Close();
    //                 return true;
    //             }
    //             else if(compare(strSemesterCode.c_str(), lpSemesterCode)) break;
    //         } while(tmp.oBlock.oAttributes.nextBlockIndex);
    //     }

    //     //Release our file since we aren't using it anymore.
    //     if(lock.isOriginalAcquirer())
    //         this->Close();
    //     return false;
    // }

    // void BlockFile::SetTerm(const Term& refTerm) {
    //     Lock lock(this->refLockGroup);
    //     if(lock.isOriginalAcquirer()){
    //         //Since we are the first 'lock' we need to ensure we have a clean slate to work with.
    //         this->Open();
    //     }
    //     { //Check if we have a term already, if we do lets bail.
    //         if(GetTerm(refTerm.strSemesterCode.c_str(), NULL)) return;
    //     }

    //     Block tmp(GetAvailable());
    //     if(!tmp.nIndex){
    //         ERROR_MSG("Failed to aquire a new block to place a student object in.");
    //         exit(E_ABORT);
    //     }
    //     this->oStream << tmp;

    //     auto& term = tmp.convert<Blocks::Term>();
    //     term.nYear = refTerm.nYear;
    //     term.oDisplayName.SetBuffer(*this, refTerm.strDisplayName.c_str(), refTerm.strDisplayName.length());
    //     term.oSemesterCode.SetBuffer(*this, refTerm.strSemesterCode.c_str(), refTerm.strSemesterCode.length());
    //     this->oStream << tmp;
        
    //     Blocks::Header& header = oHeaderBlock.convert<Blocks::Header>();
    //     if(header.firstTermBlock){
    //         Block prv(0);
    //         Block crt(0);
    //         crt.oBlock.oAttributes.nextBlockIndex = header.firstTermBlock;

    //         auto& term = crt.convert<Blocks::Term>();
    //         do {
    //             prv = crt;
    //             crt.nIndex = crt.oBlock.oAttributes.nextBlockIndex;
    //             //Read in the next student block.
    //             this->oStream >> crt;
    //             std::string strSemesterCode = term.oSemesterCode.GetBuffer(*this);

    //             //Check if this entry is past our current entry, if it is we have found our new placement.
    //             if(compare(strSemesterCode.c_str(), refTerm.strSemesterCode.c_str())) { //todo: Check if the logic needs to be inverted?
    //                 //std::cout << "Inserted '" << lpFullName << "' before '" << strFullName << "'.\n";
    //                 if(prv.nIndex != 0) { //Check if we found the head block, ie we are at the header, gotta be careful here.
    //                     prv.oBlock.oAttributes.nextBlockIndex = tmp.nIndex;
    //                     tmp.oBlock.oAttributes.nextBlockIndex = crt.nIndex;
    //                     this->oStream << prv;
    //                     this->oStream << tmp;
    //                 }
    //                 else { //We are messing with the header, be careful...
    //                     header.firstTermBlock = tmp.nIndex; //This should allow us to remedy this issue.
    //                     tmp.oBlock.oAttributes.nextBlockIndex = crt.nIndex;
    //                     this->oStream << oHeaderBlock;
    //                     this->oStream << tmp;
    //                 }
    //                 break;
    //             }
    //         } while(prv.oBlock.oAttributes.nextBlockIndex);
    //         if(prv.oBlock.oAttributes.nextBlockIndex == 0){
    //             prv.oBlock.oAttributes.nextBlockIndex = tmp.nIndex;
    //             this->oStream << prv;
    //         }
    //     }
    //     else {
    //         header.firstTermBlock = tmp.nIndex;
    //         this->oStream << oHeaderBlock;
    //     }

    //     if(lock.isOriginalAcquirer()){
    //         //Since we are the first 'lock' we need to ensure we have a clean slate to work with.
    //         this->Close();
    //     }
    // }

    // void BlockFile::ListTerms() {
    //     Lock lock(this->refLockGroup);
    //     if(lock.isOriginalAcquirer()){
    //         //Since we are the first 'lock' we need to ensure we have a clean slate to work with.
    //         this->Open();
    //     }

    //     Blocks::Header& header = oHeaderBlock.convert<Blocks::Header>();
    //     if(header.firstTermBlock){
    //         Block tmp(header.firstTermBlock); tmp.oBlock.oAttributes.nextBlockIndex = tmp.nIndex;
    //         Blocks::Term& term = tmp.convert<Blocks::Term>();
    //         do {
    //             //Read in the next student block.
    //             tmp.nIndex = tmp.oBlock.oAttributes.nextBlockIndex;
    //             this->oStream >> tmp;
    //             std::cout << term.oSemesterCode.GetBuffer(*this) << " " << term.nYear << " " << term.oDisplayName.GetBuffer(*this) << std::endl;
    //         } while(tmp.oBlock.oAttributes.nextBlockIndex);
    //     }

    //     if(lock.isOriginalAcquirer()){
    //         //Since we are the first 'lock' we need to ensure we have a clean slate to work with.
    //         this->Close();
    //     }
    // }

    
    // bool BlockFile::GetGrade(const char* lpGradeString, Grade* lpGrade) {
    //     Lock lock(this->refLockGroup);
    //     if(lock.isOriginalAcquirer()){
    //         //Since we are the first 'lock' we need to ensure we have a clean slate to work with.
    //         this->Open();
    //         if(lpGrade) lpGrade->indexToAssociatedBlock = 0;
    //     }

    //     Blocks::Header& header = oHeaderBlock.convert<Blocks::Header>();
    //     if(header.firstGradeBlock){
    //         Block tmp(header.firstGradeBlock); tmp.oBlock.oAttributes.nextBlockIndex = tmp.nIndex;
    //         Blocks::Grade& grade = tmp.convert<Blocks::Grade>();
    //         do {
    //             //Read in the next course block.
    //             tmp.nIndex = tmp.oBlock.oAttributes.nextBlockIndex;
    //             this->oStream >> tmp;

    //             std::string strGradeIdentifier = grade.oGradeIdentifier.GetBuffer(*this);

    //             //Check if we have a 1:1 match or not.
    //             //std::cout << "Comparing '" << strPrefix << "' with '" << lpPrefix << "'\n";
    //             if(strGradeIdentifier == lpGradeString){
    //                 if(lpGrade){ //Copy over if we have an object to dump data into.
    //                     lpGrade->nGrade = grade.nGrade;
    //                     lpGrade->strDisplayName = move(strGradeIdentifier);
    //                     if(!lock.isOriginalAcquirer()) lpGrade->indexToAssociatedBlock = tmp.nIndex;
    //                 }
    //                 //Release our file since we aren't using it anymore.
    //                 if(lock.isOriginalAcquirer())
    //                     this->Close();
    //                 return true;
    //             }
    //             else if(compare(strGradeIdentifier.c_str(), lpGradeString)) break;
    //         } while(tmp.oBlock.oAttributes.nextBlockIndex);
    //     }

    //     //Release our file since we aren't using it anymore.
    //     if(lock.isOriginalAcquirer())
    //         this->Close();
    //     return false;
    // }

    // void BlockFile::SetGrade(const Grade& refGrade) {
    //     Lock lock(this->refLockGroup);
    //     if(lock.isOriginalAcquirer()){
    //         //Since we are the first 'lock' we need to ensure we have a clean slate to work with.
    //         this->Open();
    //     }
    //     { //Check if we have a grade already, if we do lets bail.
    //         if(GetGrade(refGrade.strDisplayName.c_str(), NULL)) return;
    //     }

    //     Block tmp(GetAvailable());
    //     if(!tmp.nIndex){
    //         ERROR_MSG("Failed to aquire a new block to place a grade object in.");
    //         exit(E_ABORT);
    //     }
    //     this->oStream << tmp;

    //     auto& grade = tmp.convert<Blocks::Grade>();
    //     grade.nGrade = refGrade.nGrade;
    //     grade.oGradeIdentifier.SetBuffer(*this, refGrade.strDisplayName.c_str(), refGrade.strDisplayName.length());
    //     this->oStream << tmp;
        
    //     Blocks::Header& header = oHeaderBlock.convert<Blocks::Header>();
    //     if(header.firstGradeBlock){
    //         Block prv(0);
    //         Block crt(0);
    //         crt.oBlock.oAttributes.nextBlockIndex = header.firstGradeBlock;

    //         auto& grade = crt.convert<Blocks::Grade>();
    //         do {
    //             prv = crt;
    //             crt.nIndex = crt.oBlock.oAttributes.nextBlockIndex;
    //             //Read in the next student block.
    //             this->oStream >> crt;

    //             std::string strGradeIdentifier = grade.oGradeIdentifier.GetBuffer(*this);

    //             //Check if this entry is past our current entry, if it is we have found our new placement.
    //             if(compare(strGradeIdentifier.c_str(), refGrade.strDisplayName.c_str())) { //todo: Check if the logic needs to be inverted?
    //                 //std::cout << "Inserted '" << lpFullName << "' before '" << strFullName << "'.\n";
    //                 if(prv.nIndex) { //Check if we found the head block, ie we are at the header, gotta be careful here.
    //                     prv.oBlock.oAttributes.nextBlockIndex = tmp.nIndex;
    //                     tmp.oBlock.oAttributes.nextBlockIndex = crt.nIndex;
    //                     this->oStream << prv;
    //                     this->oStream << tmp;
    //                 }
    //                 else { //We are messing with the header, be careful...
    //                     header.firstGradeBlock = tmp.nIndex; //This should allow us to remedy this issue.
    //                     tmp.oBlock.oAttributes.nextBlockIndex = crt.nIndex;
    //                     this->oStream << oHeaderBlock;
    //                     this->oStream << tmp;
    //                 }
    //                 break;
    //             }
    //         } while(prv.oBlock.oAttributes.nextBlockIndex);
    //         if(prv.oBlock.oAttributes.nextBlockIndex == 0){
    //             prv.oBlock.oAttributes.nextBlockIndex = tmp.nIndex;
    //             this->oStream << prv;
    //         }
    //     }
    //     else {
    //         header.firstGradeBlock = tmp.nIndex;
    //         this->oStream << oHeaderBlock;
    //     }

    //     if(lock.isOriginalAcquirer()){
    //         //Since we are the first 'lock' we need to ensure we have a clean slate to work with.
    //         this->Close();
    //     }
    // }

    // void BlockFile::ListGrades() {
    //     Lock lock(this->refLockGroup);
    //     if(lock.isOriginalAcquirer()){
    //         //Since we are the first 'lock' we need to ensure we have a clean slate to work with.
    //         this->Open();
    //     }

    //     Blocks::Header& header = oHeaderBlock.convert<Blocks::Header>();
    //     if(header.firstGradeBlock){
    //         Block tmp(header.firstGradeBlock); tmp.oBlock.oAttributes.nextBlockIndex = tmp.nIndex;
    //         Blocks::Grade& grade = tmp.convert<Blocks::Grade>();
    //         do {
    //             //Read in the next student block.
    //             tmp.nIndex = tmp.oBlock.oAttributes.nextBlockIndex;
    //             this->oStream >> tmp;
    //             std::cout << grade.nGrade << " " << grade.oGradeIdentifier.GetBuffer(*this) << std::endl;
    //         } while(tmp.oBlock.oAttributes.nextBlockIndex);
    //     }

    //     if(lock.isOriginalAcquirer()){
    //         //Since we are the first 'lock' we need to ensure we have a clean slate to work with.
    //         this->Close();
    //     }
    // }

    // bool BlockFile::AddEnrollment(const char* lpFullName, const char* lpPrefix, unsigned int nCourseID, const char* lpSemesterCode, const char* lpGrade){
    //     Lock lock(this->refLockGroup);
    //     if(lock.isOriginalAcquirer()){
    //         //Since we are the first 'lock' we need to ensure we have a clean slate to work with.
    //         this->Open();
    //     }
        
    //     Student student;
    //     Course course;
    //     Term term;
    //     Grade grade;

    //     if(!GetStudent(lpFullName, &student)){
    //         std::cout << "Student " << lpFullName << " not found!\n";
    //         return false;
    //     }

    //     if(!GetCourse(lpPrefix, nCourseID, &course)){
    //         std::cout << "Course " << lpPrefix << " " << nCourseID << " not found!\n";
    //         return false;
    //     }

    //     if(!GetTerm(lpSemesterCode, &term)){
    //         std::cout << "Term " << lpSemesterCode << " not found!\n";
    //         return false;
    //     }

    //     if(!GetGrade(lpGrade, &grade)){
    //         std::cout << "Grade " << lpGrade << " not found!\n";
    //         return false;
    //     }

    //     //fixme: Check for a duplicate enrollment!
    //     if(IsEnrolled(student, course, term)){
    //         std::cout << "Enrollment already exists!\n";
    //         return false;
    //     }

    //     Block tmp(this->GetAvailable());
    //     if(tmp.nIndex == 0) {
    //         ERROR_MSG("Failed to allocate a block for enrollment.");
    //         return false;
    //     }
    //     this->oStream << tmp;

    //     auto& enrollment = tmp.convert<Blocks::Enrollment>(); //Setup and commit the enrollment.
    //     enrollment.courseBlock = course.indexToAssociatedBlock;
    //     enrollment.gradeBlock = grade.indexToAssociatedBlock;
    //     enrollment.studentBlock = student.indexToAssociatedBlock;
    //     enrollment.termBlock = term.indexToAssociatedBlock;
    //     this->oStream << tmp;

    //     {
    //         Block bStudent(student.indexToAssociatedBlock);
    //         this->oStream >> bStudent;

    //         auto& _student = bStudent.convert<Blocks::Student>();
    //         if(_student.firstEnrollmentBlock){
    //             Block prv(0);
    //             Block crt(0);
    //             crt.oBlock.oAttributes.nextBlockIndex = _student.firstEnrollmentBlock;

    //             auto& enrollment = crt.convert<Blocks::Enrollment>();
    //             do {
    //                 prv = crt;
    //                 crt.nIndex = crt.oBlock.oAttributes.nextBlockIndex;
    //                 //Read in the next student block.
    //                 this->oStream >> crt;

    //                 Block bTerm(enrollment.termBlock);
    //                 this->oStream >> bTerm;

    //                 auto& _term = bTerm.convert<Blocks::Term>();
    //                 std::string strDisplayName = _term.oDisplayName.GetBuffer(*this);
    //                 //std::string strSemesterCode = _term.oSemesterCode.GetBuffer(*this);
    //                 //strSemesterCode == term.strSemesterCode.c_str() || compare(strSemesterCode.c_str(), term.strDisplayName.c_str())
    //                 if(_term.nYear >= term.nYear) { //Found matching term, prepend to list (OR) we found the next alphabetical placement.
    //                     if(compare(strDisplayName.c_str(), term.strDisplayName.c_str())) continue;
    //                     if(prv.nIndex) { //Check if we found the head block, ie we are at the header, gotta be careful here.
    //                         prv.oBlock.oAttributes.nextBlockIndex = tmp.nIndex;
    //                         tmp.oBlock.oAttributes.nextBlockIndex = crt.nIndex;
    //                         this->oStream << prv;
    //                         this->oStream << tmp;
    //                     }
    //                     else{
    //                         _student.firstEnrollmentBlock = tmp.nIndex;
    //                         tmp.oBlock.oAttributes.nextBlockIndex = crt.nIndex;
    //                         this->oStream << bStudent;
    //                         this->oStream << tmp;
    //                     }
    //                     break;
    //                 }
    //             } while(prv.oBlock.oAttributes.nextBlockIndex);
    //             if(prv.oBlock.oAttributes.nextBlockIndex == 0){
    //                 prv.oBlock.oAttributes.nextBlockIndex = tmp.nIndex;
    //                 this->oStream << prv;
    //             }
    //         }
    //         else {
    //             _student.firstEnrollmentBlock = tmp.nIndex;
    //             this->oStream << bStudent;
    //         }
    //     }

    //     {
    //         Block bCourse(course.indexToAssociatedBlock);
    //         this->oStream >> bCourse;

    //         auto& _course = bCourse.convert<Blocks::Course>();
    //         if(_course.firstCourseEnrollment){
    //             Block bCourseEnrollment(0);
    //             bCourseEnrollment.oBlock.oAttributes.nextBlockIndex = _course.firstCourseEnrollment;

    //             bool inserted = false;
    //             auto& _course_enrollment = bCourseEnrollment.convert<Blocks::CourseEnrollment>();
    //             do {
    //                 bCourseEnrollment.nIndex = bCourseEnrollment.oBlock.oAttributes.nextBlockIndex;
    //                 this->oStream >> bCourseEnrollment; // Read in the next block.

    //                 for(auto& index : _course_enrollment.arrIndexs){
    //                     if(!index) {
    //                         index = tmp.nIndex;
    //                         inserted = true; break;
    //                     }
    //                 }
    //                 if(inserted){
    //                     this->oStream << bCourseEnrollment;
    //                     break;
    //                 }
    //             } while(bCourseEnrollment.oBlock.oAttributes.nextBlockIndex);

    //             if(!inserted){
    //                 Block bNewArray(this->GetAvailable());
    //                 if(!bNewArray.nIndex) {
    //                     ERROR_MSG("Failed to allocate new block for a Course Enrollment, aborting.");
    //                     exit(E_ABORT);
    //                 }
    //                 bCourseEnrollment.oBlock.oAttributes.nextBlockIndex = bNewArray.nIndex;
    //                 this->oStream << bCourseEnrollment;

    //                 auto& _new_course_enrollment = bNewArray.convert<Blocks::CourseEnrollment>();
    //                 _new_course_enrollment.arrIndexs[0] = tmp.nIndex;
    //                 this->oStream << bNewArray;
    //             }
    //         }
    //         else {
    //             Block bNewArray(this->GetAvailable());
    //             if(!bNewArray.nIndex) {
    //                 ERROR_MSG("Failed to allocate new block for a Course Enrollment, aborting.");
    //                 exit(E_ABORT);
    //             }
    //             _course.firstCourseEnrollment = bNewArray.nIndex;
    //             this->oStream << bCourse;

    //             auto& _new_course_enrollment = bNewArray.convert<Blocks::CourseEnrollment>();
    //             _new_course_enrollment.arrIndexs[0] = tmp.nIndex;
    //             this->oStream << bNewArray;
    //         }
    //     }

    //     if(lock.isOriginalAcquirer()){
    //         //Since we are the first 'lock' we need to ensure we have a clean slate to work with.
    //         this->Close();
    //     }
    //     return true;
    // }
    
    // void BlockFile::ListEnrollmentsByCourses() {
    //     Lock lock(this->refLockGroup);
    //     if(lock.isOriginalAcquirer()){
    //         //Since we are the first 'lock' we need to ensure we have a clean slate to work with.
    //         this->Open();
    //     }

    //     Blocks::Header& header = oHeaderBlock.convert<Blocks::Header>();
    //     if(header.firstCourseBlock){
    //         Block tmp(header.firstCourseBlock); tmp.oBlock.oAttributes.nextBlockIndex = tmp.nIndex;
    //         Blocks::Course& course = tmp.convert<Blocks::Course>();
    //         do {
    //             //Read in the next student block.
    //             tmp.nIndex = tmp.oBlock.oAttributes.nextBlockIndex;
    //             this->oStream >> tmp;
    //             if(course.firstCourseEnrollment){
    //                 Block bCourseEnrollment(0);
    //                 bCourseEnrollment.oBlock.oAttributes.nextBlockIndex = course.firstCourseEnrollment;

    //                 auto& _course_enrollment = bCourseEnrollment.convert<Blocks::CourseEnrollment>();
    //                 do {
    //                     bCourseEnrollment.nIndex = bCourseEnrollment.oBlock.oAttributes.nextBlockIndex;
    //                     this->oStream >> bCourseEnrollment; // Read in the next block.

    //                     for(const auto& index : _course_enrollment.arrIndexs){
    //                         if(index) {
    //                             Block bEnrollment(index);
    //                             auto& _enrollment = bEnrollment.convert<Blocks::Enrollment>();
    //                             this->oStream >> bEnrollment;

    //                             Block bStudent(_enrollment.studentBlock);
    //                             auto& _student = bStudent.convert<Blocks::Student>();
    //                             this->oStream >> bStudent;

    //                             Block bTerm(_enrollment.termBlock);
    //                             auto& _term = bTerm.convert<Blocks::Term>();
    //                             this->oStream >> bTerm;

    //                             Block bGrade(_enrollment.gradeBlock);
    //                             auto& _grade = bGrade.convert<Blocks::Grade>();
    //                             this->oStream >> bGrade;

    //                             std::cout << _student.oFullName.GetBuffer(*this) << ' ' << course.oPrefix.GetBuffer(*this) << ' ' << _term.oSemesterCode.GetBuffer(*this) << ' ' << course.nCourseNumber << ' ' << course.oDisplayName.GetBuffer(*this) << ' ' << _grade.oGradeIdentifier.GetBuffer(*this) << '\n';
    //                         }
    //                     }
    //                 } while(bCourseEnrollment.oBlock.oAttributes.nextBlockIndex);
    //             }
    //         } while(tmp.oBlock.oAttributes.nextBlockIndex);
    //     }

    //     if(lock.isOriginalAcquirer()){
    //         //Since we are the first 'lock' we need to ensure we have a clean slate to work with.
    //         this->Close();
    //     }
    // }

    // void BlockFile::ListEnrollmentsByStudent(const char* lpFullName) {
    //     Lock lock(this->refLockGroup);
    //     if(lock.isOriginalAcquirer()){
    //         //Since we are the first 'lock' we need to ensure we have a clean slate to work with.
    //         this->Open();
    //     }

    //     Blocks::Header& header = oHeaderBlock.convert<Blocks::Header>();
    //     if(header.firstStudentBlock){
    //         Block tmp(header.firstStudentBlock); tmp.oBlock.oAttributes.nextBlockIndex = tmp.nIndex;
    //         Blocks::Student& student = tmp.convert<Blocks::Student>();
    //         do {
    //             //Read in the next student block.
    //             tmp.nIndex = tmp.oBlock.oAttributes.nextBlockIndex;
    //             this->oStream >> tmp;

    //             if(student.oFullName.GetBuffer(*this) != lpFullName) continue;

    //             if(student.firstEnrollmentBlock){
    //                 Block bEnrollment(0);
    //                 bEnrollment.oBlock.oAttributes.nextBlockIndex = student.firstEnrollmentBlock;

    //                 double nGPA = 0;
    //                 unsigned int nCredits = 0;
    //                 std::string strSemesterCode;

    //                 auto& _enrollment = bEnrollment.convert<Blocks::Enrollment>();
    //                 do {
    //                     bEnrollment.nIndex = bEnrollment.oBlock.oAttributes.nextBlockIndex;
    //                     this->oStream >> bEnrollment; // Read in the next block.
                        
    //                     Block bCourse(_enrollment.courseBlock);
    //                     auto& _course = bCourse.convert<Blocks::Course>();
    //                     this->oStream >> bCourse;

    //                     Block bTerm(_enrollment.termBlock);
    //                     auto& _term = bTerm.convert<Blocks::Term>();
    //                     this->oStream >> bTerm;

    //                     Block bGrade(_enrollment.gradeBlock);
    //                     auto& _grade = bGrade.convert<Blocks::Grade>();
    //                     this->oStream >> bGrade;

    //                     if(_term.oSemesterCode.GetBuffer(*this) != strSemesterCode){
    //                         std::cout << "============ Semester: " << _term.oDisplayName.GetBuffer(*this) << ' ' << _term.nYear << " ============" << std::endl;
    //                         strSemesterCode = _term.oSemesterCode.GetBuffer(*this); //todo: Optimize this.
    //                     }
    //                     std::cout << _course.oPrefix.GetBuffer(*this) << _course.nCourseNumber << ' ' << _course.oDisplayName.GetBuffer(*this) << '(' << _course.nCredits << ") " << _grade.oGradeIdentifier.GetBuffer(*this) << std::endl;

    //                     nGPA += _grade.nGrade * _course.nCredits;
    //                     nCredits += _course.nCredits;
    //                 } while(bEnrollment.oBlock.oAttributes.nextBlockIndex);
    //                 nGPA /= nCredits;
    //                 std::cout << " STUDENT HOURS COMPLETED: " << nCredits << '\n'; 
    //                 std::cout << " STUDENT GPA: " << nGPA << '\n';
    //             }
    //         } while(tmp.oBlock.oAttributes.nextBlockIndex);
    //     }
    //     if(lock.isOriginalAcquirer()){
    //         //Since we are the first 'lock' we need to ensure we have a clean slate to work with.
    //         this->Close();
    //     }
    // }

    // bool BlockFile::IsEnrolled(const Student& refStudent, const Course& refCourse, const Term& refTerm) {
    //     Lock lock(this->refLockGroup);
    //     if(lock.isOriginalAcquirer()){
    //         //Since we are the first 'lock' we need to ensure we have a clean slate to work with.
    //         this->Open();
    //     }

    //     unsigned int indexToStudent = refStudent.indexToAssociatedBlock;
    //     if(!indexToStudent) {
    //         Student temp;
    //         if(!GetStudent(refStudent.strFullName.c_str(), &temp))
    //             return false;
    //         indexToStudent = temp.indexToAssociatedBlock;
    //     }
        
    //     Course course = refCourse;
    //     if(!course.indexToAssociatedBlock) {
    //         if(!GetCourse(refCourse.strPrefix.c_str(), refCourse.nCourseNumber, &course))
    //             return false;
    //     }

    //     Term term = refTerm;
    //     if(!term.indexToAssociatedBlock) {
    //         if(!GetTerm(refTerm.strSemesterCode.c_str(), &term))
    //             return false;
    //     }
        

    //     Block bStudent(indexToStudent);
    //     auto& _student = bStudent.convert<Blocks::Student>();
    //     this->oStream >> bStudent;

    //     if(_student.firstEnrollmentBlock){
    //         Block bEnrollment(0); bEnrollment.oBlock.oAttributes.nextBlockIndex = _student.firstEnrollmentBlock;
    //         auto& _enrollment = bEnrollment.convert<Blocks::Enrollment>();
    //         do {
    //             bEnrollment.nIndex = bEnrollment.oBlock.oAttributes.nextBlockIndex;
    //             this->oStream >> bEnrollment;

    //             if(term.indexToAssociatedBlock == _enrollment.termBlock && course.indexToAssociatedBlock == _enrollment.courseBlock) {
    //                 if(lock.isOriginalAcquirer()){
    //                     //Since we are the first 'lock' we need to ensure we have a clean slate to work with.
    //                     this->Close();
    //                 }
    //                 return true;
    //             }
    //         } while(bEnrollment.oBlock.oAttributes.nextBlockIndex);
    //     }
    //     else {
    //         if(lock.isOriginalAcquirer()){
    //             //Since we are the first 'lock' we need to ensure we have a clean slate to work with.
    //             this->Close();
    //         }
    //         return false;
    //     }

    //     if(lock.isOriginalAcquirer()){
    //         //Since we are the first 'lock' we need to ensure we have a clean slate to work with.
    //         this->Close();
    //     }
    //     return false;
    // }

    // std::string BlockFile::GetCharacterBuffer(BlockIndex firstBlock, size_t nMaxSize) {
    //     std::string res;

    //     //Make an attempt to reserve memory for the string if we happen to know a size.
    //     if(nMaxSize != (size_t)(~0))
    //         res.reserve(nMaxSize + 1);

    //     size_t index = 0;
    //     Block tmp(firstBlock);
    //     do {
    //         //Read in the block.
    //         this->oStream >> tmp;

    //         //Read in till a Null character, or we run out of data in the current block.
    //         char* txt = tmp.oBlock.oData.arrBytes;
    //         size_t i = 0;
    //         for(; i < sizeof(Block::_BLOCK::Data); i++){
    //             if(txt[i])
    //                 res += txt[i];
    //             else break;
    //         }

    //         //Check if we ended early, and bail accordingly.
    //         if(i != sizeof(Block::_BLOCK::Data)) break;

    //         //Increment the index that we are working with.
    //         index += i;

    //         //Set our block to point to the next one (if needed, otherwise we should bail at this point).
    //         tmp.nIndex = tmp.oBlock.oAttributes.nextBlockIndex;
    //     } while(tmp.nIndex && index < nMaxSize);

    //     //Moving makes the underlying string buffer move from one to the next, can be more efficient in terms of memory usage.
    //     return std::move(res);
    // }

}