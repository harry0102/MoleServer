#ifndef _ROBOT_LOGIC_FRAME_H_INCLUDE_
#define _ROBOT_LOGIC_FRAME_H_INCLUDE_

#include "ccommon.h"

class Player;
class Room;
class CMolMessageIn;

class RobotLogicFrame
{
public:
	/// ���캯��
	RobotLogicFrame() {}
	/// ��������
	virtual ~RobotLogicFrame() {}

	/// ���õ�ǰ���
	virtual void SetGamePlayer(Player* pPlayer,Room* pRoom) = 0;

	/// ���ڴ����û�׼�������Ϣ
	virtual void OnProcessPlayerReadyMes(int playerId) = 0;
	/// ���ڴ����û���ʼ��Ϸ��ʼ��Ϣ
	virtual void OnProcessPlayerGameStartMes() = 0;
	/// ���ڴ����û�������Ϸ��Ϣ
	virtual void OnProcessPlayerGameOverMes(void) = 0;
	/// ���ڴ����û�������Ϸ��������Ϣ
	virtual void OnProcessPlayerRoomMes(Json::Value &mes) = 0;
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
