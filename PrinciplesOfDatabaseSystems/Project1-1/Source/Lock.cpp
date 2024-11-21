#include "Lock.hpp"

// #include <sys/types.h>
// #include <sys/stat.h>
// #include <cstdio>

// #ifndef __WIN32__
// #include <pthread.h>
// #include <stdlib.h>
// #include <unistd.h>
// #else //__WIN32__

// #define stat _stat

// #endif//__WIN32__

namespace AK
{
    #ifdef __WIN32__
        LockGroup::LockGroup(const char* lpName) : hLock(NULL), dwCurrentThread(0), dwReferenceCount(0) {
            hLock = CreateMutexA(NULL, FALSE, lpName);
            if(!hLock) {
                ERROR_MSG("Failed to create a mutex object, thus no synchronization can be achieved. Abort!");
                exit(E_ABORT);
            }
        }

        LockGroup::~LockGroup() {
            if(hLock)
                ReleaseMutex(hLock);
            hLock = NULL;
        }

        bool LockGroup::lock() {
            if(dwReferenceCount && dwCurrentThread == GetCurrentThreadId()){
                dwReferenceCount += 1;
                //std::cout << "Thread # " << dwCurrentThread << " had the lock already!\n";
                return false;
            }else {
                DWORD dwResult;
                do {
                    dwResult = WaitForSingleObject(hLock, INFINITE);
                    if(dwResult == WAIT_FAILED){
                        ERROR_MSG("Failed to aquire a mutex lock, aborting database access!");
                        exit(-1);
                    }
                } while(dwResult);
                dwReferenceCount = 1;
                dwCurrentThread = GetCurrentThreadId();
                //std::cout << "Thread # " << dwCurrentThread << " has aquired the lock!\n";
                return true;
            }
        }

        void LockGroup::unlock() {
            if(dwReferenceCount && dwCurrentThread == GetCurrentThreadId()) {
                dwReferenceCount -= 1;
                if(!dwReferenceCount) {
                    //std::cout << "Thread # " << dwCurrentThread << " has released the lock.\n";
                    dwCurrentThread = 0;
                    ReleaseMutex(hLock);
                }
                //else
                //    std::cout << "Thread # " << dwCurrentThread << " still has the lock, " << dwReferenceCount << " references left.\n";
            }
        }
    #endif

    Lock::Lock(LockGroup& refGroup) : refGroup(refGroup) {
        bInitialLock = refGroup.lock();
    }

    Lock::~Lock() {
        refGroup.unlock();
    }
}