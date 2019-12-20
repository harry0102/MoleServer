#ifndef _C_ROOM_H_INCLUDE_
#define _C_ROOM_H_INCLUDE_

#include "../../include/molnet/Network.h"
#include "../../include/Common/serverlogicframe.h"

#include "../../include/molnet/netallocator/NedAllocatedObject.h"

#include "CRoom.h"
#include "CPlayer.h"

class Player;
class Room;

class CRoom : public Room , public NedAllocatedObject
{
public:
	CRoom(RoomType type=ROOMTYPE_NULL);
	virtual ~CRoom();

	/// ���÷���ID
	virtual void SetID(int id) { m_Id = id; }
	/// �õ�����ID
	virtual int GetID(void) { return m_Id; }
	/// ���÷���������Ϸ����
	virtual void SetGameType(int type) { m_GameType = type; }
	/// �õ�����������Ϸ����
	virtual int GetGameType(void) { return m_GameType; }
	/// ���÷���������Ϸ
	virtual void SetGameId(int gameid) { m_gameid = gameid; }
	/// �õ�����������Ϸ
	virtual int GetGameId(void) { return m_gameid; }
	/// ���÷����ʶ
	virtual void SetRoomMarking(int marking) { m_RoomMarking = marking; }
	/// �õ������ʶ
	virtual int GetRoomMarking(void) { return m_RoomMarking; }
	/// ������Ϸ�߼�����ӿ�
	virtual void SetServerLogicFrame(ServerLogicFrame *frame);
	/// �õ���Ϸ�߼�����ӿ�
	virtual ServerLogicFrame* GetServerLogicFrame(void) { return m_ServerLogicFrame; }
	/// ���÷�������
	virtual void SetRoomType(RoomType type) { m_RoomType = type; }
	/// �õ���������
	virtual RoomType GetRoomType(void) { return m_RoomType; }
	/// ���÷����������
	virtual void SetMaxPlayer(int max)
	{
		if(max > ROOM_MAX_PLAYERS)
			m_MaxCount = ROOM_MAX_PLAYERS;
		else
			m_MaxCount = max;
	}
	/// �õ������������
	virtual int GetMaxPlayer(void) { return m_MaxCount; }
	/// ���÷���״̬
	virtual void SetRoomState(RoomState state) { m_RoomState = state; }
	/// �õ�����״̬
	virtual RoomState GetRoomState(void) { return m_RoomState; }
	/// ���÷�����Ϸ����
	virtual void SetRoomGameType(enStartMode type) { m_RoomGameType = type; }
	/// �õ�������Ϸ����
	virtual enStartMode GetRoomGameType(void) { return m_RoomGameType; }
	/// ���÷�������
	virtual void SetName(std::string name) { m_Name = name; }
	/// �õ���������
	virtual std::string GetName(void) { return m_Name; }
	/// ������Ϸ��ˮֵ
	virtual void SetChouShui(float cs) { m_RoomRevenue = cs; }
	/// �õ���Ϸ��ˮֵ
	virtual float GetChouShui(void) { return m_RoomRevenue; }
	/// ������Ϸ��Ԫ����
	virtual void SetGamePielement(int pielement) { m_Pielement = pielement; }
	/// �õ���Ϸ��Ԫ����
	virtual int GetGamePielement(void) { return m_Pielement; }
	/// ������Ϸ��Ҫ�����ٽ��
	virtual void SetLastMoney(unsigned int money) { m_RoomLastMoney = money; }
	/// �õ���Ϸ��Ҫ�����ٽ��
	virtual unsigned int GetLastMoney(void) { return m_RoomLastMoney; }
	/// ��ǰ�����Ƿ�Ϊ��
	virtual bool IsEmpty(void);
	/// �õ�ָ�����������
	virtual Player* GetPlayer(int index);
	/// �õ���ʾʱָ�����������(�ͻ���ʹ��)
	virtual int SwitchViewChairID(int chairIndex) { return 0; }
	/// �õ���ǰ��������
	virtual int GetPlayerCount(void);
	/// �õ�׼����״̬����ҵ�����
	virtual int GetReadyPlayerCount(void);
	/// ���õ�ǰ����ķ���
	virtual void SetMaster(int playerId);
	/// �õ���ǰ����ķ���������Ϊ��ǰ��һ׼����ʼ��Ϸ�����
	virtual inline int GetMaster(void) { return m_masterId; }
	/// �õ������е�ǰ�����
	virtual inline int GetCurrentPlayer(void) { return m_curPlayerId; }
	/// �����û�����ID�õ��û��ڷ����е�ID
	virtual int GetPlayerRoomId(uint32 connId);
	/// �õ���ǰ����ָ��״̬����Ҹ���
	virtual int GetPlayerCount(PlayerState state);
	/// ���õ�ǰ����������ҵ�״̬
	virtual void SetAllPlayerState(PlayerState state);
	/// ����ָ����ҵ�״̬
	virtual void SetPlayerState(int index,PlayerState state);
	/// ���÷����е�ǰ���
	virtual void SetCurrentPlayer(int playerId) { m_curPlayerId = playerId; }
	/// �õ���ǰ��������һ�����
	virtual int GetCurNextPlayer(void);
	/// �õ���ǰ��������һ���������
	virtual int GetCurNextWorkPlayer(void);
	/// ���÷����������
	virtual void SetEnterPassword(std::string pwd) { m_roomenterpwd = pwd; }
	/// �õ������������
	virtual std::string getEnterPassword(void) { return m_roomenterpwd; }
	/// ���÷�������ҷ�Χ
	virtual void SetEnterMoneyRect(int64 first,int64 second) { m_roomentermoneyfirst = first; m_roomentermoneysecond = second; }
	/// �õ���������ҷ�Χ
	virtual void GetEnterMoneyRect(int64 *first,int64 *second) { *first = m_roomentermoneyfirst; *second = m_roomentermoneysecond; }

