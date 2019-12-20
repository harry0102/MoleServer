#ifndef _SERVER_SERVICE_MANAGER_H_INCLUDE_
#define _SERVER_SERVICE_MANAGER_H_INCLUDE_

class ServerServiceManager
{
public:
	/// ���캯��
	ServerServiceManager() {}
	/// ��������
	virtual ~ServerServiceManager() {}

	/// �����������
	virtual void SetGamePlayerCount(int count) = 0;
	/// �õ��������
	virtual int GetGamePlayerCount(void) = 0;
	/// ���÷����ʶ
	virtual void SetRoomMarking(int marking) = 0;
	/// �õ������ʶ
	virtual int GetRoomMarking(void) = 0;
	/// ������Ϸ����
	virtual void SetGameName(const char* name) = 0;
	/// �õ���Ϸ����
	virtual const char* GetGameName(void) = 0;
	/// ������Ϸ�ͻ����������
	virtual void SetGameModuleName(const char* name) = 0;
	/// �õ���Ϸ�ͻ����������
	virtual const char* GetGameModuleName(void) = 0;
	/// ���÷�����Ϸ����
	virtual void SetRoomGameType(enStartMode type) = 0;
	/// �õ�������Ϸ����
	virtual enStartMode GetRoomGameType(void) = 0;
	/// ���÷�����С���뱶��
	virtual void SetRoomLastDouble(int pdouble) = 0;
	/// �õ�������С���뱶��
	virtual int GetRoomLastDouble(void) = 0;
	/// ���÷������˿ں�
	virtual void SetServerPort(int pport) = 0;
	/// �õ��������˿ں�
	virtual int GetServerPort(void) = 0;

	//�����ӿ�
public:
	//��������
	virtual void * CreateTableFrameSink(void) = 0;
	//��������
	virtual void * CreateAndroidUserItemSink(void) = 0;
};

#endif
