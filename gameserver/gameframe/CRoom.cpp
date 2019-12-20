#include "../../include/Common/defines.h"
#include "PlayerManager.h"
#include "DBOperator.h"

#include "../GameFrameManager.h"
#include "RoomManager.h"

#include <iosfwd>
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>

std::wstring getCurrentTime(std::wstring str)
{
	time_t now;
	tm local;

	time(&now);

	local = *(localtime(&now));

	using namespace std;
	std::wostringstream os;
	/*os << "" << setw(2) << setfill('0') << local.tm_hour
		<< str << setw(2) << setfill('0') << local.tm_min
		<< str << setw(2) << setfill('0') << local.tm_sec
		<< "";*/

	return os.str();
}

unsigned int timeGetTime()
{
    unsigned int uptime = 0;
    struct timespec on;
    if(clock_gettime(CLOCK_MONOTONIC, &on) == 0)
             uptime = on.tv_sec*1000 + on.tv_nsec/1000000;
    return uptime;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

CRoom::CRoom(RoomType type)
	: m_Id(0),m_MaxCount(0),m_RoomType(type),m_RoomState(ROOMSTATE_WAITING),
	  m_masterId(-1),m_curPlayerId(-1),m_RoomRevenue(0),m_RoomLastMoney(0),
	  m_RoomGameType(enStartMode_AllReady),m_ServerLogicFrame(NULL),m_Pielement(0),
	  m_GameType(0),m_RoomMarking(0),m_roomentermoneyfirst(0),m_roomentermoneysecond(0),m_gameid(0)
{
	for(int i=0;i<ROOM_MAX_PLAYERS;i++)
	{
		m_PlayerList[i] = NULL;
	}
}

CRoom::~CRoom()
{
	// ���÷���״̬
	Clear();

	DeleteAllTimer();
	ClearAllPlayers();

	if(m_ServerLogicFrame)
		SafeDelete(m_ServerLogicFrame);
}

/**
 * ��ǰ�����Ƿ�Ϊ��
 *
 * @return �����ǰ����Ϊ�յĻ������棬���򷵻ؼ�
 */
bool CRoom::IsEmpty(void)
{
	bool isok=true;

	m_playersLock.Acquire();
	for(int i=0;i<ROOM_MAX_PLAYERS;i++)
	{
		if(m_PlayerList[i])
		{
			isok = false;
			break;
		}
	}
	m_playersLock.Release();

	return isok;
}

/**
 * ������������е����
 */
void CRoom::ClearAllPlayers(void)
{
	m_playersLock.Acquire();
	for(int i=0;i<ROOM_MAX_PLAYERS;i++)
	{
		if(m_PlayerList[i] != NULL)
		{
			m_PlayerList[i]->SetRoomId(-1);
			m_PlayerList[i]->SetChairIndex(-1);
			m_PlayerList[i]->SetState(PLAYERSTATE_NORAML);
			m_PlayerList[i]->SetLookOn(false);
		}
		m_PlayerList[i] = NULL;
	}
	std::map<int,std::vector<CPlayer*> >::iterator iter = m_LookonPlayerList.begin();
	for(;iter != m_LookonPlayerList.end();iter++)
	{
		for(int index = 0;index < (int)(*iter).second.size();index++)
		{
			if((*iter).second[index] != NULL)
			{
				(*iter).second[index]->SetRoomId(-1);
				(*iter).second[index]->SetChairIndex(-1);
				(*iter).second[index]->SetState(PLAYERSTATE_NORAML);
				(*iter).second[index]->SetLookOn(false);
			}
			(*iter).second[index] = NULL;
		}
		(*iter).second.clear();
	}
	m_lostPlayerList.clear();
	m_playersLock.Release();
}

/**
 * �õ���ǰ����ָ��״̬����Ҹ���
 *
 * @param state Ҫ�õ�����ҵ�״̬
 *
 * @return ����ָ��������״̬����Ҹ���
 */
int CRoom::GetPlayerCount(PlayerState state)
{
	int count = 0;

	m_playersLock.Acquire();
	for(int i=0;i<ROOM_MAX_PLAYERS;i++)
	{
		if(m_PlayerList[i] &&
			m_PlayerList[i]->GetState() == state)
			count+=1;
	}
	m_playersLock.Release();

	return count;
}

/**
 * ���õ�ǰ����������ҵ�״̬
 *
 * @param state Ҫ���õ���ҵ�״̬
 */
void CRoom::SetAllPlayerState(PlayerState state)
{
	m_playersLock.Acquire();
	for(int i=0;i<ROOM_MAX_PLAYERS;i++)
	{
		if(m_PlayerList[i] == NULL) continue;

		m_PlayerList[i]->SetState(state);

		if(state == PLAYERSTATE_GAMING)
		{
			// ���������Ϸ״̬
			m_PlayerList[i]->setCurGamingState(true);

			//m_PlayerList[i]->SetTotalResult(0);
			m_PlayerList[i]->setCurrentGamingResult(0);
			ServerDBOperator.SetPlayerGameState(m_PlayerList[i]);
		}
	}
	m_playersLock.Release();
}

/**
 * ����ָ����ҵ�״̬
 *
 * @param index Ҫ���õ���ҵķ���ID
 * @param state Ҫ���õ����״̬
 */
void CRoom::SetPlayerState(int index,PlayerState state)
{
	if(index < 0 || index >= ROOM_MAX_PLAYERS) return;

	m_playersLock.Acquire();
	if(m_PlayerList[index])
		m_PlayerList[index]->SetState(state);
	m_playersLock.Release();
}

/**
 * ���һ����ҵ���ǰ������
 *
 * @param pPlayer Ҫ��ӵ����
 * @param index Ҫ�������ӵ�ָ����λ���ϣ�������ó�-1,��ô��ʾ�����ӵ�������
 *
 * @return ��������ڷ����е�����
 */
int CRoom::AddPlayer(CPlayer *pPlayer,int index)
{
	int playerIndex = -1;

	if(pPlayer == NULL ||
		GetPlayerCount() > m_MaxCount)
		return playerIndex;

	// ����û��Ѿ��ڷ����У��Ͳ�������
	if(IsExist(pPlayer)) return playerIndex;

	// �����ҵĽ�������Ƿ����㵱ǰ����Ҫ��
	//if(pPlayer->GetMoney() < m_RoomLastMoney) return playerIndex;

	m_playersLock.Acquire();
	if(index == playerIndex)
	{
		for(int i=0;i<ROOM_MAX_PLAYERS;i++)
		{
			if(m_PlayerList[i] == NULL)
			{
				m_PlayerList[i] = pPlayer;
				playerIndex = i;
				break;
			}
		}
	}
	else
	{
		if(index >= 0 && index < ROOM_MAX_PLAYERS)
		{
			if(m_PlayerList[index] == NULL)
			{
				m_PlayerList[index] = pPlayer;
				playerIndex = index;
			}
		}
	}
	m_playersLock.Release();

	// ���������Ϸ״̬
	pPlayer->setCurTableIndex(GetID());
	pPlayer->setCurChairIndex(playerIndex);
	ServerDBOperator.SetPlayerGameState(pPlayer);

	return playerIndex;
}

/// ������Ϸ�߼�����ӿ�
void CRoom::SetServerLogicFrame(ServerLogicFrame *frame)
{
	assert(frame != NULL);
	if(frame == NULL) return;

	m_ServerLogicFrame = frame;
	m_ServerLogicFrame->SetGameRoom(this);
}

/**
 * ���һ���Թ���ҵ���ǰ������
 *
 * @param pPlayer Ҫ��ӵ����
 * @param index Ҫ�������ӵ�ָ����λ����
 *
 * @return ��������ڷ����е�����
 */
int CRoom::AddLookOnPlayer(CPlayer *pPlayer,int index)
{
	int playerIndex = -1;

	if(pPlayer == NULL ||
		(index < 0 || index > m_MaxCount))
		return playerIndex;

	// ����û��Ѿ��ڷ����У��Ͳ�������
	if(IsExistLookOn(pPlayer)) return playerIndex;

	m_playersLock.Acquire();
	if(index >= 0 && index < ROOM_MAX_PLAYERS)
	{
		std::map<int,std::vector<CPlayer*> >::iterator iter = m_LookonPlayerList.find(index);
		if(iter != m_LookonPlayerList.end())
		{
			// ����Ƿ�������û���û�в���ӽ�ȥ
			bool isExistPlayer = true;
			for(int indexP = 0;indexP < (int)(*iter).second.size();indexP++)
			{
				if((*iter).second[indexP] == pPlayer)
				{
					isExistPlayer = false;
					break;
				}
			}

			if(isExistPlayer)
			{
				(*iter).second.push_back(pPlayer);
				playerIndex = index;
			}
		}
		else
		{
			std::vector<CPlayer*> tempPlayers;
			tempPlayers.push_back(pPlayer);

			m_LookonPlayerList.insert(std::pair<int,std::vector<CPlayer*> >(index,tempPlayers));
			playerIndex = index;
		}
	}
	m_playersLock.Release();

	// ���������Ϸ״̬
	pPlayer->setCurTableIndex(GetID());
	pPlayer->setCurChairIndex(playerIndex);
	ServerDBOperator.SetPlayerGameState(pPlayer);

	return playerIndex;
}

/**
 * ���õ�ǰ����ķ���
 *
 * @param playerId Ҫ���óɷ��������ID
 */
void CRoom::SetMaster(int playerId)
{
	m_masterId = playerId;
	m_curPlayerId = m_masterId;
}

/**
 * �õ���ǰ��������һ�����
 */
int CRoom::GetCurNextPlayer(void)
{
	m_curPlayerId += 1;

	if(m_curPlayerId >= GetPlayerCount())
		m_curPlayerId = 0;

	return m_curPlayerId;
}

/**
 * �õ���ǰ��������һ���������
 */
int CRoom::GetCurNextWorkPlayer(void)
{
	int count = 0;

	while(true)
	{
		if(count >= GetPlayerCount()) break;

		int index = GetCurNextPlayer();
		Player *pPlayer = GetPlayer(index);
		if(pPlayer && pPlayer->GetState() == PLAYERSTATE_GAMING)
			return index;

		count+=1;
	}

	return -1;
}

/**
 * ��ʼһ����ʱ��
 *
 * @param timerId Ҫ�����Ķ�ʱ��ID
 * @param space ��ʱ���
 *
 * @return ��������ɹ������棬���򷵻ؼ�
 */
bool CRoom::StartTimer(int timerId,int space)
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
void CRoom::StopTimer(int id)
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
void CRoom::StopAllTimer(void)
{
	m_TimerLock.Acquire();
	CTimerItemArray::iterator iter = m_TimerItemActive.begin();
	for(;iter != m_TimerItemActive.end();++iter)
	{
		(*iter).second.nIsEnable=false;
	}
	m_TimerLock.Release();
}

/// ɾ�����еĶ�ʱ��
void CRoom::DeleteAllTimer(void)
{
	m_TimerItemActive.clear();
}

///ʱ���¼�
bool CRoom::OnEventTimer(DWORD dwTimerID, DWORD wBindParam)
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

		if(GetServerLogicFrame())
		{
			GetServerLogicFrame()->OnProcessTimerMsg(pTimerItem->nTimerID,pTimerItem->nTimeLeave);
		}
	}
	m_TimerLock.Release();

	return true;
}

