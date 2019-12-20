#include "PlayerManager.h"
#include "CRoom.h"
#include "RoomManager.h"
#include "DBOperator.h"
#include "../GameFrameManager.h"

#include "../../include/Common/defines.h"

initialiseSingleton(PlayerManager);

PlayerManager::PlayerManager()
{

}

PlayerManager::~PlayerManager()
{
	ClearAllPlayers();
}

/**
 * ������е����
 */
void PlayerManager::ClearAllPlayers(void)
{
	if(!m_PlayerList.empty())
	{
		std::map<uint32,CPlayer*>::iterator iter = m_PlayerList.begin();
		for(;iter != m_PlayerList.end();++iter)
		{
			if((*iter).second)
			{
				// ���������Ϸ״̬
				(*iter).second->setCurGameID(0);
				(*iter).second->setCurServerId(0);
				(*iter).second->setCurChairIndex(-1);
				(*iter).second->setCurTableIndex(-1);
				(*iter).second->setCurGamingState(false);
				ServerDBOperator.SetPlayerGameState((*iter).second);
			}

			if((*iter).second) delete (*iter).second;
			(*iter).second = NULL;
		}

		m_PlayerList.clear();
	}

	if(!m_RobotList.empty())
	{
		std::map<uint32,CPlayer*>::iterator iter = m_RobotList.begin();
		for(;iter != m_RobotList.end();++iter)
		{
			if((*iter).second)
			{
				// ���������Ϸ״̬
				(*iter).second->setCurGameID(0);
				(*iter).second->setCurServerId(0);
				(*iter).second->setCurChairIndex(-1);
				(*iter).second->setCurTableIndex(-1);
				(*iter).second->setCurGamingState(false);
				ServerDBOperator.SetPlayerGameState((*iter).second);
			}

			if((*iter).second) delete (*iter).second;
			(*iter).second = NULL;
		}

		m_RobotList.clear();
	}

	if(!m_LostPlayerList.empty())
	{
		std::map<uint32,CPlayer*>::iterator iter = m_LostPlayerList.begin();
		for(;iter != m_LostPlayerList.end();++iter)
		{
			if((*iter).second)
			{
				// ���������Ϸ״̬
				(*iter).second->setCurGameID(0);
				(*iter).second->setCurServerId(0);
				(*iter).second->setCurChairIndex(-1);
				(*iter).second->setCurTableIndex(-1);
				(*iter).second->setCurGamingState(false);
				ServerDBOperator.SetPlayerGameState((*iter).second);
			}

			if((*iter).second) delete (*iter).second;
			(*iter).second = NULL;
		}

		m_LostPlayerList.clear();
	}
}

/**
 * ���һ����ҵ�ϵͳ��
 *
 * @param pPlayer Ҫ��ӵ�ϵͳ�е����
 */
bool PlayerManager::AddPlayer(CPlayer *pPlayer)
{
	if(pPlayer == NULL) return false;

	// �������Ƿ��Ѿ������ڷ������У�������ھͲ�������
	if(GetPlayer(pPlayer->GetConnectID()))
	{
		//mole2d::network::System_Log("�����ҳ����غŵ�����ˡ�");
		return false;
	}

	m_PlayersLock.Acquire();
	m_PlayerList.insert(std::pair<uint32,CPlayer*>(pPlayer->GetConnectID(),pPlayer));
	m_PlayersLock.Release();

	return true;
}

/// ���һ�������˵�ϵͳ��
bool PlayerManager::AddRobot(CPlayer *pPlayer)
{
	if(pPlayer == NULL) return false;

	// �������Ƿ��Ѿ������ڷ������У�������ھͲ�������
	if(GetPlayer(pPlayer->GetConnectID()))
	{
		//mole2d::network::System_Log("��ӻ����˳����غŵ�����ˡ�");
		return false;
	}

	m_PlayersLock.Acquire();
	m_RobotList.insert(std::pair<uint32,CPlayer*>(pPlayer->GetConnectID(),pPlayer));
	m_PlayersLock.Release();

	return true;
}

