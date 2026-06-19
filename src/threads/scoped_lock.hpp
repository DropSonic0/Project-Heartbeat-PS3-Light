#ifndef THREADS_SCOPED_LOCK_HPP
#define THREADS_SCOPED_LOCK_HPP

#include "mutex.hpp"

namespace Threads {

class ScopedLock
{
   public:
      explicit ScopedLock(Mutex& lock);
      ~ScopedLock();
   private:
      Mutex& m_lock;
};

}

#endif
