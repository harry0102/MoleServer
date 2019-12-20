#ifndef _C_GAME_LOGIC_FRAME_H_INCLUDE_
#define _C_GAME_LOGIC_FRAME_H_INCLUDE_

#include "cdefines.h"
#include "GameLogic.h"
#include "czhuangmanager.h"

#include <map>
#include <vector>

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
	/// �õ����ֵ
	void GetGamingResult(void);
	/// ����������ҵ���Ӯֵ
	int64 getWinMoney(void);
	//����ָ�����Ӯ�˶���Ǯ
	int64 getWinMoneyByUserId(int userid,int64 *lRevenue);
	//�õ�ָ�����ָ���������Ӯֵ
	int64 getWinMoneyByUserIdAndArea(int userid,YanZhuType type,int64 *lRevenue);
	/// ��Ϸ����
	void GameEnd(void);
	int64 GetAllUserJettons(void);
	int64 getusertotalresult(void);
	int masterwinorlost(bool iswin,int64 pMaxMoney=-1);
	int GetGamingResultByUserId(int userid,int64 pMoney);

private:
	Room *m_g_GameRoom;                                  /**< ��Ϸ���� */

private:
	std::map<YanZhuType,int>       m_carBeiLv;               /**< ���͵ı��� */
	GameState                      m_GameState;
	CZhuangManager                 m_ZhuangManager;
	CXiaZhuManager                 m_BeiLvXiaZhuManager;
	std::map<int,CXiaZhuManager>   m_UserXiaZhuManager;
	std::vector<tagGamingRecord>   m_gamerecords;
	int                            m_curGamingResult;

	int                            m_timegamestart;
	int                            m_timexiazhu;
	int                            m_timekaipai;
	int                            m_timejiesuan;
	int                            m_timecurlost;

	bool                           m_issupercontrol;
	bool                           m_issuperZhuangWinControl;
	int                            m_supercontroltype;

	int                            m_curMonthDay;
	int                            m_curJvHao;
	std::string                    m_curStrJvHao;

	tagSuperUserControl            m_SuperUserControl;
	int64                          m_maxtotaljettons,m_maxpeoplejettons;
};


#endif