///**
// * ��ϵͳ��ɾ��ָ������ID�����
// *
// * @param connId Ҫɾ���Ŀͻ��˵�ID
// */
//void PlayerManager::ClearPlayerByconnId(uint32 connId)
//{
//	if(m_PlayerList.empty() || connId == NULL) return;
//
//	m_PlayersLock.Acquire();
//	std::map<uint32,CPlayer*>::iterator iter = m_PlayerList.find(connId);
//	if(iter != m_PlayerList.end())
//	{
//		CPlayer *pPlayer = dynamic_cast<CPlayer*>((*iter).second);
//		if(pPlayer == NULL)
//		{
//			m_PlayersLock.Release();
//			return;
//		}
//
//		if(pPlayer->GetConnectID() == connId)
//		{
//			delete pPlayer;
//			pPlayer = NULL;
//
//			m_PlayerList.erase(iter);
//		}
//	}
//	m_PlayersLock.Release();
//}

/**
 * ͨ���ͻ�������ID�õ��ͻ���
 *
 * @param connId Ҫ�õ��Ŀͻ��˵�ID
 *
 * @return �������ͻ��˴��ڷ�������ͻ��ˣ����򷵻�NULL
 */
CPlayer* PlayerManager::GetPlayer(uint32 connId)
{
	if(connId <= 0) return NULL;

	CPlayer *pPlayer = NULL;

	m_PlayersLock.Acquire();
	bool isFinded=false;

	std::map<uint32,CPlayer*>::iterator iter = m_PlayerList.find(connId);
	if(iter != m_PlayerList.end())
	{
		if((*iter).second &&
			(*iter).second->GetConnectID() == connId)
		{
			pPlayer = (*iter).second;
			isFinded=true;
		}
	}

	if(isFinded == false)
	{
		//�������л�����
		iter = m_RobotList.find(connId);
		if(iter != m_RobotList.end())
		{
			if((*iter).second &&
				(*iter).second->GetConnectID() == connId)
			{
				pPlayer = (*iter).second;
				isFinded=true;
			}
		}
	}

	if(isFinded == false && !m_LostPlayerList.empty())
	{
		//�������е������
		std::map<uint32,CPlayer*>::iterator iterLost = m_LostPlayerList.begin();
		for(;iterLost != m_LostPlayerList.end();++iterLost)
		{
			if((*iterLost).second &&
				(*iterLost).second->GetConnectID() == connId)
			{
				pPlayer = (*iterLost).second;
				isFinded=true;
			}
		}
	}
	m_PlayersLock.Release();

	return pPlayer;
}

/// �õ����л����˸���
int PlayerManager::GetFreeRobotCount(void)
{
	if(m_RobotList.empty()) return 0;

	int pcount = 0;

	m_PlayersLock.Acquire();

	std::map<uint32,CPlayer*>::iterator freerobotiter = m_RobotList.begin();      /**< ���������ȡ���л����� */
	for(;freerobotiter != m_RobotList.end();++freerobotiter)
	{
		if((*freerobotiter).second &&
			(*freerobotiter).second->GetState() == PLAYERSTATE_NORAML &&
			(*freerobotiter).second->GetRoomId() == -1)
		{
			pcount+=1;
		}
	}

	m_PlayersLock.Release();

	return pcount;
}

/**
 * �õ����еĻ�����
 */