/**
 * �ӵ�ǰ���������ָ�������
 *
 * @param pPlayer Ҫ��������
 */
void CRoom::ClearPlayer(CPlayer *pPlayer)
{
	if(pPlayer == NULL)
		return;

	// ���������Ϸ״̬
	pPlayer->setCurChairIndex(-1);
	pPlayer->setCurTableIndex(-1);
	pPlayer->SetEnterMoneyRect(0,0);
	pPlayer->SetEnterPassword("");
	ServerDBOperator.SetPlayerGameState(pPlayer);

	m_playersLock.Acquire();
	for(int i=0;i<ROOM_MAX_PLAYERS;i++)
	{
		if(m_PlayerList[i] &&
			m_PlayerList[i] == pPlayer)
		{
			//pPlayer->SetState(PLAYERSTATE_NORAML);
			//pPlayer->SetRoomId(-1);
			//pPlayer->SetChairIndex(-1);
			//pPlayer->SetLookOn(false);

			ClearLostPlayerById(i);

			m_PlayerList[i] = NULL;

			break;
		}
	}
	std::map<int,std::vector<CPlayer*> >::iterator iter = m_LookonPlayerList.find(pPlayer->GetChairIndex());
	if(iter != m_LookonPlayerList.end())
	{
		std::vector<CPlayer*>::iterator iter2 = (*iter).second.begin();
		for(;iter2 != (*iter).second.end();iter2++)
		{
			if((*iter2) == pPlayer)
			{
				//pPlayer->SetState(PLAYERSTATE_NORAML);
				//pPlayer->SetRoomId(-1);
				//pPlayer->SetChairIndex(-1);
				//pPlayer->SetLookOn(false);

				(*iter2) = NULL;
				(*iter).second.erase(iter2);
				break;
			}
		}
	}
	m_playersLock.Release();

	// ���������û�˵Ļ��������跿��
	if(IsEmpty())
	{
		bool isChangeRoomState = false;

		if(m_roomenterpwd != "" ||
			m_roomentermoneyfirst != 0 ||
			m_roomentermoneysecond != 0)
			isChangeRoomState = true;

		m_roomenterpwd="";
		m_roomentermoneyfirst=m_roomentermoneysecond=0;

		if(isChangeRoomState)
		{
			// ��������������е��������ͨ����ҽ�����Ϸ�������Ϣ
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_FRAME;
            root["MsgSubId"] = IDD_MESSAGE_FRAME_ROOM_JOINSET;
            root["UserID"] = GetID();
            root["roomenterpwd"] = m_roomenterpwd;
            root["roomentermoneyfirst"] = (uint32)m_roomentermoneyfirst;
            root["roomentermoneysecond"] = (uint32)m_roomentermoneysecond;

			ServerPlayerManager.SendMsgToEveryone(root);
		}
	}
}

