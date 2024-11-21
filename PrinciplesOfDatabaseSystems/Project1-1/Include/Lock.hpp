#include "Common.hpp"

#ifndef __WIN32__
#else //__WIN32__
#include <synchapi.h>
#include <Windows.h>
#endif//__WIN32__

#ifndef __AK_LOCK_HPP__
#define __AK_LOCK_HPP__

namespace AK
{
    class LockGroup {
        friend class Lock;
    private:
        #ifndef __WIN32__

        #else //__WIN32__
            HANDLE hLock;
            DWORD dwCurrentThread;
            DWORD dwReferenceCount;
        #endif//__WIN32__

    public:
        LockGroup(const char* lpName);
        ~LockGroup();

    protected:
        bool lock();
        void unlock();

    public:
        inline bool isLocked() const { return dwReferenceCount; }

    public:
        LockGroup(const LockGroup&) = delete;
        LockGroup& operator=(const LockGroup&) = delete;
    };

    class Lock {
    private:
        LockGroup& refGroup;
        bool bInitialLock;

    public:
        Lock(LockGroup& refGroup);
        Lock(const Lock&) = delete;
        ~Lock();

    public:
        inline bool isInitialLock() const { return bInitialLock; };

    public:
        Lock& operator=(const Lock&) = delete;
    };
}

#endif//__AK_LOCK_HPP__