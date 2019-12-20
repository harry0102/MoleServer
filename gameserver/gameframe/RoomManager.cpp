#include "RoomManager.h"
#include "PlayerManager.h"

#include "../../include/Common/defines.h"

#include "DBOperator.h"

#include <sstream>
#include <functional>

initialiseSingleton(RoomManager);

RoomManager::RoomManager()
{

}

RoomManager::~RoomManager()
{
	ClearAllRooms();
}

/**
 * ������еķ���
 */
void RoomManager::ClearAllRooms(void)
{
	if(m_RoomList.empty()) return;

	m_RoomLock.Acquire();
	for(int i=0;i<(int)m_RoomList.size();i++)
	{
		if(m_RoomList[i]) delete m_RoomList[i];
		m_RoomList[i] = NULL;
	}

	m_RoomList.clear();

	m_RoomLock.Release();
}

/**
 * ���һ�����䵽�����������
 *
 * @param pRoom Ҫ��ӵķ���
 */
void RoomManager::AddRoom(CRoom *pRoom)
{
	if(pRoom == NULL) return;

	pRoom->SetID((int)m_RoomList.size());

	m_RoomLock.Acquire();
	m_RoomList.push_back(pRoom);
	m_RoomLock.Release();
}

/**
 * �ı�һ����ҵ���Ϸ����
 *
 * @param pPlayer Ҫ�ı���Ϸ��������
 */
bool RoomManager::ChangeGameRoom(CPlayer *pPlayer)
{
	if(pPlayer == NULL ||
		m_RoomList.empty())
		return false;

	// ���ȼ����ҵ�ǰ��״̬�������Ҵ���׼�����ȴ�����Ϸ�У����ߵ�״̬ʱ˵������Ѿ��ڷ����У����ܽ����µķ���
	if(pPlayer->GetState() != PLAYERSTATE_NORAML)
		return false;

	// ��һ��������ǰ�������Ѿ��������������ȴ��ķ���
	bool state = false;

	m_RoomLock.Acquire();
	std::vector<CRoom*>::iterator iter = m_RoomList.begin();
	for(;iter != m_RoomList.end();iter++)
	{
		if((*iter))
		{
			//(*iter)->UpdateAllPlayers();
			// �����Ҳ������㷿��Ҫ������ٽ��ֵ���Ͳ��ܽ��뷿��
			if(pPlayer->GetMoney() < m_ServerSet.lastMoney ||
				pPlayer->GetMoney() < m_ServerSet.m_Pielement * m_g_ServerServiceManager->GetRoomLastDouble()) continue;

			if(!(*iter)->IsFull() &&
				(*iter)->GetPlayerCount() > 0 &&
				(*iter)->GetRoomState() != ROOMSTATE_GAMING)
			{
				// ��һ����ԭ����һ���ķ���
				if(pPlayer->GetRoomId() != (*iter)->GetID())
				{
					//PlayerState oldstate = pPlayer->GetState();

					// �������״̬Ϊ�ı䷿��״̬���������Է�ֹ�����������ǿ�е���
					//pPlayer->SetState(PLAYERSTATE_CHANGEROOM);

					pPlayer->SetRoomId((*iter)->GetID());
					//pPlayer->SetState(oldstate);

					int playerIndex = (*iter)->AddPlayer(pPlayer);
					pPlayer->SetChairIndex(playerIndex);

					state = true;
					break;
				}
			}
		}
	}
	m_RoomLock.Release();

	if(state) return state;

	// �ڶ���������ǰ������û�������ķ�����߿շ���
	state = false;

	m_RoomLock.Acquire();
	iter = m_RoomList.begin();
	for(;iter != m_RoomList.end();iter++)
	{
		if((*iter))
		{
			//(*iter)->UpdateAllPlayers();
			// �����Ҳ������㷿��Ҫ������ٽ��ֵ���Ͳ��ܽ��뷿��
			if(pPlayer->GetMoney() < m_ServerSet.lastMoney ||
				pPlayer->GetMoney() < m_ServerSet.m_Pielement * m_g_ServerServiceManager->GetRoomLastDouble()) continue;

			if(!(*iter)->IsFull() &&
				(*iter)->GetRoomState() != ROOMSTATE_GAMING)
			{
				// ��һ����ԭ����һ���ķ���
				if(pPlayer->GetRoomId() != (*iter)->GetID())
				{
					//PlayerState oldstate = pPlayer->GetState();

					// �������״̬Ϊ�ı䷿��״̬���������Է�ֹ�����������ǿ�е���
					//pPlayer->SetState(PLAYERSTATE_CHANGEROOM);

					if(pPlayer == NULL)
					{
						state = false;
						break;
					}

					pPlayer->SetRoomId((*iter)->GetID());
					//pPlayer->SetState(oldstate);

					int playerIndex = (*iter)->AddPlayer(pPlayer);
					pPlayer->SetChairIndex(playerIndex);

					state = true;
					break;
				}
			}
		}
	}
	m_RoomLock.Release();

	return state;
}

