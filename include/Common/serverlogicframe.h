#ifndef _SERVER_LOGIC_FRAME_H_INCLUDE_
#define _SERVER_LOGIC_FRAME_H_INCLUDE_

#include "ccommon.h"

class Room;
class CMolMessageIn;

class ServerLogicFrame
{
public:
	/// ���캯��
	ServerLogicFrame() {}
	/// ��������
	virtual ~ServerLogicFrame() {}

	/// ���õ�ǰӦ�÷���
	virtual void SetGameRoom(Room* pRoom) = 0;
	/// ���ڴ����û���ʼ��Ϸ��ʼ��Ϣ
	virtual void OnProcessPlayerGameStartMes() = 0;
	/// ���ڴ����û�������Ϸ��������Ϣ
	virtual void OnProcessPlayerRoomMes(int playerId,Json::Value &mes) = 0;
	/// �����û����뷿����Ϣ
	virtual void OnProcessEnterRoomMsg(int playerId) = 0;
	/// �����û��뿪������Ϣ
	virtual void OnProcessLeaveRoomMsg(int playerId) = 0;
	/// �����û�����������Ϣ
	virtual void OnProcessReEnterRoomMes(int playerId) = 0;
	/// �����û�������Ϣ
	virtual void OnProcessOfflineRoomMes(int playerId) = 0;
	/// �����û���ʱ����Ϣ
	virtual void OnProcessTimerMsg(int timerId,int curTimer) = 0;
};

#endif