CPlayer* PlayerManager::GetFreeRobot(void)
{
	if(m_RobotList.empty()) return NULL;

	CPlayer *pPlayer = NULL;

	m_PlayersLock.Acquire();

	std::vector<CPlayer*> probotList;

	std::map<uint32,CPlayer*>::iterator freerobotiter = m_RobotList.begin();      /**< ���������ȡ���л����� */
	for(;freerobotiter != m_RobotList.end();++freerobotiter)
	{
		if((*freerobotiter).second &&
			(*freerobotiter).second->GetState() == PLAYERSTATE_NORAML &&
			(*freerobotiter).second->GetRoomId() == -1)
		{
			probotList.push_back((*freerobotiter).second);
		}
	}

	if(!probotList.empty())
	{
		pPlayer = probotList[rand()%(int)probotList.size()];
	}

	m_PlayersLock.Release();

	return pPlayer;
}

/**
 * ͨ���ͻ�������ID�õ��յ�½�Ŀͻ���
 *
 * @param connId Ҫ�õ��Ŀͻ��˵�ID
 *
 * @return �������ͻ��˴��ڷ�������ͻ��ˣ����򷵻�NULL
 */
CPlayer* PlayerManager::GetNewPlayer(uint32 connId)
{
	if(connId <= 0) return NULL;

	CPlayer *pPlayer = NULL;

	m_PlayersLock.Acquire();
	bool isFinded = false;
	std::map<uint32,CPlayer*>::iterator iter = m_PlayerList.find(connId);
	if(iter != m_PlayerList.end())
	{
		CPlayer *tmpPlayer = (*iter).second;
		if( tmpPlayer &&
			tmpPlayer->GetConnectID() == connId &&
			tmpPlayer->GetState() == PLAYERSTATE_NORAML)
		{
			pPlayer = (*iter).second;
			isFinded = true;
		}
	}

	// ���һ�����
	if(isFinded == false)
	{
		iter = m_RobotList.find(connId);
		if(iter != m_RobotList.end())
		{
			CPlayer *tmpPlayer = (*iter).second;
			if( tmpPlayer &&
				tmpPlayer->GetConnectID() == connId &&
				tmpPlayer->GetState() == PLAYERSTATE_NORAML)
			{
				pPlayer = (*iter).second;
				isFinded = true;
			}
		}
	}
	m_PlayersLock.Release();

	return pPlayer;
}

/**
 * ͨ�����ID�õ��ͻ���
 *
 * @param id Ҫ�õ����û���ID
 *
 * @return ������ָ��ID���û����ڷ�������ͻ��ˣ����򷵻�NULL
 */
CPlayer* PlayerManager::GetPlayerById(uint32 id)
{
	if(id <= 0) return NULL;

	CPlayer *pPlayer = NULL;

	m_PlayersLock.Acquire();
	bool isFinded = false;
	std::map<uint32,CPlayer*>::iterator iter = m_PlayerList.begin();
	for(;iter != m_PlayerList.end();++iter)
	{
		if((*iter).second &&
			(*iter).second->GetID() == id)
		{
			pPlayer = (*iter).second;
			isFinded = true;
			break;
		}
	}

	// ���һ�����
	if(isFinded == false)
	{
		iter = m_RobotList.begin();
		for(;iter != m_RobotList.end();++iter)
		{
			if((*iter).second &&
				(*iter).second->GetID() == id)
			{
				pPlayer = (*iter).second;
				isFinded = true;
				break;
			}
		}
	}
	m_PlayersLock.Release();

	return pPlayer;
}

/// ��ϵͳ��ժ��ָ�������
void PlayerManager::DeletePlayer(CPlayer *pPlayer)
{
	if(pPlayer == NULL || m_PlayerList.empty()) return;

	m_PlayersLock.Acquire();
	std::map<uint32,CPlayer*>::iterator iter = m_PlayerList.find(pPlayer->GetConnectID());
	if(iter != m_PlayerList.end())
	{
		m_PlayerList.erase(iter);
	}
	m_PlayersLock.Release();
}

/**
 * ��ϵͳ��ɾ��ָ�������
 *
 * @param pPlayer Ҫɾ�������
 */