/**
 * ���һ���Թ���ҵ�������
 *
 * @param pPlayer Ҫ��ӵ����
 * @param roomIndex ���Ҫ����ķ��������
 * @param chairIndex ���Ҫ����ķ�������ӵ�����
 */
bool RoomManager::AddLookOnPlayer(CPlayer *pPlayer,int roomIndex,int chairIndex)
{
	if(pPlayer == NULL ||
		m_RoomList.empty())
		return false;

	// ���ȼ����ҵ�ǰ��״̬�������Ҵ���׼�����ȴ�����Ϸ�У����ߵ�״̬ʱ˵������Ѿ��ڷ����У����ܽ����µķ���
	if(pPlayer->GetState() != PLAYERSTATE_NORAML)
		return false;

	// ���Ҫ����ָ�����䣬ָ������λ
	CRoom *pRoom = GetRoomById(roomIndex);
	if(pRoom == NULL) return false;

	// ��������������߷���״̬������Ϸ�еĻ����ǽ����˷����
	if(/*pRoom->IsFull() ||*/
		pRoom->GetRoomState() != ROOMSTATE_GAMING)
		return false;

	// ��ⷿ����ָ������������Ƿ���ڣ���������ǲ��ܼ��뷿���
	if(!pRoom->GetPlayer(chairIndex))
		return false;

	pPlayer->SetRoomId(pRoom->GetID());
	pPlayer->SetChairIndex(pRoom->AddLookOnPlayer(pPlayer,chairIndex));
	pPlayer->SetLookOn(true);

	return true;
}

/**
 * ���һ����ҵ�������
 *
 * @param pPlayer Ҫ��ӵ����
 * @param roomIndex ���Ҫ����ķ��������
 * @param chairIndex ���Ҫ����ķ�������ӵ�����
 */
