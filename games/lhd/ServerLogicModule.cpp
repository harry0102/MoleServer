#include "ServerLogicModule.h"

#include "CServerServiceManager.h"

//ȫ�ֱ���
static CServerServiceManager			g_ServerServiceManager;				//�������

//����������
extern "C"
{
    void * CreateServerServiceManager(void)
    {
        return &g_ServerServiceManager;
    }
}
