#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>

#include <sys/syscall.h>  
#define gettid() syscall(__NR_gettid)
using namespace std;

class mynoncopyable
{
protected:
    mynoncopyable() {}
private:
    mynoncopyable(const mynoncopyable&) = delete;
    void operator=(const mynoncopyable&) = delete;    
};

class MyMutexLock:public mynoncopyable
{
private:
    pthread_mutex_t mutex;
    pid_t _hold;
public:
    MyMutexLock():_hold(0)
    {
        pthread_mutex_init(&mutex,NULL);
    }

    ~MyMutexLock()
    {
        assert(_hold == 0);
        pthread_mutex_destroy(&mutex);
    }

    bool isLockedByThisThread()
    {
        return _hold == gettid();
    }

    void assertLocked()
    {
        assert(isLockedByThisThread());
    }

    void lock()
    {
        pthread_mutex_lock(&mutex);
        _hold = gettid();
    }


    void unlock()
    {
        _hold = 0;
        pthread_mutex_unlock(&mutex);
    }

    // 不是提供给用户的，而是提供给MyMutexGuard的
    pthread_mutex_t * getPthreadMutex()
    {   return &mutex;  }
};


class MyMutexGuard:public mynoncopyable
{
private:
    MyMutexLock & _mutex;    //MyMutexLock对象的引用
public:
    explicit MyMutexGuard(MyMutexLock & mutex):_mutex(mutex)
    {
        _mutex.lock();
    }

    ~MyMutexGuard()
    {
        _mutex.unlock();
    }
};
