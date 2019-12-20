#ifndef _ROOM_H_INCLUDE_
#define _ROOM_H_INCLUDE_

#include <vector>
#include <string>
#include <map>

#include "Player.h"
#include "ccommon.h"

// ��󷿼�����
#define ROOM_MAX_PLAYERS  500

/**
 * ��������
 */
enum RoomType
{
	ROOMTYPE_BISAI = 0,                  // ��������
	ROOMTYPE_JIFEN,                      // ���ַ���
	ROOMTYPE_JINBIN,                     // ��ҷ���
	ROOMTYPE_LIANXI,                     // ��ϰ����
	ROOMTYPE_NULL
};

/**
 * ���䵱ǰ״̬
 */
enum RoomState
{
	ROOMSTATE_WAITING = 0,             // �ȴ���
	ROOMSTATE_GAMING,                  // ��Ϸ��
	ROOMSTATE_NULL
};

//��������
enum enScoreKind
{
	enScoreKind_Win,					//ʤ
	enScoreKind_Lost,					//��
	enScoreKind_Draw,					//��
	enScoreKind_Flee,					//��
	enScoreKind_Service,				//����
	enScoreKind_Present,				//����
};

class Player;
class ServerLogicFrame;

class Room
{
public:
	/// ���÷���ID
	virtual void SetID(int id) = 0;
	/// �õ�����ID
	virtual int GetID(void) = 0;
	/// ���÷���������Ϸ����
	virtual void SetGameType(int type) = 0;
	/// �õ�����������Ϸ����
	virtual int GetGameType(void) = 0;
	/// ���÷���������Ϸ
	virtual void SetGameId(int gameid) = 0;
	/// �õ�����������Ϸ
	virtual int GetGameId(void) = 0;
	/// ���÷����ʶ
	virtual void SetRoomMarking(int marking) = 0;
	/// �õ������ʶ
	virtual int GetRoomMarking(void) = 0;
	/// ������Ϸ�߼�����ӿ�
	virtual void SetServerLogicFrame(ServerLogicFrame *frame) = 0;
	/// ������Ϸ�߼�����ӿ�
	virtual ServerLogicFrame* GetServerLogicFrame(void) = 0;
	/// ���÷�������
	virtual void SetRoomType(RoomType type) = 0;
	/// �õ���������
	virtual RoomType GetRoomType(void) = 0;
	/// ���÷����������
	virtual void SetMaxPlayer(int max) = 0;
	/// �õ������������
	virtual int GetMaxPlayer(void) = 0;
	/// ���÷���״̬
	virtual void SetRoomState(RoomState state) = 0;
	/// �õ�����״̬
	virtual RoomState GetRoomState(void) = 0;
	/// ���÷�����Ϸ����
	virtual void SetRoomGameType(enStartMode type) = 0;
	/// �õ�������Ϸ����
	virtual enStartMode GetRoomGameType(void) = 0;
	/// ���÷�������
	virtual void SetName(std::string name) = 0;
	/// �õ���������
	virtual std::string GetName(void) = 0;
	/// ������Ϸ��ˮֵ
	virtual void SetChouShui(float cs) = 0;
	/// �õ���Ϸ��ˮֵ
	virtual float GetChouShui(void) = 0;
	/// ������Ϸ��Ԫ����
	virtual void SetGamePielement(int pielement) = 0;
	/// �õ���Ϸ��Ԫ����
	virtual int GetGamePielement(void) = 0;
	/// ������Ϸ��Ҫ�����ٽ��
	virtual void SetLastMoney(unsigned int money) = 0;
	/// �õ���Ϸ��Ҫ�����ٽ��
	virtual unsigned int GetLastMoney(void) = 0;
	/// ��ǰ�����Ƿ�Ϊ��
	virtual bool IsEmpty(void) = 0;
	/// �õ�ָ�����������
	virtual Player* GetPlayer(int index) = 0;
	/// �õ���ʾʱָ�����������(�ͻ���ʹ��)
	virtual int SwitchViewChairID(int chairIndex) = 0;
	/// �õ���ǰ��������
	virtual int GetPlayerCount(void) = 0;
	/// �õ�׼����״̬����ҵ�����
	virtual int GetReadyPlayerCount(void) = 0;
	/// ���õ�ǰ����ķ���
	virtual void SetMaster(int playerId) = 0;
	/// �õ���ǰ����ķ���������Ϊ��ǰ��һ׼����ʼ��Ϸ�����
	virtual int GetMaster(void) = 0;
	/// �õ������е�ǰ�����
	virtual int GetCurrentPlayer(void) = 0;
	/// �����û�����ID�õ��û��ڷ����е�ID
	virtual int GetPlayerRoomId(uint32 connId) = 0;
	/// �õ���ǰ����ָ��״̬����Ҹ���
	virtual int GetPlayerCount(PlayerState state) = 0;
	/// ���õ�ǰ����������ҵ�״̬
	virtual void SetAllPlayerState(PlayerState state) = 0;
	/// ����ָ����ҵ�״̬
	virtual void SetPlayerState(int index,PlayerState state) = 0;
	/// ���÷����е�ǰ���
	virtual void SetCurrentPlayer(int playerId) = 0;
	/// �õ���ǰ��������һ�����
	virtual int GetCurNextPlayer(void) = 0;
	/// �õ���ǰ��������һ���������
	virtual int GetCurNextWorkPlayer(void) = 0;
	/// ���÷����������
	virtual void SetEnterPassword(std::string pwd) = 0;
	/// �õ������������
	virtual std::string getEnterPassword(void) = 0;
	/// ���÷�������ҷ�Χ
	virtual void SetEnterMoneyRect(int64 first,int64 second) = 0;
	/// �õ���������ҷ�Χ
	virtual void GetEnterMoneyRect(int64 *first,int64 *second) = 0;

