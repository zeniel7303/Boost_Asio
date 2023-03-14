template<typename T>
CObjectPool<T>::CObjectPool(int _size)
{
	m_isAlloc = false;
	m_usedSize = 0;
	m_poolSize = _size;

	m_vector.reserve(_size);
}

template<typename T>
CObjectPool<T>::~CObjectPool()
{
	Close();
}

template<typename T>
void CObjectPool<T>::Init()
{
	assert(m_isAlloc == false && "Alloc Fail");

	std::unique_lock<CObjectPool<T>> writeLock(*this);

	for (int i = 0; i < m_poolSize; ++i)
	{
		try
		{
			T* tObject = new T;
			m_vector.push_back(tObject);
			m_queue.push(tObject);
		}
		catch (std::exception& _exception)
		{
			UNREFERENCED_PARAMETER(_exception);
			assert(0 && "Init Failed");
		}
	}

	m_usedSize = 0;
	m_isAlloc = true;
}

template<typename T>
void CObjectPool<T>::Close()
{
	std::unique_lock<CObjectPool<T>> writeLock(*this);

	if (m_isAlloc == false) return;

	while (m_queue.empty() == false)
	{
		m_queue.pop();
	}

	for (auto& _object : m_vector)
	{
		static_assert(std::is_pointer<T*>::value);

		delete[] _object;
		_object = nullptr;
	}

	m_usedSize = 0;
	m_isAlloc = false;
}

template<typename T>
T* CObjectPool<T>::NewObject()
{
	assert(m_isAlloc == true && "Init First");

	T* object = nullptr;

	{
		std::unique_lock<CObjectPool<T>> writeLock(*this);

		if (m_usedSize >= GetMaxSize()) return object;

		object = m_queue.front();
		m_queue.pop();
		m_usedSize++;
	}

	assert(object != nullptr && "New Object Is NULLPTR");

	return object;
}

template<typename T>
bool CObjectPool<T>::DeleteObject(T* _object)
{
	assert(m_isAlloc == true && "Init First");
	assert(_object != nullptr && "Delete Nullptr");

	bool flag = false;
	for (const auto& _auto : m_vector)
	{
		if (_auto == _object)
		{
			flag = true;
			break;
		}
	}

	if (flag == false)
	{
		return false;
	}

	{
		std::unique_lock<CObjectPool<T>> writeLock(*this);

		// 해당 오브젝트에 대한 초기화는 어디서? or 해야하는가?
		m_queue.push(_object);
		m_usedSize--;
	}

	return true;
}

template<typename T>
int CObjectPool<T>::GetMaxSize()
{
	return m_poolSize;
}

template<typename T>
int CObjectPool<T>::GetUsedSize()
{
	return m_usedSize;
}