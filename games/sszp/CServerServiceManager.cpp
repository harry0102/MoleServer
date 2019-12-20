#include "CServerServiceManager.h"

#include "cserverlogicframe.h"
#include "crobotlogicframe.h"

/// ���캯��
CServerServiceManager::CServerServiceManager():m_serverport(0)
{

}

/// ��������
CServerServiceManager::~CServerServiceManager()
{

}

/// �����������
void CServerServiceManager::SetGamePlayerCount(int count)
{
	m_maxPlayerCount = count;
}

/// �õ��������
int CServerServiceManager::GetGamePlayerCount(void)
{
	return m_maxPlayerCount;
}

/// ���÷����ʶ
void CServerServiceManager::SetRoomMarking(int marking)
{
	m_GameMarking = marking;
}

/// �õ������ʶ
int CServerServiceManager::GetRoomMarking(void)
{
	return m_GameMarking;
}

/// ������Ϸ����
void CServerServiceManager::SetGameName(const char* name)
{
	m_GameName = name;
}

/// �õ���Ϸ����
const char* CServerServiceManager::GetGameName(void)
{
	return m_GameName.c_str();
}

/// ������Ϸ�ͻ����������
void CServerServiceManager::SetGameModuleName(const char* name)
{
	m_GameModuleName = name;
}

/// �õ���Ϸ�ͻ����������
const char* CServerServiceManager::GetGameModuleName(void)
{
	return m_GameModuleName.c_str();
}

/// ���÷�����С���뱶��
void CServerServiceManager::SetRoomLastDouble(int pdouble)
{
	m_RoomLastDouble = pdouble;
}

/// �õ�������С���뱶��
int CServerServiceManager::GetRoomLastDouble(void)
{
	return m_RoomLastDouble;
}

//��������
void * CServerServiceManager::CreateTableFrameSink(void)
{
	//��������
	CServerLogicFrame * pTableFrameSink=NULL;

	try
	{
		pTableFrameSink=new CServerLogicFrame();
		if (pTableFrameSink==NULL) throw "���Ӵ���ʧ��";

		return pTableFrameSink;
	}
	catch (...)
	{
		printf("���Ӵ���ʧ�ܣ�\n");
	}

	//�������
	SafeDelete(pTableFrameSink);

	return NULL;
}

//��������
void * CServerServiceManager::CreateAndroidUserItemSink(void)
{
	//��������
	CRobotLogicFrame * pRobotFrameSink=NULL;

	try
	{
		pRobotFrameSink=new CRobotLogicFrame();
		if (pRobotFrameSink==NULL) throw "�����˴���ʧ��";

		return pRobotFrameSink;
	}
	catch (...)
	{
		printf("�����˴���ʧ�ܣ�\n");
	}

	//�������
	SafeDelete(pRobotFrameSink);

	return NULL;
}
