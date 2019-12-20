#ifndef _C_COMMON_SOCKET_H_INCLUDE_
#define _C_COMMON_SOCKET_H_INCLUDE_

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

class CMolMessageOut;

class CCommonSocket
{
public:
	/// �ر�����
	virtual void CloseConnect(void) = 0;
	/// ����ָ���ķ�����
	virtual bool Connect(std::string ipaddress,int port) = 0;
	/// ��������
	virtual int Send(CMolMessageOut &msg) = 0;
	/// ���ص�ǰ����״̬
	virtual ConnectState GetConnectState(void) = 0;
	/// ��⵱ǰ�Ƿ���������
	virtual bool IsConnected(void) = 0;
};

#endif