/**
 * �ӵ�ǰ������ɾ��ָ�������
 *
 * @param pPlayer Ҫɾ�������
 */
void CRoom::DeletePlayer(CPlayer *pPlayer)
{
	if(pPlayer == NULL)
		return;

	m_playersLock.Acquire();
	for(int i=0;i<ROOM_MAX_PLAYERS;i++)
	{
		if(m_PlayerList[i] == pPlayer)
		{
			if(!IsExistLostPlayer(i))
				m_lostPlayerList.push_back(i);

			delete m_PlayerList[i];
			m_PlayerList[i] = NULL;
			break;
		}
	}
	m_playersLock.Release();
}

/**
 * �����û�����ID�õ��û��ڷ����е�ID
 *
 * @param connId Ҫ�õ����û�������ID
 *
 * @return �������û�������������䣬��������û�����������е�ID�����򷵻�-1
 */
int CRoom::GetPlayerRoomId(uint32 connId)
{
	int state = -1;

	m_playersLock.Acquire();
	for(int i=0;i<ROOM_MAX_PLAYERS;i++)
	{
		if(m_PlayerList[i] == NULL) continue;

		if(m_PlayerList[i]->GetConnectID() == connId)
		{
			state = i;
			break;
		}
	}
	m_playersLock.Release();

	return state;
}

/**
 * ���ָ���������Ƿ��ڵ�������б���
 *
 * @param index Ҫ��������ڷ����е�����
 *
 * @return �������ڵ�������б��з����棬���򷵻ؼ�
 */
bool CRoom::IsExistLostPlayer(int index)
{
	if(m_lostPlayerList.empty()) return false;

	bool isok = false;
	m_playersLock.Acquire();
	for(int i=0;i<(int)m_lostPlayerList.size();i++)
	{
		if(m_lostPlayerList[i] == index)
		{
			isok = true;
			break;
		}
	}
	m_playersLock.Release();

	return isok;
}

/**
 * �����������б������
 *
 * @param id Ҫ�������ҵ�ID
 */
void CRoom::ClearLostPlayerById(int id)
{
	if(m_lostPlayerList.empty()) return;

	m_playersLock.Acquire();
	std::vector<int>::iterator iter = m_lostPlayerList.begin();
	for(int i=0;iter != m_lostPlayerList.end();i++)
	{
		if(i == id)
		{
			iter = m_lostPlayerList.erase(iter);
			break;
		}
		else
		{
			++iter;
		}
	}
	m_playersLock.Release();
}

/**
 * �ӵ�ǰ���������ָ������ID�����
 *
 * @param Id Ҫ���������ڷ����е�ID
 */
void CRoom::ClearPlayerById(int Id)
{
	m_playersLock.Acquire();
	for(int i=0;i<ROOM_MAX_PLAYERS;i++)
	{
		if(m_PlayerList[i] && m_PlayerList[i]->GetChairIndex() == Id)
		{
			//m_PlayerList[i]->SetRoomId(-1);
			//m_PlayerList[i]->SetChairIndex(-1);
			//m_PlayerList[i]->SetLookOn(false);

			ClearLostPlayerById(Id);

			m_PlayerList[i] = NULL;

			break;
		}
	}
	std::map<int,std::vector<CPlayer*> >::iterator iter = m_LookonPlayerList.find(Id);
	if(iter != m_LookonPlayerList.end())
	{
		for(int index = 0;index < (int)(*iter).second.size();index++)
		{
			if((*iter).second[index] != NULL)
			{
				//(*iter).second[index]->SetRoomId(-1);
				//(*iter).second[index]->SetChairIndex(-1);
				//(*iter).second[index]->SetState(PLAYERSTATE_NORAML);
				//(*iter).second[index]->SetLookOn(false);
			}
			(*iter).second[index] = NULL;
		}
		(*iter).second.clear();
	}
	m_playersLock.Release();
}

/**
 * �õ���ǰ��������
 */
int CRoom::GetPlayerCount(void)
{
	int count = 0;

	m_playersLock.Acquire();
	for(int i=0;i<ROOM_MAX_PLAYERS;i++)
	{
		if(m_PlayerList[i] == NULL) continue;

		if(m_PlayerList[i]->GetRoomId() != -1 &&
			m_PlayerList[i]->GetChairIndex() != -1)
			count+=1;
	}
	m_playersLock.Release();

	return count;
}

/**
 * �õ�׼����״̬����ҵ�����
 *
 * @return ���ص�ǰ������׼���õ���ҵĸ���
 */
int CRoom::GetReadyPlayerCount(void)
{
	int count = 0;

	m_playersLock.Acquire();
	for(int i=0;i<ROOM_MAX_PLAYERS;i++)
	{
		if(m_PlayerList[i] == NULL) continue;

		if(m_PlayerList[i]->GetState() == PLAYERSTATE_READY)
		{
			count += 1;
		}
	}
	m_playersLock.Release();

	return count;
}

/**
 * ��յ�ǰ��������
 */
void CRoom::Clear(void)
{
	// �������״̬
	//SetAllPlayerState(PLAYERSTATE_NORAML);

	m_lostPlayerList.clear();
	m_RoomState = ROOMSTATE_WAITING;
	m_masterId = -1;
	m_curPlayerId = -1;

	StopAllTimer();
}

