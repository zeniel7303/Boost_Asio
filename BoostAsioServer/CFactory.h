#pragma once
#include "CServer.h"
#include "CObjectPool.h"

#include <map>

// Factory Method
//https://cinrueom.tistory.com/34

// Prototype Pattern
//https://cinrueom.tistory.com/36
// 다른 Design Pattern 설명도 잘 되어있음.
//https://blog.naver.com/PostView.naver?blogId=kwizman&logNo=221296747408&categoryNo=22&parentCategoryNo=0&viewDate=&currentPage=1&postListTopCurrentPage=1&from=postView&userTopListOpen=true&userTopListCount=10&userTopListManageOpen=false&userTopListCurrentPage=1

// Composite Pattern
//https://cinrueom.tistory.com/86

class IObjectPool
{
public:
	IObjectPool() = default;
	virtual ~IObjectPool() {}

	virtual void Init() = 0;
	virtual void Close() = 0;
};

template<typename T>
class CObjectPoolEx : public IObjectPool, public CObjectPool<T>
{
public:
	CObjectPoolEx(int _size);
	~CObjectPoolEx() override;

	void Init() override;
	void Close() override;
};

class CFactory
{
private:
	static CFactory m_singleton;

	std::map<size_t, std::unique_ptr<IObjectPool>> m_mapPool;
	bool m_isInit;
	
	CFactory();
public:
	~CFactory();

	static CFactory& Instance();

	int Init();
	int Close();

	template<typename T>
	bool AddObejctPool(int _size);
	template<typename T>
	std::shared_ptr<T> New();
	template<typename T, typename TDeleter>
	std::shared_ptr<T> New(TDeleter&& _deleter);
	template<typename T>
	T* Alloc();

	template<typename T, typename TDeleter>
	static std::shared_ptr<T> MakePtr(T* _t, TDeleter&& _deleter);

	template<typename T>
	bool Delete(T* _t);
};

#include "CFactory.Inl"