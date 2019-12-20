#ifndef _C_GAME_LOGIC_FRAME_H_INCLUDE_
#define _C_GAME_LOGIC_FRAME_H_INCLUDE_

#include "cdefines.h"
#include "GameLogic.h"
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
	/// ����ĳ�ֻ�ɫ�ܵ���ע��
	int64 ComputerCardColorTotalJetton(uint8 cardcolor);
	/// �õ�ָ������ܵ���ע
	int64 ComputerUserTotalJetton(uint32 pUserId);
	/// �õ�ָ�����ָ����ɫ����ע
	int64 ComputerUserCardColorTotalJetton(uint32 pUserId,uint8 cardcolor);
	/// �õ���ǰ׬Ǯ���ٵĻ�ɫ
	int ComputerCardColorTotalResult(int64 presult);
	/// �õ���ǰ����Ļ�ɫ
	int ComputerCardColorFailTotalResult(int64 *presult);
	/// ���������ע�õ�������
	uint8 GetResultCard(void);
	/// ������Ϸ
	void TradGame(void);
	/// �����������ע��¼
	void ClearJettonRecord(void);
	/// ������Ϸ����
	void LoadGameConfig(void);

private:
	Room *m_g_GameRoom;                                  /**< ��Ϸ���� */

	CGameLogic m_CGameLogic;
	GameState m_GameState;
	uint8 m_resultCard;                                   /**< ����� */
	int64 m_GamePielement;                                    /**< ��Ϸ������ */

	bool m_gamisrunning;

	std::map<uint8,float>     m_jettonTrad;                    /**< ��ɫ���� */
	std::map<uint8,int>       m_colorrecordcount;              /**< ��ɫ���ִ��� */
	std::map<uint32,tagJettons> m_userjettonresult;              /**< �û�Ѻע��� */
	std::vector<uint8>        m_cardrecord;                    /**< ��Ϸ��¼ */
	int                      m_gamejvcount;                   /**< ��Ϸ���� */

	float m_fangkuai, m_meihua, m_heitao, m_hongtao, m_wang;
	int m_timegamestart, m_timexiazhu, m_timekaipai, m_timejiesuan;
	int m_unitmoney;
	float m_choushui;
};

#endif