void PlayerManager::ClearPlayer(CPlayer *pPlayer)
{
	if(pPlayer == NULL) return;

	// ���������Ϸ״̬
	pPlayer->setCurGameID(0);
	pPlayer->setCurServerId(0);
	pPlayer->setCurChairIndex(-1);
	pPlayer->setCurTableIndex(-1);
	pPlayer->setCurGamingState(false);
	pPlayer->SetState(PLAYERSTATE_NORAML);
	ServerDBOperator.SetPlayerGameState(pPlayer);

	m_PlayersLock.Acquire();

	if(!m_LostPlayerList.empty())
	{
		std::map<uint32,CPlayer*>::iterator iter = m_LostPlayerList.find(pPlayer->GetID());
		if(iter != m_LostPlayerList.end())
		{
			CPlayer *pPlayer = dynamic_cast<CPlayer*>((*iter).second);
			if(pPlayer != NULL)
			{
				SafeDelete(pPlayer);
				pPlayer = NULL;
			}

			m_LostPlayerList.erase(iter);

			m_PlayersLock.Release();

            //���·�������������
            ServerGameFrameManager.UpdateGameRoomInfo();

			return;
		}
	}

	if(!m_PlayerList.empty())
	{
		std::map<uint32,CPlayer*>::iterator iter = m_PlayerList.find(pPlayer->GetConnectID());
		if(iter != m_PlayerList.end())
		{
			CPlayer *pPlayer = dynamic_cast<CPlayer*>((*iter).second);
			if(pPlayer != NULL)
			{
				SafeDelete(pPlayer);
				pPlayer = NULL;
			}

			m_PlayerList.erase(iter);

			m_PlayersLock.Release();

			//���·�������������
            ServerGameFrameManager.UpdateGameRoomInfo();

			return;
		}
	}

	if(!m_RobotList.empty())
	{
		std::map<uint32,CPlayer*>::iterator iter = m_RobotList.find(pPlayer->GetConnectID());
		if(iter != m_RobotList.end())
		{
			CPlayer *pPlayer = dynamic_cast<CPlayer*>((*iter).second);
			if(pPlayer != NULL)
			{
				SafeDelete(pPlayer);
				pPlayer = NULL;
			}

			m_RobotList.erase(iter);

			m_PlayersLock.Release();

			//���·�������������
            ServerGameFrameManager.UpdateGameRoomInfo();

			return;
		}
	}

	m_PlayersLock.Release();
}

/**
 * ������ҵ�����ID�������
 *
 * @param pPlayer Ҫ�������õ��������
 */
void PlayerManager::Reset(CPlayer *pPlayer)
{
	if(pPlayer == NULL) return;

	m_PlayersLock.Acquire();
	std::map<uint32,CPlayer*>::iterator newiter = m_PlayerList.find(pPlayer->GetConnectID());
	if(newiter == m_PlayerList.end())
	{
		m_PlayerList.insert(std::pair<uint32,CPlayer*>(pPlayer->GetConnectID(),pPlayer));
	}
	m_PlayersLock.Release();
}

/**
 * ������Ϣ����ǰ���ߵ��������
 *
 * @param msg Ҫ���͵���Ϣ
 */
void PlayerManager::SendMsgToEveryone(Json::Value &msg)
{
	if(m_PlayerList.empty()) return;

	m_PlayersLock.Acquire();
	std::map<uint32,CPlayer*>::iterator iter = m_PlayerList.begin();
	for(;iter != m_PlayerList.end();++iter)
	{
		CPlayer *pPlayer = dynamic_cast<CPlayer*>((*iter).second);
		if(pPlayer == NULL)
			continue;

		if(IsConnected(pPlayer->GetConnectID()) == false)
			continue;

		Sendhtml5(pPlayer->GetConnectID(),(const char*)msg.toStyledString().c_str(),msg.toStyledString().length());
	}
	m_PlayersLock.Release();
}

