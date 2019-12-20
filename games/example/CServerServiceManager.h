#ifndef _C_SERVER_SERVICE_MANAGER_H_INCLUDE_
#define _C_SERVER_SERVICE_MANAGER_H_INCLUDE_

#include "cdefines.h"

/**
 * ���ڹ���������Ϸ�߼�
 */
class CServerServiceManager : public ServerServiceManager
{
public:
	/// ���캯��
	CServerServiceManager();
	/// ��������
	virtual ~CServerServiceManager();

	/// �����������
	virtual void SetGamePlayerCount(int count);
	/// �õ��������
	virtual int GetGamePlayerCount(void);
	/// ���÷����ʶ
	virtual void SetRoomMarking(int marking);
	/// �õ������ʶ
	virtual int GetRoomMarking(void);
	/// ������Ϸ����
	virtual void SetGameName(const char* name);
	/// �õ���Ϸ����
	virtual const char* GetGameName(void);
	/// ������Ϸ�ͻ����������
	virtual void SetGameModuleName(const char* name);
	/// �õ���Ϸ�ͻ����������
	virtual const char* GetGameModuleName(void);
	/// ���÷�����Ϸ����
	virtual void SetRoomGameType(enStartMode type) { m_GameStartMode = type; }
	/// �õ�������Ϸ����
	virtual enStartMode GetRoomGameType(void) { return m_GameStartMode; }
	/// ���÷�����С���뱶��
	virtual void SetRoomLastDouble(int pdouble);
	/// �õ�������С���뱶��
	virtual int GetRoomLastDouble(void);
	/// ���÷������˿ں�
	virtual void SetServerPort(int pport) { m_serverport = pport; }
	/// �õ��������˿ں�
	virtual int GetServerPort(void) { return m_serverport; }

	//�����ӿ�
public:
	//��������
	virtual void * CreateTableFrameSink(void);
	//��������
	virtual void * CreateAndroidUserItemSink(void);

private:
	int m_maxPlayerCount;                         /**< ������� */
	int m_GameMarking;                            /**< ��Ϸ��ʶ */
	std::string m_GameName;                       /**< ��Ϸ���� */
	std::string m_GameModuleName;                 /**< ��Ϸ�ͻ���������� */
	enStartMode m_GameStartMode;                  /**< ��Ϸ��ʼģʽ */
	int m_RoomLastDouble;                         /**< ��Ϸ������С���뱶�� */
	int m_serverport;
};

#endif
