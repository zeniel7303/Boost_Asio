#pragma once
#include "CServer.h"

class CSpinLock
{
private:
    bool m_locked = false;

public:
    void Lock()
    {
        while (m_locked); // 다른 누군가 lock을 획득했다면 무한루프.

        m_locked = true;
    }
    void UnLock()
    {
        m_locked = false;
    }
};
