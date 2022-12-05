#pragma once
#include "CSharedLock.h"
#include <type_traits>
#include <queue>
#include <vector>

template<typename T>
class CObjectPool : public CSharedLock<CObjectPool<T>>
{
private:
	std::queue<T*>	m_queue;
	std::vector<T*>	m_vector;

	bool			m_isAlloc;
	int				m_usedSize;
	int				m_poolSize;

public:
	CObjectPool(int _size);
	~CObjectPool();

	void Init();
	void Close();

	T* NewObject();
	bool DeleteObject(T* _object);

	int GetMaxSize();
	int GetUsedSize();
};

#include "CObjectPool.Inl"