bool RoomManager::AddPlayer(CPlayer *pPlayer,int roomIndex,int chairIndex,bool isQueue,bool isGaming)
{
	if(pPlayer == NULL ||
		m_RoomList.empty())
		return false;

	// ���ȼ����ҵ�ǰ��״̬�������Ҵ���׼�����ȴ�����Ϸ�У����ߵ�״̬ʱ˵������Ѿ��ڷ����У����ܽ����µķ���
	if(pPlayer->GetState() != PLAYERSTATE_NORAML)
		return false;

	// ���roomIndex����-1,chairIndex����-1��ʾ��ǰ��Ҳ���Ҫ�Ŷӽ��뷿��
	if(roomIndex == -1 && chairIndex == -1)
	{
		//// ��һ��������ǰ�������Ѿ��������������ȴ��ķ���
		std::map<int,CRoom*,std::greater<int> > pCurRoomList;

		m_RoomLock.Acquire();
		for(int index=(int)m_RoomList.size()-1;index >= 0;--index)
		{
			if(m_RoomList[index] == NULL) continue;
			if(isGaming && m_RoomList[index]->GetRoomState() == ROOMSTATE_GAMING) continue;

			int64 pEnterMinMoney,pEnterMaxMoney;
			pEnterMinMoney=pEnterMaxMoney=0;
			m_RoomList[index]->GetEnterMoneyRect(&pEnterMinMoney,&pEnterMaxMoney);

			// �������ǻ����˵Ļ������������ͨ�����ӣ�˽������������û�������
			if(/*pPlayer->GetType() == PLAYERTYPE_ROBOT && */
				!m_RoomList[index]->getEnterPassword().empty() &&
				pPlayer->getEnterPassword() != m_RoomList[index]->getEnterPassword())
				continue;

			// �������ǻ����˵Ļ����������÷�Χ��˽������ʱ����ҵ�Ǯ�����������õķ�Χ
			if(pEnterMinMoney > 0 &&
				pEnterMaxMoney > 0 &&
				/*pPlayer->GetType() == PLAYERTYPE_ROBOT && */
				(pPlayer->GetMoney() < pEnterMinMoney || pPlayer->GetMoney() > pEnterMaxMoney))
				continue;

			if(!m_RoomList[index]->IsFull())
				pCurRoomList[m_RoomList[index]->GetPlayerCount()]  = m_RoomList[index];
		}
		m_RoomLock.Release();

		if(pCurRoomList.empty()) return false;

		std::map<int,CRoom*,std::greater<int> >::iterator iterTwo = pCurRoomList.begin();
		if(iterTwo != pCurRoomList.end())
		{
			// ������Ŷ�״̬�£�����ѡ��һ�����˵ķ���
			if(isQueue)
			{
				if((*iterTwo).second->GetPlayerCount() > 0)
				{
					pPlayer->SetRoomId(((*iterTwo).second)->GetID());
					int playerIndex = ((*iterTwo).second)->AddPlayer(pPlayer);
					pPlayer->SetChairIndex(playerIndex);

					return true;
				}

				return false;
			}

			pPlayer->SetRoomId(((*iterTwo).second)->GetID());
			int playerIndex = ((*iterTwo).second)->AddPlayer(pPlayer);
			pPlayer->SetChairIndex(playerIndex);

			return true;
		}

		return false;
	}
	else
	{
		// ���Ҫ����ָ�����䣬ָ������λ
		CRoom *pRoom = GetRoomById(roomIndex);
		if(pRoom == NULL) return false;

		// ��������������߷���״̬������Ϸ�еĻ����ǽ����˷����
		if(pRoom->IsFull())
			return false;

		if(isGaming && pRoom->GetRoomState() == ROOMSTATE_GAMING)
			return false;

		// ��ⷿ����ָ������������Ƿ���ڣ���������ǲ��ܼ��뷿���
		if(pRoom->GetPlayer(chairIndex))
			return false;

		// ����û���������룬������䲻Ϊ�ղ������벻�ԵĻ������ܽ��뷿��
		if(!pPlayer->getEnterPassword().empty())
		{
			if(!pRoom->IsEmpty() && pRoom->getEnterPassword() != pPlayer->getEnterPassword())
				return false;
		}

		// ������������˽�����ƣ������ҽ�Ҳ�����������Ҳû�����뷿��
		int64 pfirst,psecond;
		pfirst=psecond=0;
		pRoom->GetEnterMoneyRect(&pfirst,&psecond);

		if(pfirst > 0 && psecond > 0 && pfirst < psecond)
		{
			if(pPlayer->GetMoney() < pfirst ||
				pPlayer->GetMoney() > psecond)
				return false;
		}

		pPlayer->SetRoomId(pRoom->GetID());
		int playerIndex = pRoom->AddPlayer(pPlayer,chairIndex);
		if(playerIndex == -1)
		{
			LOG_ERROR("���%d�ڽ��뷿���ʱ��ʧ���ˣ����Ǹ������⣡",pPlayer->GetID());
			return false;
		}
		pPlayer->SetChairIndex(playerIndex);

		// ����ǵ�һ���뷿��ľ����÷���
		if(pRoom->GetPlayerCount() <= 1)
		{
			pRoom->SetEnterPassword(pPlayer->getEnterPassword());

			int64 pfirst,psecond;
			pfirst=psecond=0;
			pPlayer->GetEnterMoneyRect(&pfirst,&psecond);

			pRoom->SetEnterMoneyRect(pfirst,psecond);

			// ��������������е��������ͨ����ҽ�����Ϸ�������Ϣ
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_FRAME;
            root["MsgSubId"] = IDD_MESSAGE_FRAME_ROOM_JOINSET;
            root["UserID"] = pRoom->GetID();
            root["EnterPassword"] = pRoom->getEnterPassword();
            root["first"] = (uint32)pfirst;
            root["second"] = (uint32)psecond;
			ServerPlayerManager.SendMsgToEveryone(root);
		}

		return true;
	}

	return false;
}

