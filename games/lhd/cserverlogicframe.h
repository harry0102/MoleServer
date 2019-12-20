#ifndef _C_GAME_LOGIC_FRAME_H_INCLUDE_
#define _C_GAME_LOGIC_FRAME_H_INCLUDE_

#include "cdefines.h"
#include "GameLogic.h"

#include <map>
#include <string>
#include <vector>

#define IDD_TIMER_GAME_STARTING   IDD_TIMER_GAME_START+1                   // ��Ϸ����ʼ��ʱ��
#define IDD_TIMER_GAME_XIAZHU     IDD_TIMER_GAME_START+2                   // ��ע
#define IDD_TIMER_GAME_KAIPAI     IDD_TIMER_GAME_START+3                   // ����
#define IDD_TIMER_GAME_JIESUAN    IDD_TIMER_GAME_START+4                   // ����

struct tagGameRecord
{
	tagGameRecord():cardtype(CARTYPE_NULL) {}
	tagGameRecord(tagCardType pcardtype,std::string strjh)
		: cardtype(pcardtype)
	{
		strncpy(strjvhao,strjh.c_str(),256);
	}

	char        strjvhao[256];
	tagCardType cardtype;
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
	int64 GetRectJettonTotal(tagCardType pCarType);
	/// ͳ�Ƹ����������ע�ܺ�
	int64 GetRectJettonTotal2(tagCardType pCarType);
	/// ͳ��ָ����ҵ���ע�ܶ�
	int64 GetUserJettonTotal(uint32 pUserId);
	/// �õ����ֵ
	tagCardType GetGamingResult(void);
	/// �õ�ָ��ֵ����Ϸ���
	tagCardType GetTheGamingResult(tagCardType pCardType);
	/// ͳ��ָ����ҵ���ע�ܶ�
	int64 GetUserJettonTotal(uint32 pUserId,tagCardType pCarType);
	/// ��Ϸ����
	void GameEnd(void);

private:
	Room *m_g_GameRoom;                                  /**< ��Ϸ���� */

private:
	std::map<tagCardType,float>       m_carBeiLv;               /**< ���͵ı��� */
	std::map<int32,std::map<tagCardType,int64> >      m_xiazhus;          /**< �����˵���ע */
	GameState                      m_GameState;
	CGameLogic                     m_GameLogic;

	int                            m_timegamestart;
	int                            m_timexiazhu;
	int                            m_timekaipai;
	int                            m_timejiesuan;
	tagCardType                    m_curGamingResult;
	uint8                           m_curGamingCard[2];
	std::vector<tagGameRecord>     m_gamerecords;
	bool                           m_issupercontrol;

	int                            m_curMonthDay;
	int                            m_curJvHao;
	std::string                    m_curStrJvHao;
};


#endif
