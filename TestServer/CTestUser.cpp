#include "CTestUser.h"

CTestUser::CTestUser()
{
	
}

CTestUser::~CTestUser()
{

}

void CTestUser::OnAccept()
{
	printf("Accept \n");
}

void CTestUser::OnDisconnect()
{
	printf("Disconnect \n");
}

void CTestUser::OnTest()
{
	printf("Test \n");
}