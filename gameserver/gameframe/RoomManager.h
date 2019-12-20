#ifndef _ROOM_MANAGER_H_INCLUDE_
#define _ROOM_MANAGER_H_INCLUDE_

#include "../../include/molnet/Network.h"

#include "CRoom.h"
#include "CPlayer.h"

#include <vector>

class RoomManager : public Singleton<RoomManager>
{
public:
	RoomManager();
	~RoomManager();

	/// ������еķ���
	void ClearAllRooms(void);

	/// ���һ�����䵽�����������
	void AddRoom(CRoom *pRoom);
	/// ���һ����ҵ�������
	bool AddPlayer(CPlayer *pPlayer,int roomIndex=-1,int chairIndex=-1,bool isQueue=true,bool isGaming=true);
	/// ���һ���Թ���ҵ�������
	bool AddLookOnPlayer(CPlayer *pPlayer,int roomIndex,int chairIndex);
	/// �ı�һ����ҵ���Ϸ����
	bool ChangeGameRoom(CPlayer *pPlayer);
	/// �ӷ��������ָ�������
	void ClearPlayer(CPlayer *pPlayer);
	/// �ӵ�ǰ������ɾ��ָ�������
	bool DeletePlayer(CPlayer *pPlayer);
	/// �������е���Ϸ����
	void ResetAllGameRooms(void);
	/// ���ݷ���ID�ŵõ�����
	CRoom* GetRoomById(int id);
	/// �õ�һ���շ���
	CRoom* GetEmptyRoom(void);
	/// �õ�һ��δ������Ϊ�յķ���
	CRoom* GetUnderFullRoom(void);
	/// �õ�һ�����еķ���
	CRoom* GetFreeRoom(void);
	/// �õ���ǰϵͳ�еķ����б�
	inline std::vector<CRoom*>& GetRoomList(void) { return m_RoomList; }
	/// �õ���ǰϵͳ�з�������
	inline int GetRoomCount(void) { return (int)m_RoomList.size(); }
	/// �õ���ǰ�����л��ж�����
	int GetRoomPlayerCount(void);
	/// ��ס��ǰ����
	inline void LockRoomList(void) { m_RoomLock.Acquire(); }
	/// ������ǰ����
	inline void UnlockRoomList(void) { m_RoomLock.Release(); }

	/// ���ڴ�����յ���������Ϣ
	void OnProcessNetMes(uint32 connId,Json::Value &mes);
	/// ���ڴ������ڶϿ�����������Ϣ
	void OnProcessDisconnectNetMes(uint32 connId);
	/// ���ڴ�����Ҹı���Ϸ����
	void OnProcessChangeGameRoomMes(uint32 connId);

	///ʱ���¼�
	bool OnEventTimer(uint32 dwTimerID, uint32 wBindParam);

private:
	std::vector<CRoom*> m_RoomList;           /**< �����б� */
	Mutex m_RoomLock;                      /**< ���ڱ�����ǰ�����������еķ��� */
};

#define ServerRoomManager RoomManager::getSingleton()


#endif