/**
 * ������Ϣ������ָ����ҵ������������
 *
 * @param connId Ҫ�ų��������ҵ�����ID
 * @param msg Ҫ���͵���Ϣ
 */
void PlayerManager::SendMsgToOtherPlayer(uint32 connId,Json::Value &msg)
{
	if(m_PlayerList.empty()) return;

	m_PlayersLock.Acquire();
	std::map<uint32,CPlayer*>::iterator iter = m_PlayerList.begin();
	for(;iter != m_PlayerList.end();++iter)
	{
		CPlayer *pPlayer = dynamic_cast<CPlayer*>((*iter).second);
		if(pPlayer == NULL || pPlayer->GetConnectID() == connId) continue;

		if(IsConnected(pPlayer->GetConnectID()) == false)
			continue;

		Sendhtml5(pPlayer->GetConnectID(),(const char*)msg.toStyledString().c_str(),msg.toStyledString().length());
	}
	m_PlayersLock.Release();
}

/// ������Ϣ��ָ��ID�����
void PlayerManager::SendMsgToPlayer(uint32 UserID,Json::Value &msg)
{
	if(m_PlayerList.empty() || UserID <= 0) return;

	m_PlayersLock.Acquire();
	std::map<uint32,CPlayer*>::iterator iter = m_PlayerList.begin();
	for(;iter != m_PlayerList.end();++iter)
	{
		CPlayer *pPlayer = dynamic_cast<CPlayer*>((*iter).second);
		if(pPlayer == NULL || pPlayer->GetID() != UserID) continue;

		if(IsConnected(pPlayer->GetConnectID()) == false)
			continue;

		Sendhtml5(pPlayer->GetConnectID(),(const char*)msg.toStyledString().c_str(),msg.toStyledString().length());
		break;
	}
	m_PlayersLock.Release();
}

/// �õ�����ܵ���Ӯ
int64 PlayerManager::GetPlayerTotalResult(void)
{
	int64 totalResult = 0;

	m_PlayersLock.Acquire();
	std::map<uint32,CPlayer*>::iterator iter = m_PlayerList.begin();
	for(;iter != m_PlayerList.end();++iter)
	{
		totalResult+=(*iter).second->GetTotalResult();
	}
	m_PlayersLock.Release();

	return totalResult;
}

/// �õ��������ܵ���Ӯ
int64 PlayerManager::GetRobotTotalResult(void)
{
	int64 totalResult = 0;

	m_PlayersLock.Acquire();
	std::map<uint32,CPlayer*>::iterator iter = m_RobotList.begin();
	for(;iter != m_RobotList.end();++iter)
	{
		totalResult+=(*iter).second->GetTotalResult();
	}
	m_PlayersLock.Release();

	return totalResult;
}

///ʱ���¼�
bool PlayerManager::OnEventTimer(uint32 dwTimerID, uint32 wBindParam)
{
	m_PlayersLock.Acquire();
	std::map<uint32,CPlayer*>::iterator iter = m_RobotList.begin();
	for(;iter != m_RobotList.end();++iter)
	{
		if((*iter).second == NULL) continue;

		(*iter).second->OnEventTimer(dwTimerID,wBindParam);
	}
	m_PlayersLock.Release();

	return true;
}

/// ���������������ʾ��λ��Ϣ
void PlayerManager::UpdatePlayersTableInfo(void)
{
	if(m_PlayerList.empty()) return;

	m_PlayersLock.Acquire();
	std::map<uint32,CPlayer*>::iterator iter = m_PlayerList.begin();
	for(;iter != m_PlayerList.end();++iter)
	{
		if((*iter).second == NULL) continue;
		if((*iter).second->GetRoomId() < 0 || (*iter).second->GetChairIndex() < 0 || (*iter).second->GetState() != PLAYERSTATE_READY)
			continue;

		// ׼��ʱ����볬��20��
		if((DWORD)time(NULL) - (*iter).second->GetReadyTime() < 20)
			continue;

		CRoom *pRoom = ServerRoomManager.GetUnderFullRoom();
		if(pRoom == NULL) break;

		CMolMessageOut out(IDD_MESSAGE_FRAME);
		out.write16(IDD_MESSAGE_FRAME_ROOM_UNDERFULL);
		out.write16(pRoom->GetID());
		out.write16(pRoom->GetMaxPlayer() - pRoom->GetPlayerCount());

		Send((*iter).second->GetConnectID(),out);
	}
	m_PlayersLock.Release();
}

