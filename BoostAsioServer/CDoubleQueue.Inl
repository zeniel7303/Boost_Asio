#include "CDoubleQueue.h"

template<typename T>
CDoubleQueue<T>::CDoubleQueue()
{
}

template<typename T>
CDoubleQueue<T>::~CDoubleQueue()
{
}

template<typename T>
void CDoubleQueue<T>::Swap()
{
	m_spinLock.Lock();

	std::queue<T>* temp;

	temp = primaryQueue;
	primaryQueue = secondaryQueue;
	secondaryQueue = temp;

	m_spinLock.UnLock();
}

template<typename T>
void CDoubleQueue<T>::AddObject(const T& _object)
{
	m_spinLock.Lock();

	primaryQueue->push(_object);

	m_spinLock.UnLock();
}

template<typename T>
T& CDoubleQueue<T>::PopObejct()
{
	T& retVal = secondaryQueue->front();
	secondaryQueue->pop();

	return retVal;
}

template<typename T>
bool CDoubleQueue<T>::IsEmpty()
{
	return secondaryQueue->empty();
}

template<typename T>
std::queue<T>& CDoubleQueue<T>::GetPrimaryQueue()
{
	return *primaryQueue;
}

template<typename T>
std::queue<T>& CDoubleQueue<T>::GetSecondaryQueue()
{
	return *secondaryQueue;
}

template<typename T>
int CDoubleQueue<T>::GetPrimaryQueueSize()
{
	return primaryQueue->size();
}

template<typename T>
int CDoubleQueue<T>::GetSecondaryQueueSize()
{
	return secondaryQueue->size();
}
