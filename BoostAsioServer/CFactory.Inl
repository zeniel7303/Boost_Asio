template<typename T>
CObjectPoolEx<T>::CObjectPoolEx(int _size)
	: IObjectPool(), CObjectPool<T>(_size)
{

}

template<typename T>
CObjectPoolEx<T>::~CObjectPoolEx()
{

}

template<typename T>
void CObjectPoolEx<T>::Init()
{
	CObjectPool<T>::Init();
}

template<typename T>
void CObjectPoolEx<T>::Close()
{
	CObjectPool<T>::Close();
}

template<typename T>
bool CFactory::AddObejctPool(int _size)
{
	size_t key = typeid(T).hash_code();
	if (m_mapPool.find(key) != m_mapPool.end()) return false;

	std::unique_ptr<IObjectPool> pool = std::make_unique<CObjectPoolEx<T>>(_size);
	auto result = m_mapPool.emplace(key, std::move(pool));

	assert(result.second == true && "add error");

	return true;
}
template<typename T>
std::shared_ptr<T> CFactory::New()
{
	return New<T>([](T* _t)
		{
			if (_t == nullptr) return false;

			return CFactory::Instance().Delete(_t);
		});
}
template<typename T, typename TDeleter>
std::shared_ptr<T> CFactory::New(TDeleter&& _deleter)
{
	T* object = Alloc<T>();

	return MakePtr(object, _deleter);
}
template<typename T>
T* CFactory::Alloc()
{
	size_t key = typeid(T).hash_code();

	if (m_mapPool.find(key) == m_mapPool.end()) return nullptr;

	auto value = m_mapPool.find(key);
	std::unique_ptr<IObjectPool>& pPool = value->second;
	CObjectPoolEx<T>* object = dynamic_cast<CObjectPoolEx<T>*>(pPool.get());

	assert(object != nullptr && "objectPool is nullptr");

	T* t = object->NewObject();
	return t;
}

template<typename T, typename TDeleter>
static std::shared_ptr<T> CFactory::MakePtr(T* _t, TDeleter&& _deleter)
{
	return std::shared_ptr<T>(_t, _deleter);
}

template<typename T>
bool CFactory::Delete(T* _t)
{
	assert(_t != nullptr && "_t is nullptr");

	size_t key = typeid(T).hash_code();

	if (m_mapPool.find(key) == m_mapPool.end()) return false;

	auto value = m_mapPool.find(key);
	std::unique_ptr<IObjectPool>& pPool = value->second;
	CObjectPoolEx<T>* object = dynamic_cast<CObjectPoolEx<T>*>(pPool.get());

	assert(object != nullptr && "objectPool is nullptr");

	return object->DeleteObject(_t);
}