/**
 * ���ָ������Ƿ���ڵ�ǰ����
 *
 * @param pPlayer Ҫ�������
 *
 * @return ���Ҫ������Ҵ��ڷ����棬���򷵻ؼ�
 */
bool CRoom::IsExist(CPlayer *pPlayer)
{
	if(pPlayer == NULL) return false;

	bool state = false;

	if(pPlayer == NULL)
		return state;

	m_playersLock.Acquire();
	for(int i=0;i<ROOM_MAX_PLAYERS;i++)
	{
		if(m_PlayerList[i] &&
			m_PlayerList[i]->GetID() == pPlayer->GetID())
		{
			state = true;
			break;
		}
	}
	std::map<int,std::vector<CPlayer*> >::iterator iter = m_LookonPlayerList.find(pPlayer->GetChairIndex());
	if(iter != m_LookonPlayerList.end())
	{
		for(int index = 0;index < (int)(*iter).second.size();index++)
		{
			if((*iter).second[index] == pPlayer)
			{
				state = true;
				break;
			}
		}
	}
	m_playersLock.Release();

	return state;
}

/**
 * ���ָ�����Թ�����Ƿ���ڵ�ǰ������
 *
 * @param pPlayer Ҫ�������
 *
 * @return ���Ҫ������Ҵ��ڷ����棬���򷵻ؼ�
 */
bool CRoom::IsExistLookOn(CPlayer *pPlayer)
{
	if(pPlayer == NULL) return false;

	bool state = false;

	if(pPlayer == NULL)
		return state;

	m_playersLock.Acquire();
	std::map<int,std::vector<CPlayer*> >::iterator iter = m_LookonPlayerList.find(pPlayer->GetChairIndex());
	if(iter != m_LookonPlayerList.end())
	{
		for(int index = 0;index < (int)(*iter).second.size();index++)
		{
			if((*iter).second[index] == pPlayer)
			{
				state = true;
				break;
			}
		}
	}
	m_playersLock.Release();

	return state;
}

/**
 * ��ⷿ���Ƿ��Ѿ���Ա
 *
 * @return ��������Ѿ���Ա�����棬���򷵻ؼ�
 */
bool CRoom::IsFull(void)
{
	return GetPlayerCount() == m_MaxCount ? true : false;
}

/**
 * ��ⷿ���Ƿ�ȫ��׼�����
 *
 * @param �����ǰ������Ա����ȫ������׼��״̬�·����棬���򷵻ؼ�
 */
bool CRoom::IsReadyed(void)
{
	if(/*m_RoomState == ROOMSTATE_WAITING &&*/
		IsFull())
	{
		bool isReady = true;

		m_playersLock.Acquire();
		for(int i=0;i<ROOM_MAX_PLAYERS;i++)
		{
			if(m_PlayerList[i] == NULL) continue;

			if(m_PlayerList[i]->GetState() != PLAYERSTATE_READY)
			{
				isReady = false;
				break;
			}
		}
		m_playersLock.Release();

		return isReady;
	}

	return false;
}

/**
 * ��ָ������ҷ�������
 *
 * @param index Ҫ���͵���ҵ�����,���Ϊ-1�Ļ�����ʾ���͸������ˣ������͸�ָ������
 * @param msg Ҫ���͵�����
 */
void CRoom::SendTableMsg(int index,Json::Value &msg)
{
	if(index != INVALID_CHAIR)
	{
		m_playersLock.Acquire();
		Player *pPlayer = dynamic_cast<Player*>(m_PlayerList[index]);
		if(pPlayer != NULL)
		{
			// �������ǻ����˵Ļ����ʹ��������
			if(pPlayer->GetType() == PLAYERTYPE_ROBOT)
			{
				if(m_PlayerList[index]->GetRobotLogicFrame() != NULL)
                {
                   // CMolMessageIn pInData = CMolMessageIn(msg.getData(),msg.getLength());
					m_PlayerList[index]->GetRobotLogicFrame()->OnProcessPlayerRoomMes(msg);
                }
			}
			else
			{
				if(pPlayer->GetConnectID() == NULL ||
					IsConnected(pPlayer->GetConnectID()) == false)
				{
					m_playersLock.Release();
					return;
				}

				Sendhtml5(pPlayer->GetConnectID(),(const char*)msg.toStyledString().c_str(),msg.toStyledString().length());
			}
		}
		m_playersLock.Release();
	}
	else
	{
		m_playersLock.Acquire();
		// ����ǰ�����еĻ�����
		for(int i=0;i<GetMaxPlayer();i++)
		{
			if(m_PlayerList[i] == NULL) continue;
			if(m_PlayerList[i]->GetType() != PLAYERTYPE_ROBOT) continue;
			if(m_PlayerList[i]->GetRobotLogicFrame() == NULL) continue;

			//CMolMessageIn pInData = CMolMessageIn(msg.getData(),msg.getLength());
			m_PlayerList[i]->GetRobotLogicFrame()->OnProcessPlayerRoomMes(msg);
		}
		m_playersLock.Release();

		SendAllPlayerMsg(msg);
	}
}

/**
 * ��ָ������ҷ����Թ�����
 *
 * @param index Ҫ���͵���ҵ�����,���Ϊ-1�Ļ�����ʾ���͸������ˣ������͸�ָ������
 * @param msg Ҫ���͵�����
 */
void CRoom::SendLookOnMes(int index,Json::Value &msg)
{
	if(index >= 0 && index < ROOM_MAX_PLAYERS)
	{
		m_playersLock.Acquire();
		std::map<int,std::vector<CPlayer*> >::iterator iter = m_LookonPlayerList.find(index);
		if(iter != m_LookonPlayerList.end())
		{
			for(int index = 0;index < (int)(*iter).second.size();index++)
			{
				if((*iter).second[index] == NULL) continue;

				if((*iter).second[index]->GetConnectID() == NULL ||
					IsConnected((*iter).second[index]->GetConnectID()) == false)
					continue;

				Sendhtml5((*iter).second[index]->GetConnectID(),(const char*)msg.toStyledString().c_str(),msg.toStyledString().length());
			}
		}
		m_playersLock.Release();
	}
	else if(index == -1)                    // ���͸�������
	{
		SendAllLookOnPlayerMsg(msg);
	}
}

/**
 * ��������ҷ�������
 *
 * @param msg Ҫ���͵�����
 */