/**
 * �ӷ��������ָ�������
 *
 * @param pPlayer Ҫ��������
 */
void RoomManager::ClearPlayer(CPlayer *pPlayer)
{
	if(pPlayer == NULL ||
		m_RoomList.empty())
		return;

	m_RoomLock.Acquire();
	std::vector<CRoom*>::iterator iter = m_RoomList.begin();
	for(;iter != m_RoomList.end();iter++)
	{
		if((*iter) &&
			(*iter)->IsExist(pPlayer))
		{
			(*iter)->ClearPlayer(pPlayer);
			break;
		}
	}
	m_RoomLock.Release();
}

/**
 * �ӵ�ǰ������ɾ��ָ�������
 *
 * @param pPlayer Ҫɾ�������
 *
 * @return  ����������ɹ������棬���򷵻ؼ�
 */
bool RoomManager::DeletePlayer(CPlayer *pPlayer)
{
	if(pPlayer == NULL ||
		m_RoomList.empty())
		return false;

	bool state = false;

	m_RoomLock.Acquire();
	std::vector<CRoom*>::iterator iter = m_RoomList.begin();
	for(;iter != m_RoomList.end();iter++)
	{
		if((*iter) &&
			(*iter)->IsExist(pPlayer))
		{
			(*iter)->DeletePlayer(pPlayer);
			state = true;
			break;
		}
	}
	m_RoomLock.Release();

	return state;
}

/**
 * ���ݷ���ID�ŵõ�����
 *
 * @param id Ҫ�õ��ķ����ID
 *
 * @return ����������ID�ܵõ�������䣬����������䣬���򷵻�NULL
 */
CRoom* RoomManager::GetRoomById(int id)
{
	if(m_RoomList.empty()) return NULL;

	CRoom *pRoom = NULL;

	m_RoomLock.Acquire();
	std::vector<CRoom*>::iterator iter = m_RoomList.begin();
	for(;iter != m_RoomList.end();iter++)
	{
		if((*iter)->GetID() == id)
		{
			pRoom = (*iter);
			break;
		}
	}
	m_RoomLock.Release();

	return pRoom;
}

/// �õ�һ�����еķ���
CRoom* RoomManager::GetFreeRoom(void)
{
	if(m_RoomList.empty()) return NULL;

	CRoom *pRoom = NULL;

	m_RoomLock.Acquire();
	std::vector<CRoom*>::iterator iter = m_RoomList.begin();
	for(;iter != m_RoomList.end();++iter)
	{
		if(!(*iter)->IsFull() &&
			(*iter)->GetRoomState() != ROOMSTATE_GAMING)
		{
			pRoom = (*iter);
			break;
		}
	}
	m_RoomLock.Release();

	return pRoom;
}

/// �õ�һ��δ������Ϊ�յķ���
CRoom* RoomManager::GetUnderFullRoom(void)
{
	if(m_RoomList.empty()) return NULL;

	CRoom *pRoom = NULL;

	// �����з�������Ѱ���������Ǹ�����
	m_RoomLock.Acquire();
	int pPlayerCount = 0;
	std::vector<CRoom*>::iterator iter = m_RoomList.begin();
	for(;iter != m_RoomList.end();++iter)
	{
		if(!(*iter)->IsFull() &&
			(*iter)->GetPlayerCount() > 0 &&
			(*iter)->GetRoomState() != ROOMSTATE_GAMING)
		{
			if((*iter)->GetPlayerCount() > pPlayerCount)
			{
				pRoom = (*iter);
				pPlayerCount = (*iter)->GetPlayerCount();
			}
		}
	}
	m_RoomLock.Release();

	return pRoom;
}

