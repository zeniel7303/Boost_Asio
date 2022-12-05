#pragma once
#include "CServer.h"
#include <shared_mutex>

template<typename TClass>
class CSharedLock
{
protected:
	std::shared_mutex m_locker;

public:
	using ReadLock = std::shared_lock<TClass>;
	using WriteLock = std::unique_lock<TClass>;

	void lock() { m_locker.lock(); }
	void unlock() { m_locker.unlock(); }
	bool try_lock() { return m_locker.try_lock(); }
	void lock_shared() { m_locker.lock_shared(); }
	void unlock_shared() { m_locker.unlock_shared(); }
	bool try_lock_shared() { return m_locker.try_lock_shared(); }
	const std::shared_mutex& GetLocker() const { return m_locker; }
};

/*using CSharedLock = std::shared_mutex;

template <typename T>
using ReadLock = std::shared_lock<T>;
template <typename T>
using WriteLock = std::unique_lock<T>;

#define LOCKOBJECT(_class, _lockType) public:\
using ReadLock	= ReadLock<_class>;\
using WriteLock = WriteLock<_class>;\
void lock(){ m_locker.lock(); }\
void unlock(){ m_locker.unlock(); }\
bool try_lock(){ return m_locker.try_lock(); }\
void lock_shared(){ m_locker.lock_shared(); }\
void unlock_shared(){ m_locker.unlock_shared(); }\
bool try_lock_shared(){ return m_locker.try_lock_shared(); }\
const _lockType& GetLocker() const { return m_locker; }\
protected:\
_lockType m_locker;		*/					