void CRoom::SendAllPlayerMsg(Json::Value &msg)
{
	m_playersLock.Acquire();
	for(int i=0;i<GetMaxPlayer();i++)
	{
		if(m_PlayerList[i] == NULL) continue;

		if(m_PlayerList[i]->GetConnectID() == NULL ||
			IsConnected(m_PlayerList[i]->GetConnectID()) == false)
			continue;

		Sendhtml5(m_PlayerList[i]->GetConnectID(),(const char*)msg.toStyledString().c_str(),msg.toStyledString().length());
	}
	m_playersLock.Release();
}

/**
 * �������Թ���ҷ�������
 *
 * @param msg Ҫ���͵�����
 */
void CRoom::SendAllLookOnPlayerMsg(Json::Value &msg)
{
	m_playersLock.Acquire();
	std::map<int,std::vector<CPlayer*> >::iterator iter = m_LookonPlayerList.begin();
	for(;iter != m_LookonPlayerList.end();iter++)
	{
		for(int index = 0;index < (int)(*iter).second.size();index++)
		{
			if((*iter).second[index] == NULL) continue;

			if((*iter).second[index]->GetConnectID() == NULL ||
				IsConnected((*iter).second[index]->GetConnectID()) == false)
				continue;

			Sendhtml5((*iter).second[index]->GetConnectID(),(const char*)msg.toStyledString().c_str(),msg.toStyledString().length());
		}
	}
	m_playersLock.Release();
}

/**
 * �õ�ָ�����������
 *
 * @param index Ҫ�õ�����ҵ�����
 *
 * @return ������ָ����������Ҵ��ڷ��������ң����򷵻�NULL
 */
Player* CRoom::GetPlayer(int index)
{
	Player *pPlayer = NULL;

	if(index < 0 || index >= ROOM_MAX_PLAYERS)
		return pPlayer;

	m_playersLock.Acquire();
	pPlayer = dynamic_cast<Player*>(m_PlayerList[index]);
	m_playersLock.Release();

	return pPlayer;
}

/**
 * �����������id�õ�����ڵ�ǰ�����е�����
 *
 * @param connId Ҫ�õ��Ŀͻ�������ID
 *
 * @return ������������ڷ����е�����
 */
int CRoom::GetPlayerIndex(uint32 connId)
{
	int state = -1;

	m_playersLock.Acquire();
	for(int i=0;i<ROOM_MAX_PLAYERS;i++)
	{
		if(m_PlayerList[i] == NULL) continue;

		if(m_PlayerList[i]->GetConnectID() == connId)
		{
			state = m_PlayerList[i]->GetChairIndex();
			break;
		}
	}
	m_playersLock.Release();

	return state;
}

/**
 * ���ڴ����û�׼�������Ϣ
 *
 * @param playerId Ҫ����Ŀͻ��˵ķ���ID
 */
void CRoom::OnProcessPlayerReadyMes(int playerId)
{
	m_playersLock.Acquire();
	// ���ݷ�������ȷ���Ƿ���Կ�ʼ��Ϸ
	if(GetRoomGameType() == enStartMode_FullReady)
	{
		// ���׼��״̬����Ҵﵽ2���Ϳ��Կ�ʼ��Ϸ��
		int pReadyCount = GetReadyPlayerCount();

		if(pReadyCount >= 2 && pReadyCount == GetPlayerCount())
			GameStart();

		m_playersLock.Release();
		return;
	}
	else if(GetRoomGameType() == enStartMode_AllReady)
	{
		if(IsReadyed())
		{
			GameStart();
		}

		m_playersLock.Release();
		return;
	}

	// ����ǰ�����еĻ�����
	//for(int i=0;i<GetMaxPlayer();i++)
	//{
		//if(m_PlayerList[playerId] == NULL) continue;
		//if(m_PlayerList[playerId]->GetType() != PLAYERTYPE_ROBOT) continue;
		//if(m_PlayerList[playerId]->GetRobotLogicFrame() == NULL) continue;

		if(m_PlayerList[playerId] &&
			m_PlayerList[playerId]->GetType() == PLAYERTYPE_ROBOT &&
			m_PlayerList[playerId]->GetRobotLogicFrame() != NULL)
			m_PlayerList[playerId]->GetRobotLogicFrame()->OnProcessPlayerReadyMes(playerId);
	//}
	m_playersLock.Release();
}

/**
 * ���ڴ����û�������Ϸ��������Ϣ
 *
 * @param playerId Ҫ����Ŀͻ��˵ķ���ID
 * @param mes ���յĿͻ��˷��͵���Ϣ
 */
void CRoom::OnProcessPlayerRoomMes(int playerId,Json::Value &mes)
{
	assert(m_ServerLogicFrame!=NULL);
	if(m_ServerLogicFrame == NULL) return;

	m_ServerLogicFrame->OnProcessPlayerRoomMes(playerId,mes);
}

/// �����û����뷿����Ϣ
void CRoom::OnProcessEnterRoomMsg(int playerId)
{
	assert(m_ServerLogicFrame!=NULL);
	if(m_ServerLogicFrame == NULL) return;

	m_ServerLogicFrame->OnProcessEnterRoomMsg(playerId);

	m_playersLock.Acquire();

	// ����ǰ�����еĻ�����
	//for(int i=0;i<GetMaxPlayer();i++)
	//{
	//	if(m_PlayerList[i] == NULL) continue;
	//	if(m_PlayerList[i]->GetType() != PLAYERTYPE_ROBOT) continue;
	//	if(m_PlayerList[i]->GetRobotLogicFrame() == NULL) continue;

		if(m_PlayerList[playerId] &&
			m_PlayerList[playerId]->GetType() == PLAYERTYPE_ROBOT &&
			m_PlayerList[playerId]->GetRobotLogicFrame() != NULL)
		{
			m_PlayerList[playerId]->SetCurGameRoom(this);
			m_PlayerList[playerId]->GetRobotLogicFrame()->OnProcessEnterRoomMsg(playerId);
		}
	//}
	m_playersLock.Release();
}

/// �����û��뿪������Ϣ
void CRoom::OnProcessLeaveRoomMsg(int playerId)
{
	assert(m_ServerLogicFrame!=NULL);
	if(m_ServerLogicFrame == NULL || playerId < 0 || playerId >= GetMaxPlayer()) return;

	m_ServerLogicFrame->OnProcessLeaveRoomMsg(playerId);

	m_playersLock.Acquire();

	// ����ǰ�����еĻ�����
	//for(int i=0;i<GetMaxPlayer();i++)
	//{
	//	if(m_PlayerList[i] == NULL) continue;
	//	if(m_PlayerList[i]->GetType() != PLAYERTYPE_ROBOT) continue;
	//	if(m_PlayerList[i]->GetRobotLogicFrame() == NULL) continue;

		if(m_PlayerList[playerId] &&
			m_PlayerList[playerId]->GetType() == PLAYERTYPE_ROBOT &&
			m_PlayerList[playerId]->GetRobotLogicFrame() != NULL)
		{
			m_PlayerList[playerId]->GetRobotLogicFrame()->OnProcessLeaveRoomMsg(playerId);
		}
	//}

	if(m_PlayerList[playerId])
		m_PlayerList[playerId]->SetState(PLAYERSTATE_NORAML);
	m_playersLock.Release();
}

