#include "Database.hpp"
#include "Block.hpp"
#include "Blocks.hpp"

namespace AK
{
    Block& Database::GetBlock(BlockIndex index) {
        auto& find = this->mapBlocks.find(index);
        if(find != this->mapBlocks.end()){
            if(!this->blockFile.ValidateBlock(*find->second))
                this->blockFile.ReadBlock(*find->second);
            return *find->second;
        }
        Block* block = new Block(index);
        if(!block){
            FATAL_MSG("Out of memory! Couldn't allocate a block to the cache.");
            exit(-2);
        }
        this->blockFile.ReadBlock(*block);
        this->mapBlocks.insert({index,block});
        return *block;
    }

    void Database::Decache(BlockIndex index) { 
        auto& find = this->mapBlocks.find(index);
        if(find != this->mapBlocks.end()){
            if(find->second) delete find->second;
            this->mapBlocks.erase(index);
        }
    }

    Database::Database(const char* lpFilename, const char* lpLockName) : lockGroup(lpLockName), blockFile(lpFilename, lockGroup) {}

    int Database::run(int argc, char** argv) {
        std::istream* input = &std::cin;
        bool bFile = false;
        if(argc > 2){
            if(!strncmp(argv[1], "-f", 2)){
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
        Lock lock(this->lockGroup);
        if(lock.isInitialLock())
            blockFile.Open();

        if(this->getStudent(refStudent.strLastname.c_str(), refStudent.strLastname.c_str(), NULL))
            return false;

        auto& headerBlock = this->GetBlock(0);
        auto& header = headerBlock.data<Blocks::Header>();
        if(header.firstStudentBlock){
            auto& studentBlock = this->GetBlock(header.firstStudentBlock);
            if(studentBlock.)
        }

        if(lock.isInitialLock())
            blockFile.Close();
    }

    bool Database::addCourse(const Course& refCourse) {

    }

    bool Database::addTerm(const Term& refTerm) {

    }

    bool Database::addGrade(const Grade& refGrade) {

    }

    bool Database::addEnrollment(const Student& refStudent, const Course& refCourse, const Term& refTerm, const Grade& refGrade) {

    }


}