/// �õ�һ���շ���
CRoom* RoomManager::GetEmptyRoom(void)
{
	if(m_RoomList.empty()) return NULL;

	CRoom *pRoom = NULL;

	m_RoomLock.Acquire();
	std::vector<CRoom*>::iterator iter = m_RoomList.begin();
	for(;iter != m_RoomList.end();iter++)
	{
		if((*iter)->IsEmpty())
		{
			pRoom = (*iter);
			break;
		}
	}
	m_RoomLock.Release();

	return pRoom;
}

/// �õ���ǰ�����л��ж�����
int RoomManager::GetRoomPlayerCount(void)
{
	if(m_RoomList.empty()) return 0;

	int Count = 0;

	m_RoomLock.Acquire();
	std::vector<CRoom*>::iterator iter = m_RoomList.begin();
	for(;iter != m_RoomList.end();iter++)
	{
		Count += (*iter)->GetPlayerCount();
	}
	m_RoomLock.Release();

	return Count;
}

/**
 * �������е���Ϸ����
 */
void RoomManager::ResetAllGameRooms(void)
{
	if(m_RoomList.empty())
		return;

	m_RoomLock.Acquire();
	std::vector<CRoom*>::iterator iter = m_RoomList.begin();
	for(;iter != m_RoomList.end();iter++)
	{
		if((*iter))
			(*iter)->ClearAllPlayers();
	}
	m_RoomLock.Release();
}

/**
 * ���ڴ�����Ҹı���Ϸ����
 *
 * @param connId Ҫ������������ӵĿͻ�������ID
 */
void RoomManager::OnProcessChangeGameRoomMes(uint32 connId)
{
	CPlayer *pPlayer = ServerPlayerManager.GetNewPlayer(connId);
	if(pPlayer != NULL)
	{
		if(ChangeGameRoom(pPlayer))
		{
			// ��������������е��������ͨ����ҽ�����Ϸ�������Ϣ
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_FRAME;
            root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
            root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_SUCC;
            root["UserID"] = pPlayer->GetID();
            root["RoomId"] = pPlayer->GetRoomId();
            root["ChairIndex"] = pPlayer->GetChairIndex();
			ServerPlayerManager.SendMsgToEveryone(root);
		}
		else
		{
			// ��ǰ��ҷ��ͽ�����Ϸ����ʧ�ܵ���Ϣ
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_FRAME;
            root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
            root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_FAIL;
            root["UserID"] = pPlayer->GetID();
			Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());
		}
	}
}

/**
 * ���ڴ������ڶϿ�����������Ϣ
 *
 * @param connId Ҫ�Ͽ��������ӵĿͻ�������ID
 */