	/// ��ⷿ���Ƿ��Ѿ���Ա
	virtual bool IsFull(void) = 0;
	/// ��ⷿ���Ƿ�ȫ��׼�����
	virtual bool IsReadyed(void) = 0;
	/// �����������id�õ�����ڵ�ǰ�����е�����
	virtual int GetPlayerIndex(uint32 connId) = 0;
	/// �õ���ǰ�����е����������
	virtual inline int GetLostPlayerCount(void) = 0;

	/// ��յ�ǰ��������
	virtual void Clear(void) = 0;
	/// �޳�ָ������ң���������Ϸ����ʱ������޳�
	virtual bool EliminatePlayer(Player *pPlayer) = 0;
	/// д���û�����
	virtual bool WriteUserScore(int wChairID, int64 lScore, int64 lRevenue, enScoreKind ScoreKind,int64 pAgentmoney=0,bool isCumulativeResult=true,int64 pcurJetton=0,const char* pgametip="") = 0;
	/// ��Ϸ����ʱ����
	virtual void GameEnd(bool isupdateuserdata=true) = 0;
	/// ��Ϸ��ʼ�ǵ���
	virtual void GameStart(void) = 0;
    /// ����ָ����ҵ�����
    virtual void UpdateUserScore(Player *pPlayer) = 0;

	/// ��⵱ǰ�����Ӯ���(2:��ʾ���Ӯ�ˣ�1����ʾ������ˣ�0����ʾ���䲻Ӯ�����
	virtual int IsUserWin(int64 *result = NULL, int64 *sysMax = NULL, int64 *sysMin = NULL, bool isUpdate = false) = 0;
	/// ���ָ����ҵ���Ӯ���(1:��ʾ������ˣ�2����ʾ���Ӯ�ˣ�0����ʾ���䲻Ӯ�����-1�������ƣ�
	virtual int IsSuperPlayerWin(Player *pPlayer,int64 *curresult=NULL,int64 *decResult=NULL) = 0;

	/// ��ָ������ҷ�������
	virtual void SendTableMsg(int index,Json::Value &msg) = 0;
	/// ��ָ������ҷ����Թ�����
	virtual void SendLookOnMes(int index,Json::Value &msg) = 0;

	/// ��ʼһ����ʱ��
	virtual bool StartTimer(int timerId,int space) = 0;
	/// �ر�һ����ʱ��
	virtual void StopTimer(int id) = 0;
	/// �ر����еĶ�ʱ��
	virtual void StopAllTimer(void) = 0;
	//virtual std::string WideCharConverToUtf8(CString& str)=NULL;

	/// ��ӡ��־
	virtual void Room_Log(enLogLevel loglevel,std::string log) = 0;
	/// �õ��������ò���
	virtual std::string GetRoomParameters(void) = 0;
};

#endif
