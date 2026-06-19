#ifndef THREADS_COND_HPP
#define THREADS_COND_HPP

#include "mutex.hpp"

namespace Threads {

class Cond
{
   public:
      Cond();
      ~Cond();
      void wait(Mutex& lock);
      void wake();
      void wake_all();

   private:
      pthread_cond_t m_cond;
};

}
#endif