/// �����û�������Ϣ
void CRoom::OnProcessOfflineRoomMes(int playerId)
{
	assert(m_ServerLogicFrame!=NULL);
	if(m_ServerLogicFrame == NULL) return;

	m_ServerLogicFrame->OnProcessOfflineRoomMes(playerId);

	m_playersLock.Acquire();

	// ����ǰ�����еĻ�����
	//for(int i=0;i<GetMaxPlayer();i++)
	//{
	//	if(m_PlayerList[i] == NULL) continue;
	//	if(m_PlayerList[i]->GetType() != PLAYERTYPE_ROBOT) continue;
	//	if(m_PlayerList[i]->GetRobotLogicFrame() == NULL) continue;

		if(m_PlayerList[playerId] &&
			m_PlayerList[playerId]->GetType() == PLAYERTYPE_ROBOT &&
			m_PlayerList[playerId]->GetRobotLogicFrame() != NULL)
		{
			m_PlayerList[playerId]->GetRobotLogicFrame()->OnProcessOfflineRoomMes(playerId);
		}
	//}
	m_playersLock.Release();
}

/// �����û�����������Ϣ
void CRoom::OnProcessReEnterRoomMes(int playerId)
{
	assert(m_ServerLogicFrame!=NULL);
	if(m_ServerLogicFrame == NULL) return;

	m_ServerLogicFrame->OnProcessReEnterRoomMes(playerId);

	m_playersLock.Acquire();

	// ����ǰ�����еĻ�����
	//for(int i=0;i<GetMaxPlayer();i++)
	//{
	//	if(m_PlayerList[i] == NULL) continue;
	//	if(m_PlayerList[i]->GetType() != PLAYERTYPE_ROBOT) continue;
	//	if(m_PlayerList[i]->GetRobotLogicFrame() == NULL) continue;

		if(m_PlayerList[playerId] &&
			m_PlayerList[playerId]->GetType() == PLAYERTYPE_ROBOT &&
			m_PlayerList[playerId]->GetRobotLogicFrame() != NULL)
		{
			m_PlayerList[playerId]->GetRobotLogicFrame()->OnProcessReEnterRoomMes(playerId);
		}
	//}
	m_playersLock.Release();
}

/**
 * ���ڴ�����յ���������Ϣ
 *
 * @param connId Ҫ����Ŀͻ���
 * @param mes Ҫ����Ŀͻ��˷��͵�������Ϣ
 */
void CRoom::OnProcessNetMes(uint32 connId,Json::Value &mes)
{
	int playerId = GetPlayerIndex(connId);
	if(playerId == -1) return;

	m_playersLock.Acquire();
	OnProcessPlayerRoomMes(playerId,mes);
	m_playersLock.Release();
}

/**
 * ����ָ����ҵ�����
 *
 * @param ChairID Ҫ���µ���ҵ�ID
 */
void CRoom::UpdateUserScore(Player *pPlayer)
{
	if(pPlayer == NULL) return;

	ServerGameFrameManager.UpdatePlayerInfo(pPlayer);
}

/// д���û�����
bool CRoom::WriteUserScore(int wChairID, int64 lScore, int64 lRevenue, enScoreKind ScoreKind,int64 pAgentmoney,bool isCumulativeResult,int64 pcurJetton,const char* pgametip)
{
	assert(m_ServerLogicFrame!=NULL);
	if(m_ServerLogicFrame == NULL) return false;

	//if(lScore == 0) return false;

	CPlayer *pPlayer = static_cast<CPlayer*>(GetPlayer(wChairID));
	if(pPlayer == NULL) return false;

	bool isok=true;

	int64 pTempMoney = (lScore + lRevenue) + pAgentmoney;

	m_playersLock.Acquire();
	pPlayer->SetMoney(pPlayer->GetMoney() + lScore);
	pPlayer->SetTotalBureau(pPlayer->GetTotalBureau()+1);
	pPlayer->SetExperience(pPlayer->GetExperience()+1);
	pPlayer->SetTotalResult(pPlayer->GetTotalResult() + pTempMoney);
	if(isCumulativeResult) pPlayer->setCurrentGamingResult(pTempMoney);

	switch(ScoreKind)
	{
	case enScoreKind_Win:
		{
			pPlayer->SetSuccessBureau(pPlayer->GetSuccessBureau()+1);
		}
		break;
	case enScoreKind_Lost:
		{
			pPlayer->SetFailBureau(pPlayer->GetFailBureau()+1);
		}
		break;
	case enScoreKind_Flee:
		{
			pPlayer->SetRunawayBureau(pPlayer->GetRunawayBureau()+1);
		}
		break;
	default:
		break;
	}

	// ����ȼ�
	uint32 plevel = uint32((pPlayer->GetLevel() + 1) * 10 + pPlayer->GetLevel() * 10);
	if(pPlayer->GetExperience() >= plevel)
		pPlayer->SetLevel(pPlayer->GetLevel()+1);

	// ����ʤ��
	pPlayer->SetSuccessRate((float)pPlayer->GetSuccessBureau() / (float)pPlayer->GetTotalBureau());

	// ����������
	pPlayer->SetRunawayrate((float)pPlayer->GetRunawayBureau() / (float)pPlayer->GetTotalBureau());

	// ����˰��
	if(lRevenue > 0)
	{
		pPlayer->SetRevenue(pPlayer->GetRevenue()+lRevenue);
	}

	// д��ҵ���Ϸ��¼
	ServerDBOperator.InsertPlayerGameRecord(pPlayer->GetID(),
											lScore,
											lRevenue,
											GetGameId(),
											GetRoomMarking(),
											m_ServerSet.GameName,
											pPlayer->GetRoomId(),
											pPlayer->GetChairIndex(),
											pPlayer->GetMoney(),
											pgametip,
											pAgentmoney,
											pcurJetton);

	isok = ServerDBOperator.UpdateUserData(pPlayer);

	if(isCumulativeResult) {
        if(pPlayer->GetType() == PLAYERTYPE_NORMAL)
            ServerDBOperator.UpdateGamingUserTotalResult(0,pPlayer->getCurrentGamingResult());
        else if(pPlayer->GetType() == PLAYERTYPE_ROBOT)
            ServerDBOperator.UpdateGamingUserTotalResult(pPlayer->getCurrentGamingResult(),0);
	}

	m_playersLock.Release();

	return isok;
}