	/// ������������е����
	void ClearAllPlayers(void);
	/// ���һ����ҵ���ǰ������
	int AddPlayer(CPlayer *pPlayer,int index=-1);
	/// ���һ���Թ���ҵ���ǰ������
	int AddLookOnPlayer(CPlayer *pPlayer,int index);
	/// �ӵ�ǰ���������ָ�������
	void ClearPlayer(CPlayer *pPlayer);
	/// �ӵ�ǰ������ɾ��ָ�������
	void DeletePlayer(CPlayer *pPlayer);
	/// �ӵ�ǰ���������ָ������ID�����
	void ClearPlayerById(int Id);
	/// ���ָ������Ƿ���ڵ�ǰ����
	bool IsExist(CPlayer *pPlayer);
	/// ���ָ�����Թ�����Ƿ���ڵ�ǰ������
	bool IsExistLookOn(CPlayer *pPlayer);
	/// ��ⷿ���Ƿ��Ѿ���Ա
	virtual bool IsFull(void);
	/// ��ⷿ���Ƿ�ȫ��׼�����
	virtual bool IsReadyed(void);
	/// �����������id�õ�����ڵ�ǰ�����е�����
	virtual int GetPlayerIndex(uint32 connId);
	/// �õ���ǰ�����е����������
	virtual inline int GetLostPlayerCount(void) { return (int)m_lostPlayerList.size(); }
	/// �����������б������
	void ClearLostPlayerById(int id);

	/// ��յ�ǰ��������
	virtual void Clear(void);
	/// �޳�ָ������ң���������Ϸ����ʱ������޳�
	virtual bool EliminatePlayer(Player *pPlayer);
	/// д���û�����
	virtual bool WriteUserScore(int wChairID, int64 lScore, int64 lRevenue, enScoreKind ScoreKind,int64 pAgentmoney=0,bool isCumulativeResult=true,int64 pcurJetton=0,const char* pgametip="");
	/// ��Ϸ����ʱ����
	virtual void GameEnd(bool isupdateuserdata=true);
	/// ��Ϸ��ʼ�ǵ���
	virtual void GameStart(void);
    /// ����ָ����ҵ�����
    virtual void UpdateUserScore(Player *pPlayer);

	/// ��⵱ǰ�����Ӯ���(2:��ʾ���Ӯ�ˣ�1����ʾ������ˣ�0����ʾ���䲻Ӯ�����
	virtual int IsUserWin(int64 *result = NULL, int64 *sysMax = NULL, int64 *sysMin = NULL, bool isUpdate = false);
	/// ���ָ����ҵ���Ӯ���(1:��ʾ������ˣ�2����ʾ���Ӯ�ˣ�0����ʾ���䲻Ӯ�����-1�������ƣ�
	virtual int IsSuperPlayerWin(Player *pPlayer,int64 *curresult=NULL,int64 *decResult=NULL);

