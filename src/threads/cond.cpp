#include "cond.hpp"

namespace Threads {

Cond::Cond()
{
   pthread_cond_init(&m_cond, NULL);
}

Cond::~Cond()
{
   pthread_cond_destroy(&m_cond);
}

void Cond::wait(Mutex& lock)
{
   pthread_cond_wait(&m_cond, &lock.m_lock);
}

void Cond::wake()
{
   pthread_cond_signal(&m_cond);
}

void Cond::wake_all()
{
   pthread_cond_broadcast(&m_cond);
}

}
