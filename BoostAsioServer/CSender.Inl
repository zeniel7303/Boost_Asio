template<typename T>
static std::shared_ptr<CSender> CSender::Alloc()
{
	int size = sizeof(T);

	assert(size >= sizeof(CPacketHeader) && "size error");

	auto shared = m_sendPool.Alloc(size);

	if (shared == nullptr) return nullptr;

	(reinterpret_cast<CPacketHeader*>(shared->m_tempChunk))->m_dataSize = size;

	return shared;
}

template<typename T>
T* CSender::GetWritePointer()
{
	auto ptr = reinterpret_cast<char*>(m_tempChunk);
	
	return reinterpret_cast<T*>(ptr);
}