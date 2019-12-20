#ifndef _C_GAME_LOGIC_FRAME_H_INCLUDE_
#define _C_GAME_LOGIC_FRAME_H_INCLUDE_

#include "cdefines.h"
#include "GameLogic.h"
#include "czhuangmanager.h"
#include <map>
#include <vector>

#define IDD_TIMER_GAME_STARTING   IDD_TIMER_GAME_START+1                   // ��Ϸ����ʼ��ʱ��
#define IDD_TIMER_GAME_XIAZHU     IDD_TIMER_GAME_START+2                   // ��ע
#define IDD_TIMER_GAME_KAIPAI     IDD_TIMER_GAME_START+3                   // ����
#define IDD_TIMER_GAME_JIESUAN    IDD_TIMER_GAME_START+4                   // ����

struct tagGamingRecord
{
	tagGamingRecord():decCardType(CARTYPE_NULL) {}
	tagGamingRecord(tagCarType pCardType,std::string jhstr)
		: decCardType(pCardType)
	{
		strncpy(JvHaoStr,jhstr.c_str(),256);
	}

	tagCarType decCardType;
	char JvHaoStr[256];
};

class CServerLogicFrame : public ServerLogicFrame
{
public:
	/// ���캯��
	CServerLogicFrame();
	/// ��������
	~CServerLogicFrame();

	/// ���õ�ǰӦ�÷���
	virtual void SetGameRoom(Room* pRoom) { m_g_GameRoom = pRoom; }
	/// ���ڴ����û���ʼ��Ϸ��ʼ��Ϣ
	virtual void OnProcessPlayerGameStartMes();
	/// ���ڴ����û�������Ϸ��������Ϣ
	virtual void OnProcessPlayerRoomMes(int playerId,Json::Value &mes);
	/// �����û����뷿����Ϣ
	virtual void OnProcessEnterRoomMsg(int playerId);
	/// �����û��뿪������Ϣ
	virtual void OnProcessLeaveRoomMsg(int playerId);
	/// �����û�����������Ϣ
	virtual void OnProcessReEnterRoomMes(int playerId);
	/// �����û�������Ϣ
	virtual void OnProcessOfflineRoomMes(int playerId);
	/// �����û���ʱ����Ϣ
	virtual void OnProcessTimerMsg(int timerId,int curTimer);

private:
	/// �������������
	void LoadServerConfig(void);
	/// ͳ�Ƹ����������ע����Ӯ
	int64 GetRectJettonTotal(tagCarType pCarType,bool isRobot);
	int64 GetRectJettonTotal2(tagCarType pCarType);
	/// ͳ��ָ����ҵ���ע�ܶ�
	int64 GetUserJettonTotal(uint32 pUserId);
	int64 getusertotalresult(void);
	int64 getusertotalresultbytype(tagCarType pCarType);
	int64 GetUserRectJetton(uint32 pUserId,tagCarType pCarType);
	int getLastMoneyResult(int64 pMoney,bool isRobot,bool isWinOk=false);
	int getLastMoneyResult2(int64 pMoney,bool isWinOk=false);
	int getLastRobtolMoneyResult(int64 pMoneystart,int64 pMoneyTwo);
	/// �õ����ֵ
	int GetGamingResult(void);
	/// ��Ϸ����
	void GameEnd(void);
	void OnProcessOpenRate(void);

private:
	Room *m_g_GameRoom;                                  /**< ��Ϸ���� */

private:
	std::map<tagCarType,int>       m_carBeiLv;               /**< ���͵ı��� */
	std::map<int32,std::map<tagCarType,int64> >      m_xiazhus;          /**< �����˵���ע */
	GameState                      m_GameState;
	CGameLogic                     m_GameLogic;
	CZhuangManager                 m_ZhuangManager;

	int                            m_timegamestart;
	int                            m_timexiazhu;
	int                            m_timekaipai;
	int                            m_timejiesuan;
	int							   m_timecurlost;
	bool                           m_issupercontrol;
	int                            m_superzhuangstate;
	tagCarType                     m_supercontroltype;

	int                            m_curGamingResult;
	std::vector<tagGamingRecord>   m_gamerecords;

	int                            m_curMonthDay;
	int                            m_curJvHao;
	std::string                    m_curStrJvHao;

	bool                           m_isEnableOpenRate;
	std::map<int,std::vector<tagCarType> >       m_openrates;
};

#endif
