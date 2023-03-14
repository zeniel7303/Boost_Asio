#include "CTestServer.h"

int main()
{
    CServerSetting setting;
    // session 및 gameUser 갯수
    setting.SetSessionCount(10000);
    // Acceptor port 설정
    // AddAddress는 여러개 가능하다.
    setting.AddAddress(30001);
    setting.AddAddress(30002);

    CTestServer testServer(&setting);

    return 0;
}