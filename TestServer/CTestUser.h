#pragma once
#include "CGameUser.h"

class CTestUser : public CGameUser
{
public:
	CTestUser();
	~CTestUser();

	void OnAccept() override;
	void OnDisconnect() override;
};