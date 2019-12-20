#include "crobotlogicframe.h"
#include "GameLogic.h"

/// ���캯��
CRobotLogicFrame::CRobotLogicFrame() :m_g_myself(NULL), m_g_myselfRoom(NULL)
{

}

/// ��������
CRobotLogicFrame::~CRobotLogicFrame()
{

}

/// ���ڴ����û�׼�������Ϣ
void CRobotLogicFrame::OnProcessPlayerReadyMes(int playerId)
{
	assert(m_g_myself != NULL);
	if(m_g_myself == NULL) return;
}

/// ���ڴ����û���ʼ��Ϸ��ʼ��Ϣ
void CRobotLogicFrame::OnProcessPlayerGameStartMes()
{

}

/// ���ڴ����û�������Ϸ��Ϣ
void CRobotLogicFrame::OnProcessPlayerGameOverMes(void)
{

}

/// ���ڴ����û�������Ϸ��������Ϣ
void CRobotLogicFrame::OnProcessPlayerRoomMes(Json::Value &mes)
{
	assert(m_g_myself != NULL);
	if(m_g_myself == NULL) return;

	switch(mes["MsgSubId"].asInt())
	{
	case IDD_MESSAGE_ROOM_STARTXIAZHU:
		{
			m_g_myself->StartTimer(IDD_TIMER_GAME_XIAZHU1,rand()%6+2);
		}
		break;
	default:
		break;
	}
}

/// �����û����뷿����Ϣ
void CRobotLogicFrame::OnProcessEnterRoomMsg(int playerId)
{
	assert(m_g_myself != NULL);
	if(m_g_myself == NULL) return;

}

/// �����û��뿪������Ϣ
void CRobotLogicFrame::OnProcessLeaveRoomMsg(int playerId)
{

}

/// �����û�������Ϣ
void CRobotLogicFrame::OnProcessOfflineRoomMes(int playerId)
{

}

/// �����û�����������Ϣ
void CRobotLogicFrame::OnProcessReEnterRoomMes(int playerId)
{

}

/// �����û���ʱ����Ϣ
void CRobotLogicFrame::OnProcessTimerMsg(int timerId,int curTimer)
{
	assert(m_g_myself != NULL);
	if(m_g_myself == NULL) return;

	if(timerId == IDD_TIMER_GAME_XIAZHU1 && curTimer <= 0)
	{
		m_g_myself->StopTimer(IDD_TIMER_GAME_XIAZHU1);

		int pTepJettonMoney[] = {100,500,1000,5000,10000,50000,100000,500000,1000000};

		int64 pTempMoney = pTepJettonMoney[rand()%6];

		//if(m_g_myselfRoom->GetLastMoney() > 50000)
		//	pTempMoney = pTepJettonMoney[rand()%4+4];

		if(pTempMoney <= m_g_myself->GetMoney())
		{
            Json::Value out;
            out["MsgId"] = IDD_MESSAGE_ROOM;
            out["MsgSubId"] = IDD_MESSAGE_ROOM_XIAZHU;
            out["JettonArea"] = rand()%3;
            out["JettonMoney"] = (int)pTempMoney;
            m_g_myself->SendGameMsg(out);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