void RoomManager::OnProcessDisconnectNetMes(uint32 connId)
{
	// ���ȸ����û�����ID�õ��û���Ȼ������û���Ϣ�õ�������Ϣ����󽻸����䴦��
	CPlayer *pPlayer = ServerPlayerManager.GetPlayer(connId);
	if(pPlayer == NULL) return;

	CRoom *pRoom = GetRoomById(pPlayer->GetRoomId());
	if(pRoom == NULL)
	{
		// ����û�û�н����κη���Ļ���ɾ��������
        Json::Value root;
        root["MsgId"] = IDD_MESSAGE_FRAME;
        root["MsgSubId"] = IDD_MESSAGE_LEAVE_SERVER;
        root["UserID"] = pPlayer->GetID();
		ServerPlayerManager.SendMsgToEveryone(root);

		uint32 pConnID = pPlayer->GetConnectID();
		ServerPlayerManager.ClearPlayer(pPlayer);
		Disconnect(pConnID);
		return;
	}

	bool isGaming = (pPlayer->GetState() == PLAYERSTATE_GAMING);

	if(isGaming)
	{
		pPlayer->SetState(PLAYERSTATE_LOSTLINE);

		//�������״̬
		ServerDBOperator.SetPlayerGameState(pPlayer);

		// �ȴ�����б���ժ��������
		ServerPlayerManager.DeletePlayer(pPlayer);
		// Ȼ����뵽�����б���
		ServerPlayerManager.AddLostPlayer(pPlayer);

		// �򷿼�������ҹ㲥����뿪������Ϣ
        Json::Value root;
        root["MsgId"] = IDD_MESSAGE_FRAME;
        root["MsgSubId"] = IDD_MESSAGE_FALLLINE_ROOM;
        root["UserID"] = pPlayer->GetID();
		ServerPlayerManager.SendMsgToEveryone(root);
	}
	else
	{
		// ����Ƕ���״̬��ֱ���˳�
		if(pPlayer->GetState() == PLAYERSTATE_LOSTLINE)
		{
			//mole2d::network::System_Log("�Ͽ����ӵ�ʱ����Ҳ��ܴ��ڶ���״̬��");

			// �ȴ�����б���ժ��������
			ServerPlayerManager.DeletePlayer(pPlayer);
			// Ȼ����뵽�����б���
			ServerPlayerManager.AddLostPlayer(pPlayer);

			return;
		}

		// �򷿼�������ҹ㲥����뿪��������Ϣ
        Json::Value root;
        root["MsgId"] = IDD_MESSAGE_FRAME;
        root["MsgSubId"] = IDD_MESSAGE_LEAVE_SERVER;
        root["UserID"] = pPlayer->GetID();
		ServerPlayerManager.SendMsgToEveryone(root);

		m_RoomLock.Acquire();

		pRoom->OnProcessLeaveRoomMsg(pPlayer->GetChairIndex());
		pRoom->ClearPlayer(pPlayer);

		uint32 pConnID = pPlayer->GetConnectID();
		ServerPlayerManager.ClearPlayer(pPlayer);
		Disconnect(pConnID);

        // ׼�����˵��û��Ƿ������ʼ��Ϸ
        pRoom->OnProcessContinueGaming();

		m_RoomLock.Release();
		return;
	}

	m_RoomLock.Acquire();
	pRoom->OnProcessOfflineRoomMes(pPlayer->GetChairIndex());

	//if(pPlayer->GetState() != PLAYERSTATE_LOSTLINE)
	//	pRoom->ClearPlayer(pPlayer);

	//if(m_ServerSet.GameType != ROOMTYPE_BISAI)
	//{
	//	// ��������е��û�ȫ�����ߣ�����շ���״̬
	//	if(pRoom->GetPlayerCount(PLAYERSTATE_LOSTLINE) == pRoom->GetMaxPlayer())
	//	{
	//		pRoom->GameEnd();
	//		//pRoom->AllPlayerLeveaServer();
	//	}
	//}
	m_RoomLock.Release();
}

/**
 * ���ڴ�����յ���������Ϣ
 *
 * @param connId Ҫ����Ŀͻ���
 * @param mes Ҫ����Ŀͻ��˵���Ϣ
 */
void RoomManager::OnProcessNetMes(uint32 connId,Json::Value &mes)
{
	if(m_RoomList.empty()) return;

	Player *pPlayer = ServerPlayerManager.GetPlayer(connId);
	if(pPlayer == NULL) return;

	CRoom *pRoom = ServerRoomManager.GetRoomById(pPlayer->GetRoomId());
	if(pRoom == NULL) return;

	m_RoomLock.Acquire();
	pRoom->OnProcessNetMes(connId,mes);
	m_RoomLock.Release();
}

///ʱ���¼�
bool RoomManager::OnEventTimer(uint32 dwTimerID, uint32 wBindParam)
{
	m_RoomLock.Acquire();
	std::vector<CRoom*>::iterator iter = m_RoomList.begin();
	for(;iter != m_RoomList.end();++iter)
	{
		if((*iter) == NULL) continue;

		(*iter)->OnEventTimer(dwTimerID,wBindParam);
	}
	m_RoomLock.Release();

	return true;
}