/**
 * ��ǰ����������ҷ����û��뿪��������Ϣ
 */
void CRoom::AllPlayerLeveaServer(void)
{
	m_playersLock.Acquire();
	for(int i=0;i<ROOM_MAX_PLAYERS;i++)
	{
		CPlayer *pPlayer = m_PlayerList[i];
		if(pPlayer == NULL) continue;

		// �򷿼�������ҹ㲥����뿪��������Ϣ
        Json::Value root;
        root["MsgId"] = IDD_MESSAGE_FRAME;
        root["MsgSubId"] = IDD_MESSAGE_LEAVE_SERVER;
        root["UserID"] = m_PlayerList[i]->GetID();
		ServerPlayerManager.SendMsgToEveryone(root);

		m_PlayerList[i]->SetRoomId(-1);
		m_PlayerList[i]->SetChairIndex(-1);
		m_PlayerList[i]->SetState(PLAYERSTATE_NORAML);

		ClearPlayer(m_PlayerList[i]);

		uint32 pConnID = pPlayer->GetConnectID();
		ServerPlayerManager.ClearPlayer(pPlayer);
		Disconnect(pConnID);
	}
	m_playersLock.Release();
}

/// �Ƿ������Ϸ
void CRoom::OnProcessContinueGaming(void)
{
	// �����ϷģʽΪ���˿�ʼ����ô����Ϸ���䲻������Ϸ״̬������£�
	// ������������ϵ���׼��������£���ʼ��Ϸ
	m_playersLock.Acquire();
	if(/*m_ServerSet.GameType != ROOMTYPE_BISAI && */
		GetRoomGameType() == enStartMode_FullReady &&
		GetRoomState() != ROOMSTATE_GAMING)
	{
		// ���׼��״̬����Ҵﵽ2���Ϳ��Կ�ʼ��Ϸ��
		int pReadyCount = GetReadyPlayerCount();

		if(pReadyCount >= 2 && pReadyCount == GetPlayerCount())
			GameStart();
	}
	m_playersLock.Release();
}

/**
 * ��Ϸ��ʼ�ǵ���
 */
void CRoom::GameStart(void)
{
	// ��ǰ�������������û��㲥��ǰ�����Ѿ���ʼ����Ϸ
    Json::Value root;
    root["MsgId"] = IDD_MESSAGE_FRAME;
    root["MsgSubId"] = IDD_MESSAGE_GAME_START;
    root["UserID"] = GetID();
	ServerPlayerManager.SendMsgToEveryone(root);

	// ���÷���״̬Ϊ��ʼ״̬�������û�״̬Ϊ��ʼ״̬
	SetRoomState(ROOMSTATE_GAMING);
	SetAllPlayerState(PLAYERSTATE_GAMING);

	srand((uint32)time(0)+rand()%10000);

	// ��Ϸ��ʼ��
	OnProcessPlayerGameStartMes();
}

/// ���ڴ����û���ʼ��Ϸ��ʼ��Ϣ
void CRoom::OnProcessPlayerGameStartMes(void)
{
	assert(m_ServerLogicFrame!=NULL);
	if(m_ServerLogicFrame == NULL) return;

	m_playersLock.Acquire();
	// ����ǰ�����еĻ�����
	for(int i=0;i<GetMaxPlayer();i++)
	{
		if(m_PlayerList[i] == NULL) continue;
		if(m_PlayerList[i]->GetType() != PLAYERTYPE_ROBOT) continue;
		if(m_PlayerList[i]->GetRobotLogicFrame() == NULL) continue;

		m_PlayerList[i]->GetRobotLogicFrame()->OnProcessPlayerGameStartMes();
	}

	m_playersLock.Release();

	m_ServerLogicFrame->OnProcessPlayerGameStartMes();
}

/**
 * ��Ϸ����ʱ����
 */