/// ���һ����ҵ������б���
void PlayerManager::AddLostPlayer(CPlayer *pPlayer)
{
	if(pPlayer == NULL) return;

	m_PlayersLock.Acquire();
	std::map<uint32,CPlayer*>::iterator iter = m_LostPlayerList.find(pPlayer->GetID());
	if(iter == m_LostPlayerList.end())
	{
		// �ȱ���ԭ���ϵ�����ID
		pPlayer->SetOffLineConnectId(pPlayer->GetConnectID());

		// �������ID����
		pPlayer->SetConnectID(IDD_LOSTONLINE_SOCKET_START+pPlayer->GetID());

		m_LostPlayerList.insert(std::pair<uint32,CPlayer*>(pPlayer->GetID(),pPlayer));
	}
	m_PlayersLock.Release();
}

/// ����ID�õ��������
CPlayer* PlayerManager::GetLostPlayer(uint32 userId)
{
	CPlayer *pPlayer = NULL;

	m_PlayersLock.Acquire();

	bool isfinded = false;

	if(!m_LostPlayerList.empty())
	{
		std::map<uint32,CPlayer*>::iterator iter = m_LostPlayerList.find(userId);
		if(iter != m_LostPlayerList.end())
		{
			pPlayer = (*iter).second;
			isfinded = true;
		}
	}

	//if(!isfinded)
	//{
	//	// ��������б���û���ҵ�����Ҫȥ��������б������ˣ���Ϊ�п��ܳ���
	//	// �������������������£���ҿ�������������б��У���û�г����ڵ����б���
	//	if(!m_PlayerList.empty())
	//	{
	//		std::map<uint32,CPlayer*>::iterator iter = m_PlayerList.begin();
	//		for(;iter != m_PlayerList.end();++iter)
	//		{
	//			if((*iter).second == NULL || (*iter).second->GetID() != userId)
	//				continue;

	//			if((*iter).second->GetState() == PLAYERSTATE_LOSTLINE)
	//			{
	//				pPlayer = (*iter).second;
	//				isfinded = true;
	//				break;
	//			}
	//		}
	//	}

	//}

	m_PlayersLock.Release();

	return pPlayer;
}

/// ����IDɾ���������
void PlayerManager::DeleteLostPlayer(uint32 userId)
{
	m_PlayersLock.Acquire();
	std::map<uint32,CPlayer*>::iterator iter = m_LostPlayerList.find(userId);
	if(iter != m_LostPlayerList.end())
	{
		m_LostPlayerList.erase(iter);
	}
	m_PlayersLock.Release();
}

/// �õ���⵽���Ѿ����ߵ�����б�
void PlayerManager::GetRealLostPlayerList(void)
{
	std::vector<uint32> pTmpLostPlayers;

	m_PlayersLock.Acquire();
	std::map<uint32,CPlayer*>::iterator iter = m_PlayerList.begin();
	for(;iter != m_PlayerList.end();++iter)
	{
		if((*iter).second == NULL) continue;
		if(IsConnected((*iter).second->GetConnectID())) continue;

		pTmpLostPlayers.push_back((*iter).second->GetConnectID());
	}

	for(int i=0;i<(int)pTmpLostPlayers.size();i++)
	{
		ServerGameFrameManager.OnProcessDisconnectNetMes(pTmpLostPlayers[i]);
	}
	m_PlayersLock.Release();
}
