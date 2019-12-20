#ifndef _PLAYER_MANAGER_H_INCLUDE_
#define _PLAYER_MANAGER_H_INCLUDE_

#include "../../include/molnet/Network.h"
#include "CPlayer.h"

#include <map>

class PlayerManager : public Singleton<PlayerManager>
{
public:
	PlayerManager();
	~PlayerManager();

	/// ���һ�������˵�ϵͳ��
	bool AddRobot(CPlayer *pPlayer);
	/// �õ����еĻ�����
	CPlayer* GetFreeRobot(void);
	/// �õ����л����˸���
	int GetFreeRobotCount(void);
	/// �õ��������ܵ���Ӯ
	int64 GetRobotTotalResult(void);

	/// ������е����
	void ClearAllPlayers(void);
	/// ���һ����ҵ�ϵͳ��
	bool AddPlayer(CPlayer *pPlayer);
	/// ������ҵ�����ID�������
	void Reset(CPlayer *pPlayer);
	/// ��ϵͳ��ɾ��ָ�������
	void ClearPlayer(CPlayer *pPlayer);
	/// ��ϵͳ��ժ��ָ�������
	void DeletePlayer(CPlayer *pPlayer);
	///// ��ϵͳ��ɾ��ָ������ID�����
	//void ClearPlayerByconnId(uint32 connId);
	/// ͨ���ͻ�������ID�õ��ͻ���
	CPlayer* GetPlayer(uint32 connId);
	/// ͨ���ͻ�������ID�õ��յ�½�Ŀͻ���
	CPlayer* GetNewPlayer(uint32 connId);
	/// ͨ�����ID�õ��ͻ���
	CPlayer* GetPlayerById(uint32 id);
	/// �õ�����ܵ���Ӯ
	int64 GetPlayerTotalResult(void);
	/// �õ���ǰϵͳ��Ҹ���
	inline int GetPlayerCount(void) { return (int)m_PlayerList.size(); }
	/// �õ���ǰϵͳ�����˸���
	inline int GetRobotCount(void) { return (int)m_RobotList.size(); }
	/// �õ���ǰϵͳ�ܵ��������
	inline int GetTotalCount(void) { return GetPlayerCount() + GetRobotCount() + GetLostPlayerCount(); }
	/// ������ǰ���
	inline void LockPlayerList(void) { m_PlayersLock.Acquire(); }
	/// ������ǰ���
	inline void UnlockPlayerList(void) { m_PlayersLock.Release(); }
	/// �õ���������б�
	inline std::map<uint32,CPlayer*>& GetPlayerList(void) { return m_PlayerList; }
	/// �õ����߻������б�
	inline std::map<uint32,CPlayer*>& GetRobotList(void) { return m_RobotList; }
	/// �õ���������б�
	inline std::map<uint32,CPlayer*>& GetLostPlayerList(void) { return m_LostPlayerList; }
	/// ���������������ʾ��λ��Ϣ
	void UpdatePlayersTableInfo(void);

	/// ������Ϣ����ǰ���ߵ��������
	void SendMsgToEveryone(Json::Value &msg);
	/// ������Ϣ������ָ����ҵ������������
	void SendMsgToOtherPlayer(uint32 connId,Json::Value &msg);
	/// ������Ϣ��ָ��ID�����
	void SendMsgToPlayer(uint32 UserID,Json::Value &msg);

	///ʱ���¼�
	bool OnEventTimer(uint32 dwTimerID, uint32 wBindParam);

	/// ���һ����ҵ������б���
	void AddLostPlayer(CPlayer *pPlayer);
	/// ����ID�õ��������
	CPlayer* GetLostPlayer(uint32 userId);
	/// ����IDɾ���������
	void DeleteLostPlayer(uint32 userId);
	/// �õ������������
	inline int GetLostPlayerCount(void) { return (int)m_LostPlayerList.size(); }
	/// �õ���⵽���Ѿ����ߵ�����б�
	void GetRealLostPlayerList(void);

private:
	std::map<uint32,CPlayer*> m_PlayerList;                 /**< ����б� */
	std::map<uint32,CPlayer*> m_RobotList;                  /**< �������б� */
	std::map<uint32,CPlayer*> m_LostPlayerList;             /**< ��������б� */

	Mutex m_PlayersLock;                                    /**< ������ǰ������� */
};

#define ServerPlayerManager PlayerManager::getSingleton()

#endif
