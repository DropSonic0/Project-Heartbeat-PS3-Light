#ifndef THREADS_MUTEX_HPP
#define THREADS_MUTEX_HPP
#include <pthread.h>

namespace Threads {

class Cond;
class Mutex
{
   friend class Cond;
   public:
      Mutex();
      ~Mutex();
      void lock();
      void unlock();

   private:
      pthread_mutex_t m_lock;
};

}

#endif
