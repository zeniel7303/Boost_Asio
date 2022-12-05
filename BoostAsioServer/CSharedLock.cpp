#include "CSharedLock.h"

#ifdef _DEBUG
	CSharedLock::CSharedLock() : m_mutex()
	{
	
	}
	
	CSharedLock::~CSharedLock()
	{
	
	}
	
	void CSharedLock::lock()
	{
		m_mutex.lock();
	}
	
	void CSharedLock::unlock()
	{
		m_mutex.unlock();
	}
	
	bool CSharedLock::try_lock()
	{
		return m_mutex.try_lock();
	}
	
	void CSharedLock::lock_shared()
	{
		m_mutex.lock_shared();
	}
	
	void CSharedLock::unlock_shared()
	{
		m_mutex.unlock_shared();
	}
	
	bool CSharedLock::try_lock_shared()
	{
		return m_mutex.try_lock_shared();
	}
#endif // _DEBUG