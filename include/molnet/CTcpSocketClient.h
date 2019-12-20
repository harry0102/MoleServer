#ifndef _C_TCP_SOCKET_CLIENT_H_INCLUDE_
#define _C_TCP_SOCKET_CLIENT_H_INCLUDE_

#include "common.h"
#include "Singleton.h"
#include "Mutex.h"
#include "ThreadStarter.h"
#include "MolNetMessage.h"
#include <string>
#include <list>

#define REV_SIZE      30000                     // �������ݵĻ����С

class CMolMessageIn;
class CMolMessageOut;
class CircularBuffer;

/**
* ��ǰsocket������״̬
*/
enum ConnectState
{
	NOCONNECT = 0,     // û������
	CONNECTTING,       // ������
	CONNECTED,         // ������
	MESPROCESS         // ������Ϣ����
};

class CMolTcpSocketClient : public ThreadBase
{
public:
	/// ���캯��
	CMolTcpSocketClient();
	/// ��������
	~CMolTcpSocketClient(void);

	/// �ر�����
	void CloseConnect(bool isShow=false);
	/// ����ָ���ķ�����
	bool Connect(std::string ipaddress,int port);
		/// ���ص�ǰ����״̬
	inline ConnectState GetConnectState(void) { return m_bConnectState; }
	/// ��������
	int Send(CMolMessageOut &msg);
	int Send(char *msg,uint32 len);
	int Sendhtml5(char *Bytes,uint32 len);
	/// ��⵱ǰ�Ƿ���������
	inline bool IsConnected(void)
	{
		return m_bConnectState != NOCONNECT ? true : false;
	}

	int GetNetMessage(NetMessage & mes);
	void ExitWorkingThread(void);

private:
	virtual bool run();
	/// �õ���Ϣ�б�
	inline std::list<MessageStru>* GetMesList(void)
	{
		return &_MesList;
	}
	/// ���һ����Ϣ���б���
	inline void PushMessage(MessageStru mes)
	{
		_mesLock.Acquire();
		_MesList.push_back(mes);
		//++_mesLock_count;
		_mesLock.Release();
	}
	/// �õ���ǰ��Ϣ����
	inline int GetMesCount(void)
	{
		return (int)_MesList.size();
	}
	/// �����Ϣ�б�
	void ClearMesList(void);

private:
	void ProcessSelect(void);
	void GameMainLoop(void);

private:
	int m_Socket;                 /**< ��ǰ�ؼ���socket��� */
	ConnectState m_bConnectState;    /**< ��ǰ������״̬ */
	CircularBuffer *m_ReadBuffer;  /**< �������ݻ����� */

	std::list<MessageStru> _MesList;
	Mutex _mesLock,_sendLock,m_ReadBufferLock;

	fd_set m_readableSet,m_writeableSet;
	fd_set m_exceptionSet;
	struct timeval lostHeartHintTime;
	volatile int sendedhearthintcount;

	volatile bool m_mainlooprunning;
	unsigned int remaining;
	unsigned short opcode;
	uint16 compress;
	uint32 mchecksum;
};

class CTcpSocketClientManager : public Singleton<CTcpSocketClientManager>
{
public:
	CTcpSocketClientManager();
	~CTcpSocketClientManager();

	bool addTcpSocketClient(CMolTcpSocketClient *pClient);
	bool delTcpSocketClient(CMolTcpSocketClient *pClient);
	void ExitWorkingThread(void);
	void deleteAllTcpSocketClient(void);

private:
	std::vector<CMolTcpSocketClient*> m_TcpSocketClients;
};

class CMolGameTcpSocketClient : public CMolTcpSocketClient,public Singleton<CMolGameTcpSocketClient>
{
public:
	CMolGameTcpSocketClient() {}
	virtual ~CMolGameTcpSocketClient() {}
};

class CMolChatTcpSocketClient : public CMolTcpSocketClient,public Singleton<CMolChatTcpSocketClient>
{
public:
	CMolChatTcpSocketClient() {}
	virtual ~CMolChatTcpSocketClient() {}
};

#define MolTcpSocketClient CMolGameTcpSocketClient::getSingleton()
#define MolChatTcpSocketClient CMolChatTcpSocketClient::getSingleton()

#endif
