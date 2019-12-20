#ifndef _C_GAME_LOGIC_FRAME_H_INCLUDE_
#define _C_GAME_LOGIC_FRAME_H_INCLUDE_

#include "cdefines.h"
#include "GameLogic.h"
#include <map>
#include <vector>

#include "czhuangmanager.h"

#define IDD_TIMER_GAME_STARTING   IDD_TIMER_GAME_START+1                   // ��Ϸ����ʼ��ʱ��
#define IDD_TIMER_GAME_XIAZHU     IDD_TIMER_GAME_START+2                   // ��ע
#define IDD_TIMER_GAME_KAIPAI     IDD_TIMER_GAME_START+3                   // ����
#define IDD_TIMER_GAME_JIESUAN    IDD_TIMER_GAME_START+4                   // ����

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
	int64 GetRectJettonTotal(tagCardType pCarType,bool isYouRobits=false);
	/// ͳ��ָ����ҵ���ע�ܶ�
	int64 GetUserJettonTotal(uint32 pUserId);
	int64 getusertotalresult(void);
	int64 getuserswinandlostresult(void);
	int masterwinorlost(bool iswin, bool isYouRobits=true,int64 pLastMoney=0);

	/// �õ����ֵ
	int GetGamingResult(void);
	/// ��Ϸ����
	void GameEnd(void);

	std::string getCardTypeName(tagCardType ptype)
	{
		switch (ptype)
		{
		case CARDTYPE_QINGLONG:
		{
			return "����";
		}
			break;
		case CARDTYPE_BAIHU:
		{
			return "�׻�";
		}
			break;
		case CARDTYPE_ZHUQUE:
		{
			return "��ȸ";
		}
			break;
		case CARDTYPE_XUANWU:
		{
			return "����";
		}
			break;
		case CARDTYPE_XIAOBAILONG:
		{
			return "С����";
		}
			break;
		case CARDTYPE_XIAOLAOHU:
		{
			return "С�ϻ�";
		}
			break;
		case CARDTYPE_XIAOFENGHUANG:
		{
			return "С���";
		}
			break;
		case CARDTYPE_XIAOWUGUI:
		{
			return "С�ڹ�";
		}
			break;
		default:
			break;
		}

		return "";
	}

private:
	Room *m_g_GameRoom;                                  /**< ��Ϸ���� */

private:
	std::map<tagCardType,int>       m_carBeiLv;               /**< ���͵ı��� */
	std::map<int32,std::map<tagCardType,int64> >      m_xiazhus;          /**< �����˵���ע */
	GameState                      m_GameState;
	CGameLogic                     m_GameLogic;
	std::vector<int>               m_GameRecords;

	CZhuangManager                 m_ZhuangManager;

	tagCardType                    m_curDecCardType;
	bool                           m_isZhuangYing;
	int                            m_supercontroltype;
	bool                           m_issupercontrol;

	int                            m_timegamestart;
	int                            m_timexiazhu;
	int                            m_timekaipai;
	int                            m_timejiesuan;
	int                            m_timecurlost;
	int                            m_curGamingResult;
};

#endif
