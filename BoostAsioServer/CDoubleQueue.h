#pragma once
#include "CServer.h"
#include "CSpinLock.h"

template<typename T>
class CDoubleQueue
{
private:
	std::queue<T>*	primaryQueue;
	std::queue<T>*	secondaryQueue;

	CSpinLock		m_spinLock;

public:
	CDoubleQueue();
	~CDoubleQueue();

	void Swap();
	void AddObject(const T& _object);
	T& PopObejct();
	bool IsEmpty();
	std::queue<T>& GetPrimaryQueue();
	std::queue<T>& GetSecondaryQueue();
	int GetPrimaryQueueSize();
	int GetSecondaryQueueSize();
};

#include "CDoubleQueue.Inl"