	/// ��ָ������ҷ�������
	virtual void SendTableMsg(int index,Json::Value &msg);
	/// ��ָ������ҷ����Թ�����
	virtual void SendLookOnMes(int index,Json::Value &msg);

	/// ��ʼһ����ʱ��
	virtual bool StartTimer(int timerId,int space);
	/// �ر�һ����ʱ��
	virtual void StopTimer(int id);
	/// �ر����еĶ�ʱ��
	virtual void StopAllTimer(void);

	/// ��ӡ��־
	virtual void Room_Log(enLogLevel loglevel,std::string log);
	/// �õ��������ò���
	virtual std::string GetRoomParameters(void);

	///ʱ���¼�
	bool OnEventTimer(DWORD dwTimerID, DWORD wBindParam);

	/// ���ڴ�����յ���������Ϣ
	virtual void OnProcessNetMes(uint32 connId,Json::Value &mes);

	/// ���ڴ����û�׼�������Ϣ
	void OnProcessPlayerReadyMes(int playerId);
	/// ���ڴ����û���ʼ��Ϸ��ʼ��Ϣ
	void OnProcessPlayerGameStartMes(void);
	/// ���ڴ����û�������Ϸ��������Ϣ
	void OnProcessPlayerRoomMes(int playerId,Json::Value &mes);
	/// �����û����뷿����Ϣ
	void OnProcessEnterRoomMsg(int playerId);
	/// �����û��뿪������Ϣ
	void OnProcessLeaveRoomMsg(int playerId);
	/// �����û�������Ϣ
	void OnProcessOfflineRoomMes(int playerId);
	/// �����û�����������Ϣ
	void OnProcessReEnterRoomMes(int playerId);

	/// �Ƿ������Ϸ
	void OnProcessContinueGaming(void);

private:
	/// ��������ҷ�������
	void SendAllPlayerMsg(Json::Value &sg);
	/// ��ǰ����������ҷ����û��뿪��������Ϣ
	void AllPlayerLeveaServer(void);
	/// �������Թ���ҷ�������
	void SendAllLookOnPlayerMsg(Json::Value &msg);
	/// ���ָ���������Ƿ��ڵ�������б���
	bool IsExistLostPlayer(int index);
	/// ɾ�����еĶ�ʱ��
	void DeleteAllTimer(void);

	//ʱ�����
protected:
	CTimerItemArray					m_TimerItemActive;					//�����
	Mutex                           m_TimerLock;                        /**< ���ڱ�������еĶ�ʱ�� */

private:
	int m_Id;                   /**< ����ID */
	int m_GameType;             /**< ������������Ϸ���� */
	int m_gameid;               /**< ����������Ϸ */
	int m_RoomMarking;          /**< �����ʶ��һ�������Ķ˿ںű�ʾ */
	std::string m_Name;         /**< �������� */
	int m_MaxCount;             /**< �����������Ҹ��� */
	RoomType m_RoomType;        /**< �������� */
	RoomState m_RoomState;      /**< ���䵱ǰ״̬ */
	enStartMode m_RoomGameType; /**< ������Ϸ���� */
	float m_RoomRevenue;        /**< ����˰�� */
	int m_RoomLastMoney;        /**< ������С��� */
	int m_Pielement;            /**< ���䵥Ԫ���� */
	//int64 m_JackPot;            /**< ���� */

	std::string m_roomenterpwd; /**< ����������� */
	int64 m_roomentermoneyfirst,m_roomentermoneysecond;  /**< ��������ҷ�Χ */

	int m_masterId;             /**< ��ǰ����ID */
	int m_curPlayerId;          /**< ��ǰ���ID */

	CPlayer *m_PlayerList[ROOM_MAX_PLAYERS];       /**< ��������Ϸ����б� */
	std::map<int,std::vector<CPlayer*> > m_LookonPlayerList;       /**< �������Թ�����б� */
	std::vector<int> m_lostPlayerList;            /**< �����е�������б� */

	Mutex m_playersLock;                          /**< ������Ҳ��� */

	ServerLogicFrame* m_ServerLogicFrame;               /**< ���ڴ�����Ϸ�߼� */
};

#endif
