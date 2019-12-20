#ifndef _GAME_FRAME_MANAGER_H_INCLUDE_
#define _GAME_FRAME_MANAGER_H_INCLUDE_

#include "../../include/Common/defines.h"
#include "../../include/molnet/Network.h"

#include "gameframe/DBOperator.h"
#include "gameframe/CPlayer.h"

#include <string.h>

using namespace network;

class GameFrameManager : public Singleton<GameFrameManager>
{
public:
	/// ���캯��
	GameFrameManager();
	/// ��������
	~GameFrameManager();

	/// ���ڴ�����յ���������Ϣ
	void OnProcessNetMes(uint32 connId,CMolMessageIn *mes);

	/// ���ڴ����������������Ϣ
	void OnProcessConnectedNetMes(uint32 connId);

	/// ���ڴ������ڶϿ�����������Ϣ
	void OnProcessDisconnectNetMes(uint32 connId);

	/// ���������Ϣ
	void UpdatePlayerInfo(Player *pPlayer);
	/// ���·�����Ϣ
	void UpdateGameRoomInfo(void);
    /// ���������Ϣ
    void OnProcessUserInfo(CPlayer *pPlayer);
    /// ��Ҷ����ػط���
    bool OnProcessReEnterRoomMes(uint32 connId);
    /// �½�һ���µ����
    CPlayer* CreateNewPlayer(void);
    /// ����ָ����ҵ�½�ɹ�����Ϣ
    void SendPlayerLoginSuccess(CPlayer *pPlayer);
    /// ���ڴ����û����뷿��
    bool JoinPlayerToGameRoom(CPlayer *pPlayer,int pRoomIndex=-1,int pChairIndex=-1,bool isQueue=true);

private:
	/// ���ڴ����û���¼��Ϣ
	void OnProcessUserLoginMes(uint32 connId,Json::Value &mes);
    /// �ȴ�����Ϸ�����Ϣ
    void OnProcessFrameMes(uint32 connId,Json::Value &mes);
    /// ���ڴ����û�׼����Ϣ
    void OnProcessGameReadyMes(uint32 connId,Json::Value &mes);
    /// ���ڴ����û��뿪��Ϣ
    void OnProcessGameLeaveRoomMes(uint32 connId,Json::Value &mes);

public:
	/// ����һ����ҵ��Ŷ��б���
	void AddQueueList(CPlayer *connId);
	/// ɾ��һ����Ҵ��Ŷ��б���
	void DelQueueList(uint32 connId);
	/// �����Ŷ�����б�
	void UpdateQueueList(void);
	/// ���»�����
	void UpdateRobot(void);
	/// ����Ŷ��б�
	void ClearQueueList(void);

public:
	/// ���͵�ǰ�Ŷ�����
	void SendQueuingCount(void);
	/// �õ���ǰ�Ŷ�����
    int GetCurrentQueuePlayerCount(void);
	/// �õ���ǰ�Ŷ�����
	int GetQueueRealPlayerCount(void);
	/// �õ���ǰ�Ŷӻ�������
	int GetQueueRobotPlayerCount(void);

private:
    /// ����������ϵ�Ǯ
    void UpdatePlayerMoney(Player *pPlayer);
    /// ���ڴ����û����뷿��
    bool AddPlayerInServer(CPlayer *pPlayer,int pRoomIndex,int pChairIndex,bool isQueue,bool isGaming);

private:
	std::map<uint32,CPlayer*> m_PlayerQueueList;                 /**< ����Ŷ��б� */
	Mutex m_PlayerQueueListLock;                                 /**< ����Ŷ��б��� */
};

#define ServerGameFrameManager GameFrameManager::getSingleton()

#endif