void CRoom::GameEnd(bool isupdateuserdata)
{
	// �ȸ����û��ܵĽ��ֵ
	//UpdateUserTotalResult();

	// ���÷���״̬
	Clear();

    // �ӷ������޳����еĵ����û�,���Ҹ���������ҵ�����
	m_playersLock.Acquire();
	for(int index = 0;index < GetMaxPlayer();index++)
	{
		CPlayer *pPlayer = m_PlayerList[index];
		if(pPlayer == NULL) continue;

		if(m_PlayerList[index]->GetState() == PLAYERSTATE_LOSTLINE)          // ���һ�ֽ��������û��Ѿ����߶���û�л��������뿪���䣬Ȼ���뿪������������Ҫ���µ�¼���ܽ�����Ϸ
		{
			// �򷿼�������ҹ㲥����뿪��������Ϣ
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_FRAME;
            root["MsgSubId"] = IDD_MESSAGE_LEAVE_ROOM;
            root["UserID"] = m_PlayerList[index]->GetID();
			ServerPlayerManager.SendMsgToEveryone(root);

			//m_PlayerList[index]->SetRoomId(-1);
			//m_PlayerList[index]->SetChairIndex(-1);
			//m_PlayerList[index]->SetState(PLAYERSTATE_NORAML);

			ClearPlayer(m_PlayerList[index]);

			uint32 pConnID = pPlayer->GetConnectID();

            // �򷿼�������ҹ㲥����뿪��������Ϣ
            Json::Value root2;
            root2["MsgId"] = IDD_MESSAGE_FRAME;
            root2["MsgSubId"] = IDD_MESSAGE_LEAVE_SERVER;
            root2["UserID"] = pPlayer->GetID();
            ServerPlayerManager.SendMsgToEveryone(root2);

			ServerPlayerManager.ClearPlayer(pPlayer);
			Disconnect(pConnID);

			continue;
		}

		// ���½�״̬���ó�����״̬
		m_PlayerList[index]->SetState(PLAYERSTATE_NORAML);

		// �����û���Ϣ���ͻ���
		UpdateUserScore(m_PlayerList[index]);

		// ���������Ϸ״̬
		pPlayer->setCurGamingState(false);
		ServerDBOperator.SetPlayerGameState(pPlayer);

		// ���һ�ֽ������û��ǻ����ˣ����뿪����
		if(m_PlayerList[index]->GetType() == PLAYERTYPE_ROBOT)
		{
			if(rand()%100 < 50)
			{
				// �ȵ����û��뿪����ӿ�
				OnProcessLeaveRoomMsg(pPlayer->GetChairIndex());

				// ��ǰ�������������û���������뿪������Ϣ
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_LEAVE_ROOM;
                root["UserID"] = m_PlayerList[index]->GetID();
				ServerPlayerManager.SendMsgToEveryone(root);

				ClearPlayer(m_PlayerList[index]);

				pPlayer->SetRoomId(-1);
				pPlayer->SetChairIndex(-1);
				continue;
			}
		}

		// �����ҽ��������ͱ�׼������Ҫ�뿪����
		if(m_PlayerList[index]->GetMoney() < m_ServerSet.lastMoney ||
			pPlayer->GetMoney() < m_ServerSet.m_Pielement * m_g_ServerServiceManager->GetRoomLastDouble())
		{
			// �ȵ����û��뿪����ӿ�
			OnProcessLeaveRoomMsg(m_PlayerList[index]->GetChairIndex());

			// ��ǰ�������������û���������뿪������Ϣ
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_FRAME;
            root["MsgSubId"] = IDD_MESSAGE_LEAVE_ROOM;
            root["UserID"] = m_PlayerList[index]->GetID();
			ServerPlayerManager.SendMsgToEveryone(root);

			ClearPlayer(m_PlayerList[index]);

			// ������ҵ�״̬Ϊ�뿪����
			pPlayer->SetRoomId(-1);
			pPlayer->SetChairIndex(-1);
			continue;
		}
	}
	m_playersLock.Release();

	// ��ǰ�������������û��㲥��ǰ�����Ѿ���������Ϸ
	Json::Value root;
    root["MsgId"] = IDD_MESSAGE_FRAME;
    root["MsgSubId"] = IDD_MESSAGE_GAME_END;
    root["UserID"] = GetID();
	ServerPlayerManager.SendMsgToEveryone(root);

	m_playersLock.Acquire();
	// ����ǰ�����еĻ�����
	for(int i=0;i<GetMaxPlayer();i++)
	{
		if(m_PlayerList[i] == NULL) continue;
		if(m_PlayerList[i]->GetType() != PLAYERTYPE_ROBOT) continue;
		if(m_PlayerList[i]->GetRobotLogicFrame() == NULL) continue;

		m_PlayerList[i]->GetRobotLogicFrame()->OnProcessPlayerGameOverMes();
	}
	m_playersLock.Release();
}

/// �޳�ָ������ң���������Ϸ����ʱ������޳�
bool CRoom::EliminatePlayer(Player *pPlayer)
{
	if(pPlayer == NULL ||
		m_RoomState == ROOMSTATE_GAMING ||
		pPlayer->GetRoomId() != this->GetID())
		return false;

	// �ȵ����û��뿪����ӿ�
	OnProcessLeaveRoomMsg(pPlayer->GetChairIndex());

	// ��ǰ�������������û���������뿪������Ϣ
	Json::Value root;
    root["MsgId"] = IDD_MESSAGE_FRAME;
    root["MsgSubId"] = IDD_MESSAGE_LEAVE_ROOM;
    root["UserID"] = pPlayer->GetID();
	ServerPlayerManager.SendMsgToEveryone(root);

	ClearPlayer(static_cast<CPlayer*>(pPlayer));

	pPlayer->SetRoomId(-1);
	pPlayer->SetChairIndex(-1);

	return true;
}

/// ��ӡ��־
void CRoom::Room_Log(enLogLevel loglevel,std::string log)
{
    switch(loglevel)
    {
    case BASIC:
        {
            LOG_BASIC(log.c_str());
        }
        break;
     case DETAIL:
        {
            LOG_DETAIL(log.c_str());
        }
        break;
    case ERROR:
        {
            LOG_ERROR(log.c_str());
        }
        break;
     case DEBUG:
        {
            LOG_DEBUG(log.c_str());
        }
        break;
    default:
        break;
    }
}

/// �õ��������ò���
std::string CRoom::GetRoomParameters(void)
{
	return ServerDBOperator.GetRoomParamaters(GetGameId(),
											  GetRoomMarking());
}

/**
 * ���ָ����ҵ���Ӯ���(1:��ʾ������ˣ�2����ʾ���Ӯ�ˣ�0����ʾ���䲻Ӯ�����-1�������ƣ�
 *
 * @param pPlayer Ҫ�õ���Ӯ��Ϣ�����
 * @param curresult �������Ӯ
 * @param decResult ���Ŀ����Ӯ
 */
int CRoom::IsSuperPlayerWin(Player *pPlayer,int64 *curresult,int64 *decResult)
{
	if(pPlayer == NULL) return 0;

	int64 pCurResult,pDecResult;
	int pIsControlUser = 0;
	pCurResult=pDecResult=0;

	// ��ȡ��ҿ�������
	ServerDBOperator.getplayercontrolconfig(pPlayer,&pCurResult,&pDecResult,&pIsControlUser);

	// ���������
	if(pIsControlUser == 0)
        return -1;

	if(curresult && decResult)
    {
        *curresult = pCurResult;
        *decResult = pDecResult;
    }

	if(pDecResult == -1) return 0;

	if(pCurResult > pDecResult)
		return 2;
	else if(pCurResult < pDecResult)
		return 1;

	return 0;
}

/// ��⵱ǰ�����Ӯ���(2:��ʾ���Ӯ�ˣ�1����ʾ������ˣ�0����ʾ���䲻Ӯ�����
int CRoom::IsUserWin(int64 *result, int64 *sysMax, int64 *sysMin, bool isUpdate)
{
    int64 RobotWinMax = 0;
    int64 RobotLostMax = 0;

	// ��ȡ�����˿�������
	ServerDBOperator.getrobotcontrolconfig(&RobotWinMax, &RobotLostMax, (int)isUpdate);

	int64 totalResult = ServerDBOperator.GetPlayersTotalResult(0);

	if(result != NULL)
		*result = totalResult;

	*sysMax = RobotWinMax;
	*sysMin = RobotLostMax;

	if(totalResult > RobotWinMax)
		return 2;

	//int64 playertotalResult = ServerDBOperator.GetPlayerTotalResult();

	if(totalResult < RobotLostMax/* && playertotalResult > m_ServerOtherSet.RobotWinMax*/)
		return 1;

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
