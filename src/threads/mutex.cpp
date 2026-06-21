#include "mutex.hpp"

namespace Threads {

Mutex::Mutex()
{
   pthread_mutexattr_t attr;
   pthread_mutexattr_init(&attr);
   pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
   pthread_mutex_init(&m_lock, &attr);
   pthread_mutexattr_destroy(&attr);
}

Mutex::~Mutex()
{
   pthread_mutex_destroy(&m_lock);
}

void Mutex::lock()
{
   pthread_mutex_lock(&m_lock);
}

void Mutex::unlock()
{
   pthread_mutex_unlock(&m_lock);
}

}
