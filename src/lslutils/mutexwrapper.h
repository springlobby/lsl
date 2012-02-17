#ifndef LSL_MUTEXWRAPPER_H
#define LSL_MUTEXWRAPPER_H

#include <boost/thread/mutex.hpp>
#include "logging.h"

namespace LSL {

template<class T>
class MutexWrapper;

//! pure interface a single MutexWrapper interface
class AbstractMutexWrapper{
  public:
  virtual ~AbstractMutexWrapper(){}
  virtual void Lock()=0;
  virtual void UnLock()=0;
};

//! implements a temporary exclusive access object
template<class T>
class ScopedLocker
{
private:
    MutexWrapper<T> &mw;
    ScopedLocker(const ScopedLocker<T> &/*other*/){}/// prevent copying
    ScopedLocker&  operator= (const ScopedLocker& /*other*/){}/// and assignment
public:
    explicit ScopedLocker(MutexWrapper<T> &mw_):mw(mw_){
        mw.Lock();
    }
    ~ScopedLocker(){
        mw.UnLock();
    }
    T &Get(){
        return mw.GetData();
    }
};
/*
class ScopedLocker
{
  private:
  AbstractMutexWrapper *mw;
  ScopedLocker(const ScopedLocker<T> &other){}/// prevent copying
  ScopedLocker&  operator= (const ScopedLocker& other){}/// and assignment
  public:
  explicit ScopedLocker(AbstractMutexWrapper &mw_):mw(*mw_){
    mw.Lock();
  }
  ~ScopedLocker(){
    mw.UnLock();
  }
};*/

//!
template<class T>
class MutexWrapper: public AbstractMutexWrapper
{
  boost::mutex mutex;/// critical section is same as mutex except on windows it only works within one process (i.e. program). I'm gonna call it mutex.
  T data;
  bool locked;
  public:
    MutexWrapper():locked(false){
    }
    virtual ~MutexWrapper(){
    }
    virtual void Lock(){
      mutex.lock();
      locked=true;
    }
    virtual void UnLock(){
      locked=false;
      mutex.unlock();
    }
    protected:
    T &GetData(){
      if(!locked) {
            LslError("serious error in MutexWrapper usage : not locked, but Get() is called!");
      }
      return data;
    }
    friend class ScopedLocker<T>;
};

} // namespace LSL

/**
 * \file mutexwrapper.h
 * \section LICENSE
Copyright 2012 by The libSpringLobby team. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of
      conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, this list
      of conditions and the following disclaimer in the documentation and/or other materials
      provided with the distribution.

THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**/

#endif // LSL_MUTEXWRAPPER_H
