#include "CPlayer.h"

#include "../GameFrameManager.h"
#include "PlayerManager.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////

CPlayer::CPlayer(PlayerType type)
	: m_Id(-1),m_ConnectId(0),m_roomId(-1),m_PlayerState(PLAYERSTATE_NORAML),
	m_ChairIndex(-1),m_level(0),m_Money(0),m_experience(0),
	m_totalbureau(0),m_successbureau(0),m_failbureau(0),m_successrate(0),m_runawayrate(0),
	m_PlayerType(type),m_isLookOn(false),m_BankMoney(0),m_RunawayBureau(0),
	m_Revenue(0),m_RobotLogicFrame(NULL),m_TotalResult(0),sex(0),m_RealyTime(0),
	gtype(0),ipaddress(0),m_PlayerDeviceType(PLAYERDEVICETYPE_NULL),m_CurGameType(0),m_CurServerPort(0),m_CurTableIndex(-1),m_CurChairIndex(-1),
	m_CurGamingState(false),m_roomentermoneyfirst(0),m_roomentermoneysecond(0),m_curGamingResult(0)
{
	SetType(type);

	if(m_PlayerType == PLAYERTYPE_ROBOT)
	{

	}
}

CPlayer::CPlayer(int id,uint32 conid)
	: m_Id(id),m_ConnectId(conid),m_roomId(-1),m_PlayerState(PLAYERSTATE_NORAML),
	m_ChairIndex(-1),m_level(0),m_Money(0),m_experience(0),
	m_totalbureau(0),m_successbureau(0),m_failbureau(0),m_successrate(0),m_runawayrate(0),
	m_PlayerType(PLAYERTYPE_NORMAL),m_isLookOn(false),m_BankMoney(0),m_RunawayBureau(0),
	m_Revenue(0),m_RobotLogicFrame(NULL),m_TotalResult(0),sex(0),m_RealyTime(0),
	gtype(0),ipaddress(0),m_PlayerDeviceType(PLAYERDEVICETYPE_NULL),
	m_offlineConnectId(0),m_CurGameType(0),m_CurServerPort(0),m_CurTableIndex(-1),m_CurChairIndex(-1),
	m_CurGamingState(false),m_roomentermoneyfirst(0),m_roomentermoneysecond(0),m_curGamingResult(0)
{
	SetType(PLAYERTYPE_NORMAL);
}

CPlayer::~CPlayer()
{
	DeleteAllTimer();

	if(m_PlayerType == PLAYERTYPE_ROBOT)
	{
		if(m_RobotLogicFrame)
			SafeDelete(m_RobotLogicFrame);
	}
}

/// ���û�������Ϸ�߼�����ӿ�
void CPlayer::SetRobotLogicFrame(RobotLogicFrame *frame)
{
	assert(frame != NULL);
	if(frame == NULL) return;

	m_RobotLogicFrame = frame;
	m_RobotLogicFrame->SetGamePlayer(this,NULL);
}

/// ���õ�ǰ��Ϸ����
void CPlayer::SetCurGameRoom(Room* pRoom)
{
	assert(m_RobotLogicFrame != NULL);
	if(m_RobotLogicFrame == NULL) return;

	m_RobotLogicFrame->SetGamePlayer(this,pRoom);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// ɾ�����еĶ�ʱ��
void CPlayer::DeleteAllTimer(void)
{
	m_TimerItemActive.clear();
}

/**
 * ��ʼһ����ʱ��
 *
 * @param timerId Ҫ�����Ķ�ʱ��ID
 * @param space ��ʱ���
 *
 * @return ��������ɹ������棬���򷵻ؼ�
 */
bool CPlayer::StartTimer(int timerId,int space)
{
	m_TimerLock.Acquire();

	//Ѱ������
	CTimerItemArray::iterator iter = m_TimerItemActive.find(timerId);
	if(iter != m_TimerItemActive.end())
	{
		//��ȡʱ��
		tagSubTimerItem *pTimerItem=&(*iter).second;

		//�����ж�
		if (pTimerItem->nTimerID==timerId)
		{
			pTimerItem->nTimerID=timerId;
			pTimerItem->nTimeLeave=space;
			pTimerItem->nIsEnable=true;

			m_TimerLock.Release();
			return true;
		}
	}

	tagSubTimerItem pTimerItemNew;

	//���ñ���
	pTimerItemNew.nTimerID=timerId;
	pTimerItemNew.nTimeLeave=space;
	pTimerItemNew.nIsEnable=true;

	m_TimerItemActive.insert(std::pair<uint32,tagSubTimerItem>(timerId,pTimerItemNew));

	m_TimerLock.Release();

	return true;
}

/**
 * �ر�һ����ʱ��
 *
 * @param id Ҫ�رյĶ�ʱ��ID
 */
void CPlayer::StopTimer(int id)
{
	m_TimerLock.Acquire();
	//ɾ��ʱ��
	if (id!=0)
	{
		//Ѱ������
		CTimerItemArray::iterator iter = m_TimerItemActive.find(id);
		if(iter != m_TimerItemActive.end())
		{
			//��ȡʱ��
			tagSubTimerItem *pTimerItem=&(*iter).second;

			//ɾ���ж�
			if (pTimerItem->nTimerID==id)
			{
				pTimerItem->nIsEnable=false;
			}
		}
	}
	else
	{
		CTimerItemArray::iterator iter = m_TimerItemActive.begin();
		for(;iter != m_TimerItemActive.end();++iter)
		{
			(*iter).second.nIsEnable=false;
		}
	}
	m_TimerLock.Release();
}

/**
 * �ر����еĶ�ʱ��
 */
void CPlayer::StopAllTimer(void)
{
	m_TimerLock.Acquire();
	CTimerItemArray::iterator iter = m_TimerItemActive.begin();
	for(;iter != m_TimerItemActive.end();++iter)
	{
		(*iter).second.nIsEnable=false;
	}
	m_TimerLock.Release();
}

///ʱ���¼�
bool CPlayer::OnEventTimer(DWORD dwTimerID, DWORD wBindParam)
{
	m_TimerLock.Acquire();
	//Ѱ������
	CTimerItemArray::iterator iter = m_TimerItemActive.begin();
	for(;iter != m_TimerItemActive.end();++iter)
	{
		if((*iter).second.nIsEnable == false)
			continue;

		//��������
		tagSubTimerItem *pTimerItem=&(*iter).second;
		if(pTimerItem == NULL) continue;

		//ʱ�䴦��
		if (pTimerItem->nTimeLeave<1L)
		{
			pTimerItem->nIsEnable=false;
		}
		else
		{
			pTimerItem->nTimeLeave--;
		}

		if(GetRobotLogicFrame())
		{
			GetRobotLogicFrame()->OnProcessTimerMsg(pTimerItem->nTimerID,pTimerItem->nTimeLeave);
		}
	}
	m_TimerLock.Release();

	return true;
}

/// ������Ϸ����
void CPlayer::SendGameMsg(Json::Value &msg)
{
	CMolMessageIn pInData = CMolMessageIn((const char*)msg.toStyledString().c_str(),msg.toStyledString().length());
	ServerGameFrameManager.OnProcessNetMes(GetConnectID(),&pInData);
}

/// ����׼����Ϣ
void CPlayer::SendReadyMsg(void)
{

}
