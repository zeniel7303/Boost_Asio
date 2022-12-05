#include "CFactory.h"

CFactory CFactory::m_singleton;

CFactory::CFactory() : m_mapPool(), m_isInit(false)
{

}

CFactory::~CFactory()
{

}

CFactory& CFactory::Instance()
{
	return m_singleton;
}

int CFactory::Init()
{
	assert(m_isInit == false && "init error");

	for (auto& pair : m_mapPool)
	{
		pair.second->Init();
	}

	m_isInit = true;

	return 0;
}

int CFactory::Close()
{
	m_mapPool.clear();

	return 0;
}