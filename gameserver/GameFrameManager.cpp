#include "GameFrameManager.h"
#include "../include/Common/defines.h"
#include "../../include/molnet/Network.h"
#include "gameframe/DBOperator.h"
#include "gameframe/RoomManager.h"
#include "gameframe/PlayerManager.h"

#include <map>
#include <sstream>

#define IDD_SEND_PLAYER_MAX_COUNT         30               // һ����෢�Ͷ��ٸ��˵�����


initialiseSingleton(GameFrameManager);

/**
 * ���캯��
 */
GameFrameManager::GameFrameManager()
{
}

/**
 * ��������
 */
GameFrameManager::~GameFrameManager()
{
}

/**
 * ���ڴ�����յ���������Ϣ
 *
 * @param connId Ҫ����Ŀͻ��˵�����ID
 * @param mes ���յ��Ŀͻ��˵���Ϣ
 */
void GameFrameManager::OnProcessNetMes(uint32 connId,CMolMessageIn *mes)
{
	if(mes == NULL) return;

    Json::Reader reader;
    Json::Value json_object;

    if (!reader.parse(mes->getData(), json_object))
    {
        LOG_ERROR("���ݽ���ʧ��:%s",mes->getData());
        return;
    }

	switch(json_object["MsgId"].asInt())
	{
	case IDD_MESSAGE_FRAME:                            // �����Ϣ
		{
			OnProcessFrameMes(connId,json_object);
		}
		break;
	case IDD_MESSAGE_GAME_LOGIN:                       // ��¼��Ϣ
		{
			OnProcessUserLoginMes(connId,json_object);
		}
		break;
	case IDD_MESSAGE_ROOM:                             // ������Ϣ
		{
			ServerRoomManager.OnProcessNetMes(connId,json_object);
		}
		break;
	default:
		break;
	}
}

/**
 * ���ڴ����������������Ϣ
 *
 * @param connId Ҫ����Ŀͻ��˵�����ID
 */
void GameFrameManager::OnProcessConnectedNetMes(uint32 connId)
{
    Json::Value root;
    root["MsgId"] = IDD_MESSAGE_CONNECT;
    root["MsgSubId"] = IDD_MESSAGE_CONNECT_SUCESS;

    Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
}

/**
 * ���ڴ������ڶϿ�����������Ϣ
 *
 * @param connId Ҫ����Ŀͻ��˵�����ID
 */
void GameFrameManager::OnProcessDisconnectNetMes(uint32 connId)
{
    DelQueueList(connId);
	ServerRoomManager.OnProcessDisconnectNetMes(connId);
}

CPlayer* GameFrameManager::CreateNewPlayer(void)
{
	return new CPlayer(PLAYERTYPE_ROBOT);
}

/// ���������Ϣ
void GameFrameManager::UpdatePlayerInfo(Player *pPlayer)
{
	if(pPlayer == NULL) return;

	Json::Value root;
    root["MsgId"] = IDD_MESSAGE_FRAME;
    root["MsgSubId"] = IDD_MESSAGE_UPDATE_USER_DATA;
    root["ID"] = pPlayer->GetID();
    root["State"] = pPlayer->GetState();
    root["Type"] = pPlayer->GetType();
    root["RoomId"] = pPlayer->GetRoomId();
    root["ChairIndex"] = pPlayer->GetChairIndex();
    root["Name"] = pPlayer->GetName();
    root["Level"] = pPlayer->GetLevel();
    root["Money"] = (uint32)pPlayer->GetMoney();
    root["BankMoney"] = (uint32)pPlayer->GetBankMoney();
    root["Revenue"] = (uint32)pPlayer->GetRevenue();
    root["TotalResult"] = (uint32)pPlayer->GetTotalResult();
    root["Experience"] = pPlayer->GetExperience();
    root["TotalBureau"] = pPlayer->GetTotalBureau();
    root["SuccessBureau"] = pPlayer->GetSuccessBureau();
    root["FailBureau"] = pPlayer->GetFailBureau();
    root["RunawayBureau"] = pPlayer->GetRunawayBureau();
    root["SuccessRate"] = pPlayer->GetSuccessRate();
    root["Runawayrate"] = pPlayer->GetRunawayrate();

	ServerPlayerManager.SendMsgToEveryone(root);
}

/**
 * �����û���¼ϵͳ��Ϣ
 *
 * @param connId Ҫ����Ŀͻ���
 * @param mes Ҫ����Ŀͻ��˵���Ϣ
 */
void GameFrameManager::OnProcessUserLoginMes(uint32 connId,Json::Value &mes)
{
	std::string pUserName = mes["UserName"].asString();
	std::string pUserPW = mes["UserPW"].asString();
	int pDeviceType = mes["DeviceType"].asInt();

	// ������������⣬�Ͳ��ܽ�����Ϸ��
	tagGameRoom ptagGameRoom;
    if(ServerDBOperator.GetGameRoomInfo(m_ServerSet.RoomId,&ptagGameRoom) && ptagGameRoom.gstate == 1)
    {
        Json::Value root;
        root["MsgId"] = IDD_MESSAGE_GAME_LOGIN;
        root["MsgSubId"] = IDD_MESSAGE_GAME_LOGIN_BANLOGIN;
        Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
        return;
    }

	uint32 pUserId = ServerDBOperator.IsExistUser(pUserName.c_str(),pUserPW.c_str());

	if(pUserId <= 0)
	{
        Json::Value root;
        root["MsgId"] = IDD_MESSAGE_GAME_LOGIN;
        root["MsgSubId"] = IDD_MESSAGE_GAME_LOGIN_FAIL;
        Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
		return;
	}

	//��鵱ǰ�����Ƿ񳬹�ָ����������
	if(ServerPlayerManager.GetPlayerCount() >= m_ServerSet.TableCount * m_ServerSet.PlayerCount)
	{
        Json::Value root;
        root["MsgId"] = IDD_MESSAGE_GAME_LOGIN;
        root["MsgSubId"] = IDD_MESSAGE_GAME_LOGIN_FULL;
        Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
		return;
	}

	UserDataStru pUserData;
	if(!ServerDBOperator.GetUserData(pUserId,pUserData))
	{
        Json::Value root;
        root["MsgId"] = IDD_MESSAGE_GAME_LOGIN;
        root["MsgSubId"] = IDD_MESSAGE_GAME_LOGIN_FAIL;
        Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
		return;
	}

	// �������˺ű���Ҳ���ܽ�����Ϸ
	if(pUserData.genable != 1)
    {
        Json::Value root;
        root["MsgId"] = IDD_MESSAGE_GAME_LOGIN;
        root["MsgSubId"] = IDD_MESSAGE_GAME_LOGIN_USERBANLOGIN;
        Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
        return;
    }

	// ���Ȳ��ҵ�ǰ����б����Ƿ��������û����������,�����û����ڵ���״̬�£��ʹ����û��ĵ�������
	CPlayer *pPlayer = ServerPlayerManager.GetLostPlayer(pUserId);
	if(pPlayer)
	{
		// �����豸����
		pPlayer->SetDeviceType((PlayerDeviceType)pDeviceType);

		if(pPlayer->GetState() == PLAYERSTATE_LOSTLINE)
		{
			pPlayer->SetConnectID(connId);

			// �ȴӵ����б������������
			ServerPlayerManager.DeleteLostPlayer(pPlayer->GetID());

			// Ȼ�����¼��뵽����б���
			ServerPlayerManager.Reset(pPlayer);

			// ����ҷ��ͳɹ���¼��������Ϣ
			SendPlayerLoginSuccess(pPlayer);
			return;
		}
	}

	pPlayer = ServerPlayerManager.GetPlayerById(pUserId);
	if(pPlayer)
	{
		// �����豸����
		pPlayer->SetDeviceType((PlayerDeviceType)pDeviceType);

		if(pPlayer->GetState() != PLAYERSTATE_LOSTLINE)
		{
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_GAME_LOGIN;
            root["MsgSubId"] = IDD_MESSAGE_GAME_LOGIN_EXIST;
            root["ServerPort"] = m_ServerSet.m_iServerPort;
            root["GameType"] = m_ServerSet.m_GameId;
            root["GameState"] = 1;
            Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
			return;
		}
	}
	else
	{
		// ����û��Ƿ�����һ����Ϸ��������
		uint32 pserverid,pgametype;
		int32 proomid,pchairid;
		pserverid=proomid=pchairid=pgametype=0;
		if(ServerDBOperator.IsExistUserGaming(pUserId,&pserverid,&proomid,&pchairid,&pgametype) &&
			(pserverid > 0 || proomid > -1 || pchairid > -1 || pgametype > 0))
		{
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_GAME_LOGIN;
            root["MsgSubId"] = IDD_MESSAGE_GAME_LOGIN_EXIST;
            root["ServerPort"] = pserverid;
            root["GameType"] = pgametype;
            root["GameState"] = 2;
            Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
			return;
		}

		CPlayer *pPlayer = new CPlayer(pUserId,connId);
		pPlayer->SetName(pUserName.c_str());
		pPlayer->SetMoney(pUserData.Money);
		pPlayer->SetBankMoney(pUserData.BankMoney);
		pPlayer->SetRevenue(pUserData.Revenue);
		pPlayer->SetTotalResult(pUserData.TotalResult);
		pPlayer->SetLevel(pUserData.Level);
		pPlayer->SetExperience(pUserData.Experience);
//		pPlayer->SetUserAvatar(pUserData.UserAvatar);
		pPlayer->SetTotalBureau(pUserData.TotalBureau);
		pPlayer->SetSuccessBureau(pUserData.SBureau);
		pPlayer->SetFailBureau(pUserData.FailBureau);
		pPlayer->SetRunawayBureau(pUserData.RunawayBureau);
		pPlayer->SetSuccessRate(pUserData.SuccessRate);
		pPlayer->SetRunawayrate(pUserData.RunawayRate);
		pPlayer->SetSex(pUserData.sex);
		pPlayer->SetSysType(pUserData.gType);
		pPlayer->SetRealName(pUserData.realName);
		pPlayer->SetLoginIP((uint32)inet_addr(pUserData.UserIP));
		pPlayer->SetDeviceType((PlayerDeviceType)pDeviceType);

		// �����¼�ɹ�������Ҽ��뵽��������
		if(ServerPlayerManager.AddPlayer(pPlayer))
		{
			SendPlayerLoginSuccess(pPlayer);
		}
		else
		{
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_GAME_LOGIN;
            root["MsgSubId"] = IDD_MESSAGE_GAME_LOGIN_FAIL;
            Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
		}
	}
}

/// ����ָ����ҵ�½�ɹ�����Ϣ
void GameFrameManager::SendPlayerLoginSuccess(CPlayer *pPlayer)
{
	if(pPlayer == NULL) return;

	pPlayer->setCurGameID(m_ServerSet.m_GameId);
	pPlayer->setCurServerId(m_ServerSet.RoomId);
	ServerDBOperator.SetPlayerGameState(pPlayer);

	// ����ҷ��ͳɹ���¼��������Ϣ
    Json::Value root;
    root["MsgId"] = IDD_MESSAGE_GAME_LOGIN;
    root["MsgSubId"] = IDD_MESSAGE_GAME_LOGIN_SUCESS;
    root["ID"] = pPlayer->GetID();
    root["State"] = pPlayer->GetState();
    root["Type"] = pPlayer->GetType();
    root["RoomId"] = pPlayer->GetRoomId();
    root["ChairIndex"] = pPlayer->GetChairIndex();
    root["Name"] = pPlayer->GetName();
    //root["UserAvatar"] = pPlayer->GetUserAvatar();
    root["Level"] = pPlayer->GetLevel();
    root["Money"] = (uint32)pPlayer->GetMoney();
    root["BankMoney"] = (uint32)pPlayer->GetBankMoney();
    root["Revenue"] = (uint32)pPlayer->GetRevenue();
    root["TotalResult"] = (uint32)pPlayer->GetTotalResult();
    root["Experience"] = pPlayer->GetExperience();
    root["TotalBureau"] = pPlayer->GetTotalBureau();
    root["SuccessBureau"] = pPlayer->GetSuccessBureau();
    root["FailBureau"] = pPlayer->GetFailBureau();
    root["SuccessBureau"] = pPlayer->GetSuccessBureau();
    root["SuccessRate"] = pPlayer->GetSuccessRate();
    root["Runawayrate"] = pPlayer->GetRunawayrate();
    root["Sex"] = pPlayer->GetSex();
    root["RealName"] = pPlayer->GetRealName();
    //root["LoginIP"] = pPlayer->GetLoginIP();
    root["DeviceType"] = pPlayer->GetDeviceType();
    root["CurGameID"] = pPlayer->getCurGameID();
    root["CurServerId"] = pPlayer->getCurServerId();

    in_addr inaddr;
    inaddr.s_addr=pPlayer->GetLoginIP();
    root["LoginIP"] = inet_ntoa(inaddr);

	ServerPlayerManager.SendMsgToEveryone(root);

	//���·�������������
	UpdateGameRoomInfo();
}

/**
 * �ȴ�����Ϸ�����Ϣ
 *
 * @param connId Ҫ����Ŀͻ���
 * @param mes Ҫ����Ŀͻ��˵���Ϣ
 */
void GameFrameManager::OnProcessFrameMes(uint32 connId,Json::Value &mes)
{
	switch(mes["MsgSubId"].asInt())
	{
	case IDD_MESSAGE_LOOKON_ENTER_ROOM:              // �Թ۽�����Ϸ����
		{
			CPlayer *pPlayer = ServerPlayerManager.GetNewPlayer(connId);
			if(pPlayer == NULL) return;

			// ����û��Ƿ�����һ����Ϸ��������
			uint32 pserverid,pgametype;
			int32 proomid,pchairid;
			pserverid=proomid=pchairid=pgametype=0;
			if(ServerDBOperator.IsExistUserGaming(pPlayer->GetID(),&pserverid,&proomid,&pchairid,&pgametype) &&
				(m_ServerSet.RoomId != pserverid || m_ServerSet.m_GameId != pgametype))
			{
				Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
                root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_EXIST;
                root["serverid"] = pserverid;
                root["gametype"] = pgametype;
                Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());
				return;
			}

			int pRoomIndex = mes["RoomIndex"].asInt();
			int pChairIndex = mes["ChairIndex"].asInt();

			if(pRoomIndex >= 0 && pChairIndex >= 0)
			{
				if(ServerRoomManager.AddLookOnPlayer(pPlayer,pRoomIndex,pChairIndex))
				{
					// ��������������е��������ͨ����ҽ�����Ϸ�������Ϣ
                    Json::Value root;
                    root["MsgId"] = IDD_MESSAGE_FRAME;
                    root["MsgSubId"] = IDD_MESSAGE_LOOKON_ENTER_ROOM;
                    root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_SUCC;
                    root["ID"] = pPlayer->GetID();
                    root["RoomId"] = pPlayer->GetRoomId();
                    root["ChairIndex"] = pPlayer->GetChairIndex();

					ServerPlayerManager.SendMsgToEveryone(root);

					CRoom *pRoom = ServerRoomManager.GetRoomById(pPlayer->GetRoomId());
					if(pRoom) pRoom->OnProcessEnterRoomMsg(pPlayer->GetChairIndex());

					return;
				}
			}

			// ��ǰ��ҷ��ͽ�����Ϸ����ʧ�ܵ���Ϣ
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_FRAME;
            root["MsgSubId"] = IDD_MESSAGE_LOOKON_ENTER_ROOM;
            root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_FAIL;
            root["ID"] = pPlayer->GetID();
            Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());
		}
		break;
	case IDD_MESSAGE_ENTER_ROOM:                     // ������Ϸ����
		{
			CPlayer *pPlayer = ServerPlayerManager.GetNewPlayer(connId);
			if(pPlayer == NULL)
            {
                //����������µ�¼����ң�����ǲ��ǵ��ߵ����
                OnProcessReEnterRoomMes(connId);

                return;
            }

			// �������Ƿ��Ѿ��ڷ������ˣ�����ڷ����У��ǲ��ܽ��뷿���
			CRoom *pRoom = ServerRoomManager.GetRoomById(pPlayer->GetRoomId());
			if(pRoom != NULL)
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
                root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_EXIST;
                root["serverid"] = 0;
                root["GameType"] = m_ServerSet.m_GameId;
                Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());

                return;
			}

			int pRoomIndex = mes["RoomIndex"].asInt();
			int pChairIndex = mes["ChairIndex"].asInt();
			std::string pEnterPWd = mes["EnterPWd"].asString();
			int64 pEnterfirst = mes["Enterfirst"].asInt();
			int64 pEntersecond = mes["Entersecond"].asInt();

			pPlayer->SetEnterPassword(pEnterPWd);
			pPlayer->SetEnterMoneyRect(pEnterfirst,pEntersecond);

			if(pRoomIndex < 0 || pRoomIndex >= 65535) pRoomIndex = -1;
			if(pChairIndex < 0 || pChairIndex >= 65535) pChairIndex = -1;

			// ����û��Ƿ�����һ����Ϸ��������
			uint32 pserverid,pgametype;
			int32 proomid,pchairid;
			pserverid=proomid=pchairid=pgametype=0;
			if(ServerDBOperator.IsExistUserGaming(pPlayer->GetID(),&pserverid,&proomid,&pchairid,&pgametype) &&
				(m_ServerSet.RoomId != pserverid || m_ServerSet.m_GameId != pgametype))
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
                root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_EXIST;
                root["serverid"] = pserverid;
                root["GameType"] = pgametype;
                Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());
				return;
			}

			// �����û����ϵ�Ǯ����������
			OnProcessUserInfo(pPlayer);

			// ����û��Ľ���Ƿ����㷿��Ҫ��
			if(pPlayer->GetMoney() < m_ServerSet.lastMoney ||
				pPlayer->GetMoney() < m_ServerSet.m_Pielement * m_g_ServerServiceManager->GetRoomLastDouble())
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
                root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_LASTMONEY;
                Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());
				return;
			}

			if(m_ServerSet.m_QueueGaming)
				AddQueueList(pPlayer);                // �Ŷӽ���
			else
                JoinPlayerToGameRoom(pPlayer,pRoomIndex,pChairIndex,m_ServerSet.m_QueueGaming);
		}
		break;
	case IDD_MESSAGE_CHANGER_ROOM:                  // ������Ϸ����
		{
			CPlayer *pPlayer = ServerPlayerManager.GetPlayer(connId);
			if(pPlayer == NULL) return;

			// ���Ƚű��������û��뿪��Ϸ����
			CRoom *pRoom = ServerRoomManager.GetRoomById(pPlayer->GetRoomId());
			if(pRoom == NULL) return;

			// ����������Ϸ���ǲ��ܻ�����
			if(pPlayer->GetState() == PLAYERSTATE_GAMING)
				return;

			// ����û������Թ��û�����ô���Ƚ�����Ϸ
			if(!pPlayer->IsLookOn())
			{
				pRoom->OnProcessLeaveRoomMsg(pPlayer->GetChairIndex());
			}

			// ��ǰ�������������û���������뿪������Ϣ
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_FRAME;
            root["MsgSubId"] = IDD_MESSAGE_LEAVE_ROOM;
            root["UserID"] = pPlayer->GetID();

			ServerPlayerManager.SendMsgToEveryone(root);

			// �ӷ������������û�
			pRoom->ClearPlayer(pPlayer);

			// ������ҵ�״̬Ϊ�뿪����
			pPlayer->SetRoomId(-1);
			pPlayer->SetChairIndex(-1);

			// ׼�����˵��û��Ƿ������ʼ��Ϸ
			pRoom->OnProcessContinueGaming();

			// ����û��Ƿ�����һ����Ϸ��������
			uint32 pserverid,pgametype;
			int32 proomid,pchairid;

			pserverid=proomid=pchairid=pgametype=0;
			if(ServerDBOperator.IsExistUserGaming(pPlayer->GetID(),&pserverid,&proomid,&pchairid,&pgametype) &&
				(m_ServerSet.RoomId != pserverid || m_ServerSet.m_GameId != pgametype))
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
                root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_EXIST;
                root["serverid"] = pserverid;
                root["GameType"] = pgametype;
                Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());
				return;
			}

			// �����û����ϵ�Ǯ����������
			OnProcessUserInfo(pPlayer);

			// ����û��Ľ���Ƿ����㷿��Ҫ��
			if(pPlayer->GetMoney() < m_ServerSet.lastMoney ||
				pPlayer->GetMoney() < m_ServerSet.m_Pielement * m_g_ServerServiceManager->GetRoomLastDouble())
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
                root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_LASTMONEY;
                Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());
				return;
			}

			if(m_ServerSet.m_QueueGaming)
				AddQueueList(pPlayer);                // �Ŷӽ���
			else
                JoinPlayerToGameRoom(pPlayer,-1,-1,m_ServerSet.m_QueueGaming);
		}
		break;
	case IDD_MESSAGE_USER_CHAT:                     // �û�����
		{
			int senduserID = mes["senduserID"].asInt();
			int receiverID = mes["receiverID"].asInt();
			std::string ChatMsg = mes["ChatMsg"].asString();

            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_FRAME;
            root["MsgSubId"] = IDD_MESSAGE_USER_CHAT;
            root["senduserID"] = senduserID;
            root["receiverID"] = receiverID;
            root["ChatMsg"] = ChatMsg;

			if(receiverID == -1)
				ServerPlayerManager.SendMsgToEveryone(root);
			else
			{
				ServerPlayerManager.SendMsgToPlayer(senduserID,root);
				ServerPlayerManager.SendMsgToPlayer(receiverID,root);
			}
		}
		break;
	case IDD_MESSAGE_UPDATE_USER_MONEY:             // �����û����ϵ�Ǯ
		{
		    uint32 pUserId = mes["UserId"].asInt();

			CPlayer *pPlayer = ServerPlayerManager.GetPlayerById(pUserId);
			if(pPlayer == NULL) return;

			// �����û�������
			OnProcessUserInfo(pPlayer);
		}
		break;
	case IDD_MESSAGE_LEAVE_ROOM:                    // �뿪��Ϸ����
		{
			// �����û��뿪����
			OnProcessGameLeaveRoomMes(connId,mes);

			DelQueueList(connId);
		}
		break;
	case IDD_MESSAGE_GET_ONLINE_PLAYERS:             // �õ��������
		{
			ServerPlayerManager.LockPlayerList();

			int pPlayerCount = 0;
			std::vector<std::vector<CPlayer*> > pPlayerList;
			std::vector<CPlayer*> pNewPlayers;

			std::map<uint32,CPlayer*>::iterator iter = ServerPlayerManager.GetPlayerList().begin();
			for(;iter != ServerPlayerManager.GetPlayerList().end();++iter)
			{
				if((*iter).second == NULL) continue;

				pPlayerCount+=1;
				pNewPlayers.push_back((*iter).second);

				if(pPlayerCount >= IDD_SEND_PLAYER_MAX_COUNT)
				{
					pPlayerList.push_back(pNewPlayers);
					pPlayerCount=0;
					pNewPlayers.clear();
				}
			}

			iter = ServerPlayerManager.GetRobotList().begin();
			for(;iter != ServerPlayerManager.GetRobotList().end();++iter)
			{
				if((*iter).second == NULL) continue;

				pPlayerCount+=1;
				pNewPlayers.push_back((*iter).second);

				if(pPlayerCount >= IDD_SEND_PLAYER_MAX_COUNT)
				{
					pPlayerList.push_back(pNewPlayers);
					pPlayerCount=0;
					pNewPlayers.clear();
				}
			}

			iter = ServerPlayerManager.GetLostPlayerList().begin();
			for(;iter != ServerPlayerManager.GetLostPlayerList().end();++iter)
			{
				if((*iter).second == NULL) continue;

				pPlayerCount+=1;
				pNewPlayers.push_back((*iter).second);

				if(pPlayerCount >= IDD_SEND_PLAYER_MAX_COUNT)
				{
					pPlayerList.push_back(pNewPlayers);
					pPlayerCount=0;
					pNewPlayers.clear();
				}
			}

			if(pPlayerCount > 0 && !pNewPlayers.empty())
				pPlayerList.push_back(pNewPlayers);

			for(int i=0;i<(int)pPlayerList.size();i++)
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_GET_ONLINE_PLAYERS;
                root["PlayerCount"] = (int)pPlayerList[i].size();

				for(int k=0;k<(int)pPlayerList[i].size();k++)
				{
				    Json::Value root2;
					root2["ID"] = pPlayerList[i][k]->GetID();
					root2["State"] = pPlayerList[i][k]->GetState();
					root2["Type"] = pPlayerList[i][k]->GetType();
					root2["RoomId"] = pPlayerList[i][k]->GetRoomId();
					root2["ChairIndex"] = pPlayerList[i][k]->GetChairIndex();
					root2["Name"] = pPlayerList[i][k]->GetName();
					root2["Level"] = pPlayerList[i][k]->GetLevel();
					root2["Money"] = (uint32)pPlayerList[i][k]->GetMoney();
					root2["BankMoney"] = (uint32)pPlayerList[i][k]->GetBankMoney();
					root2["Revenue"] = (uint32)pPlayerList[i][k]->GetRevenue();
					root2["TotalResult"] = (uint32)pPlayerList[i][k]->GetTotalResult();
					root2["Experience"] = pPlayerList[i][k]->GetExperience();
					root2["TotalBureau"] = pPlayerList[i][k]->GetTotalBureau();
					root2["SuccessBureau"] = pPlayerList[i][k]->GetSuccessBureau();
					root2["FailBureau"] = pPlayerList[i][k]->GetFailBureau();
					root2["RunawayBureau"] = pPlayerList[i][k]->GetRunawayBureau();
					root2["SuccessRate"] = pPlayerList[i][k]->GetSuccessRate();
					root2["Runawayrate"] = pPlayerList[i][k]->GetRunawayrate();
					root2["Sex"] = pPlayerList[i][k]->GetSex();
					root2["RealName"] = pPlayerList[i][k]->GetRealName();
					//root2["LoginIP"] = pPlayerList[i][k]->GetLoginIP();
					root2["DeviceType"] = pPlayerList[i][k]->GetDeviceType();

                    in_addr inaddr;
                    inaddr.s_addr=pPlayerList[i][k]->GetLoginIP();
                    root2["LoginIP"] = inet_ntoa(inaddr);

                    root["player"].append(root2);
				}

				Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
			}

            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_FRAME;
            root["MsgSubId"] = IDD_MESSAGE_GET_ONLINE_PLAYERS_END;
            Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());

			ServerPlayerManager.UnlockPlayerList();
		}
		break;
	case IDD_MESSAGE_GET_ROOM_LIST:                  // �õ���ǰ�������з����б�
		{
			ServerRoomManager.LockRoomList();

			std::vector<CRoom*> pRoomList = ServerRoomManager.GetRoomList();
			if(pRoomList.empty())
			{
				ServerRoomManager.UnlockRoomList();
				return;
			}

            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_FRAME;
            root["MsgSubId"] = IDD_MESSAGE_GET_ROOM_LIST;

			// ����д�������з��������
			root["RoomCount"] = (int)pRoomList.size();
			root["GameName"] = m_ServerSet.GameName;
			root["GameType"] = m_ServerSet.GameType;
			root["PlayerCount"] = m_ServerSet.PlayerCount;

			std::vector<CRoom*>::iterator iter = pRoomList.begin();
			for(int index=0;iter != pRoomList.end();iter++,index++)
			{
				if((*iter) == NULL) continue;

				int64 pfirst,psecond;
				pfirst=psecond = 0;
				(*iter)->GetEnterMoneyRect(&pfirst,&psecond);

				Json::Value root2;
				// ���ͷ���״̬
				root2["RoomState"] = (int)(*iter)->GetRoomState();
				root2["EnterPassword"] = (*iter)->getEnterPassword();
				root2["first"] = (uint32)pfirst;
				root2["second"] = (uint32)psecond;
				root2["PlayerCount"] = (*iter)->GetPlayerCount();

				// ��ʼ���ͷ����������Ϣ
				for(int i=0;i<(*iter)->GetMaxPlayer();i++)
				{
					Player *pPlayer = (*iter)->GetPlayer(i);
					if(pPlayer == NULL) continue;

					Json::Value root3;
					root3["ID"] = pPlayer->GetID();
					root3["ChairIndex"] = pPlayer->GetChairIndex();

                    root2["player"].append(root3);
				}

				root["room"].append(root2);
			}

			Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());

			ServerRoomManager.UnlockRoomList();
		}
		break;
	case IDD_MESSAGE_READY_START:                     // ���׼����ʼ��Ϸ
		{
			OnProcessGameReadyMes(connId,mes);
		}
		break;
	case IDD_MESSAGE_REENTER_ROOM:                    // ���»ص���Ϸ
		{
            OnProcessReEnterRoomMes(connId);
		}
		break;
	default:
		break;
	}
}

/// ��Ҷ����ػط���
bool GameFrameManager::OnProcessReEnterRoomMes(uint32 connId)
{
    CPlayer *pPlayer = ServerPlayerManager.GetPlayer(connId);
    if(pPlayer == NULL) return false;

    CRoom *pRoom = ServerRoomManager.GetRoomById(pPlayer->GetRoomId());
    if(pRoom == NULL || pRoom->GetRoomState() != ROOMSTATE_GAMING)
    {
        // ��ǰ��ҷ��ͽ�����Ϸ����ʧ�ܵ���Ϣ
        Json::Value root;
        root["MsgId"] = IDD_MESSAGE_FRAME;
        root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
        root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_FAIL;
        root["ID"] = pPlayer->GetID();
        Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());

        return false;
    }

    Json::Value root;
    root["MsgId"] = IDD_MESSAGE_FRAME;
    root["MsgSubId"] = IDD_MESSAGE_REENTER_ROOM;
    root["ID"] = pPlayer->GetID();
    ServerPlayerManager.SendMsgToEveryone(root);

    pPlayer->SetState(PLAYERSTATE_GAMING);

    //�������״̬
    ServerDBOperator.SetPlayerGameState(pPlayer);

    pRoom->OnProcessReEnterRoomMes(pPlayer->GetChairIndex());

    return true;
}

/// ���������Ϣ
void GameFrameManager::OnProcessUserInfo(CPlayer *pPlayer)
{
	if(pPlayer == NULL) return;

	UserDataStru pUserData;
	if(ServerDBOperator.GetUserData(pPlayer->GetID(),pUserData))
	{
		bool isSendUpdateMsg = false;

		if(pPlayer->GetMoney() != pUserData.Money || pPlayer->GetBankMoney() != pUserData.BankMoney ||
			pPlayer->GetSex() != pUserData.sex || pPlayer->GetRealName() != pUserData.realName)
			isSendUpdateMsg = true;

		pPlayer->SetMoney(pUserData.Money);
		pPlayer->SetBankMoney(pUserData.BankMoney);
		pPlayer->SetSex(pUserData.sex);
		pPlayer->SetRealName(pUserData.realName);
		pPlayer->SetLoginIP((uint32)inet_addr(pUserData.UserIP));

		if(isSendUpdateMsg)
		{
			UpdatePlayerMoney(pPlayer);
		}
	}
}

/// ����������ϵ�Ǯ
void GameFrameManager::UpdatePlayerMoney(Player *pPlayer)
{
	if(pPlayer == NULL) return;

    Json::Value root;
    root["MsgId"] = IDD_MESSAGE_FRAME;
    root["MsgSubId"] = IDD_MESSAGE_UPDATE_USER_MONEY;
    root["ID"] = pPlayer->GetID();
    root["Money"] = (uint32)pPlayer->GetMoney();
    root["BankMoney"] = (uint32)pPlayer->GetBankMoney();
    root["Sex"] = pPlayer->GetSex();
    root["RealName"] = pPlayer->GetRealName();

	ServerPlayerManager.SendMsgToEveryone(root);
}

/// ���ڴ����û�׼����Ϣ
void GameFrameManager::OnProcessGameReadyMes(uint32 connId,Json::Value &mes)
{
	CPlayer *pPlayer = ServerPlayerManager.GetPlayer(connId);
	if(pPlayer == NULL) return;

	CRoom *pRoom = ServerRoomManager.GetRoomById(pPlayer->GetRoomId());
	if(pRoom == NULL) return;

	// �������û�����������ֻ������ͨ�û�
	if(pPlayer->GetState() != PLAYERSTATE_NORAML)
		return;

	// ����������Ϸ��Ҳ������
	if(pRoom->GetRoomState() == ROOMSTATE_GAMING)
		return;

    Json::Value root;
    root["MsgId"] = IDD_MESSAGE_FRAME;
    root["MsgSubId"] = IDD_MESSAGE_READY_START;
    root["ID"] = pPlayer->GetID();
	ServerPlayerManager.SendMsgToEveryone(root);

	pPlayer->SetState(PLAYERSTATE_READY);
	pPlayer->SetReadyTime((DWORD)time(NULL));

    //�������״̬
    ServerDBOperator.SetPlayerGameState(pPlayer);

	if(pRoom->GetMaster() < 0)
		pRoom->SetMaster(pPlayer->GetChairIndex());

	pRoom->OnProcessPlayerReadyMes(pPlayer->GetChairIndex());
}

/// ���ڴ����û��뿪��Ϣ
void GameFrameManager::OnProcessGameLeaveRoomMes(uint32 connId,Json::Value &mes)
{
	CPlayer *pPlayer = ServerPlayerManager.GetPlayer(connId);
	if(pPlayer == NULL) return;

	CRoom *pRoom = ServerRoomManager.GetRoomById(pPlayer->GetRoomId());
	if(pRoom == NULL) return;

	if(!pPlayer->IsLookOn())
	{
		pRoom->OnProcessLeaveRoomMsg(pPlayer->GetChairIndex());

		//pPlayer->SetState(PLAYERSTATE_NORAML);
	}

    Json::Value root;
    root["MsgId"] = IDD_MESSAGE_FRAME;
    root["MsgSubId"] = IDD_MESSAGE_LEAVE_ROOM;
    root["ID"] = pPlayer->GetID();

	ServerPlayerManager.SendMsgToEveryone(root);

	pRoom->ClearPlayer(pPlayer);

	pPlayer->SetRoomId(-1);
	pPlayer->SetChairIndex(-1);
}

bool GameFrameManager::JoinPlayerToGameRoom(CPlayer *pPlayer,int pRoomIndex,int pChairIndex,bool isQueue)
{
	if(pPlayer == NULL)
		return false;

	if(AddPlayerInServer(pPlayer,pRoomIndex,pChairIndex,isQueue,true)) return true;

	if(!isQueue)
	{
		if(m_ServerSet.m_GameStartMode == enStartMode_FullReady ||
			m_ServerSet.m_GameStartMode == enStartMode_TimeControl)
		{
			if(!AddPlayerInServer(pPlayer,pRoomIndex,pChairIndex,isQueue,false))
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;

				if(!pPlayer->getEnterPassword().empty())
					root["MsgSubId2"] = IDD_MESSAGE_PERSONROOM_ENTER_ROOM_FAIL;
				else
					root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_FAIL;

				root["ID"] = pPlayer->GetID();

				Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());
			}
		}
		else
		{
			CRoom *pRoom = ServerRoomManager.GetFreeRoom();
			if(pRoom != NULL)
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
                root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_FAIL;
                root["ID"] = pPlayer->GetID();

				Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());
			}
			else
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
                root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_FULL;
                root["ID"] = pPlayer->GetID();

				Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());
			}
		}
	}

	pPlayer->SetEnterMoneyRect(0,0);
	pPlayer->SetEnterPassword("");

	return false;
}

bool GameFrameManager::AddPlayerInServer(CPlayer *pPlayer,int pRoomIndex,int pChairIndex,bool isQueue,bool isGaming)
{
	if(ServerRoomManager.AddPlayer(pPlayer,pRoomIndex,pChairIndex,isQueue,isGaming))
	{
        Json::Value root;
        root["MsgId"] = IDD_MESSAGE_FRAME;
        root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
        root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_SUCC;
        root["ID"] = pPlayer->GetID();
        root["RoomId"] = pPlayer->GetRoomId();
        root["ChairIndex"] = pPlayer->GetChairIndex();

		ServerPlayerManager.SendMsgToEveryone(root);

		CRoom *pRoom = ServerRoomManager.GetRoomById(pPlayer->GetRoomId());
		if(pRoom)
		{
			pRoom->OnProcessEnterRoomMsg(pPlayer->GetChairIndex());
		}

		pPlayer->SetEnterMoneyRect(0,0);
		pPlayer->SetEnterPassword("");

		return true;
	}

	return false;
}

/// ���·�����Ϣ
void GameFrameManager::UpdateGameRoomInfo(void)
{
    ServerDBOperator.UpdateGameRoomOnlinePlayerCount(m_ServerSet.RoomId,
                                                     ServerPlayerManager.GetPlayerCount(),
                                                     ServerPlayerManager.GetRobotCount());
}

/// �õ���ǰ�Ŷ�����
int GameFrameManager::GetQueueRealPlayerCount(void)
{
    int count = 0;

    m_PlayerQueueListLock.Acquire();
    std::map<uint32,CPlayer*>::iterator iter = m_PlayerQueueList.begin();
    for(;iter != m_PlayerQueueList.end();++iter)
    {
        if((*iter).second != NULL && (*iter).second->GetType() == PLAYERTYPE_NORMAL) count+=1;
    }
    m_PlayerQueueListLock.Release();

    return count;
}
/// �õ���ǰ�Ŷӻ�������
int GameFrameManager::GetQueueRobotPlayerCount(void)
{
    int count = 0;

    m_PlayerQueueListLock.Acquire();
    std::map<uint32,CPlayer*>::iterator iter = m_PlayerQueueList.begin();
    for(;iter != m_PlayerQueueList.end();++iter)
    {
        if((*iter).second != NULL && (*iter).second->GetType() == PLAYERTYPE_ROBOT) count+=1;
    }
    m_PlayerQueueListLock.Release();

    return count;
}
/// ����Ŷ��б�
void GameFrameManager::ClearQueueList(void)
{
    m_PlayerQueueListLock.Acquire();
    std::map<uint32,CPlayer*>::iterator iter = m_PlayerQueueList.begin();
    for(;iter != m_PlayerQueueList.end();++iter)
    {
        if((*iter).second == NULL) continue;

        (*iter).second->SetState(PLAYERSTATE_NORAML);
    }
    m_PlayerQueueList.clear();
    m_PlayerQueueListLock.Release();
}

/// �õ���ǰ�Ŷ�����
int GameFrameManager::GetCurrentQueuePlayerCount(void)
{
	int pWaitingCount = 0;

	m_PlayerQueueListLock.Acquire();
	pWaitingCount = (int)m_PlayerQueueList.size();
	m_PlayerQueueListLock.Release();

	return pWaitingCount;
}

/// ���͵�ǰ�Ŷ�����
void GameFrameManager::SendQueuingCount(void)
{
	// ��������������ҷ����Ŷ���Ϣ
	if(!m_ServerSet.m_QueueGaming/* || m_PlayerQueueList.empty()*/)
		return;

	int pWaitingCount = 0;

	m_PlayerQueueListLock.Acquire();
	pWaitingCount = (int)m_PlayerQueueList.size();
	m_PlayerQueueListLock.Release();

	//// ���������ͬ���Ͳ�������
	//if(m_oldWaitingCount == pWaitingCount)
	//	return;

    Json::Value root;
    root["MsgId"] = IDD_MESSAGE_FRAME;
    root["MsgSubId"] = IDD_MESSAGE_FRAME_MATCH;
    root["MsgSubId2"] = IDD_MESSAGE_FRAME_MATCH_WAITINGCOUNT;
    root["WaitingCount"] = pWaitingCount;

	ServerPlayerManager.SendMsgToEveryone(root);

	//m_oldWaitingCount = pWaitingCount;
}

/// ����һ����ҵ��Ŷ��б���
void GameFrameManager::AddQueueList(CPlayer *connId)
{
	if(connId == NULL) return;

	m_PlayerQueueListLock.Acquire();

	std::map<uint32,CPlayer*>::iterator iter = m_PlayerQueueList.find(connId->GetID());
	if(iter == m_PlayerQueueList.end())
	{
		connId->SetState(PLAYERSTATE_QUEUE);

		//�������״̬
		ServerDBOperator.SetPlayerGameState(connId);

		m_PlayerQueueList.insert(std::pair<uint32,CPlayer*>(connId->GetID(),connId));
	}

	m_PlayerQueueListLock.Release();
	//CString tmpStr;
	//tmpStr.Format("�����Ŷ�����:%d\n",(int)m_PlayerQueueList.size());
	//::OutputDebugString(tmpStr);

	// �ȸ��������Ŷ�����
	SendQueuingCount();

	// ����Ŷ������������Ҫ�󣬾����̿�ʼ����
	UpdateQueueList();
}

/// ɾ��һ����Ҵ��Ŷ��б���
void GameFrameManager::DelQueueList(uint32 connId)
{
	m_PlayerQueueListLock.Acquire();
	std::map<uint32,CPlayer*>::iterator iter = m_PlayerQueueList.begin();
	for(;iter != m_PlayerQueueList.end();)
	{
		if((*iter).second->GetConnectID() == connId)
		{
			// ֻ�����û������Ŷ�״̬ʱ������Ȩ�ı�����״̬
			if((*iter).second->GetState() == PLAYERSTATE_QUEUE)
			{
				(*iter).second->SetState(PLAYERSTATE_NORAML);
			}

            //�������״̬
            ServerDBOperator.SetPlayerGameState((*iter).second);

			iter = m_PlayerQueueList.erase(iter);

			break;
		}
		else
		{
			++iter;
		}
	}
	m_PlayerQueueListLock.Release();

	//CString tmpStr;
	//tmpStr.Format("�뿪�Ŷ�����:%d\n",(int)m_PlayerQueueList.size());
	//::OutputDebugString(tmpStr);

	// �ȸ��������Ŷ�����
	SendQueuingCount();
}

/// �����Ŷ�����б�
void GameFrameManager::UpdateQueueList(void)
{
	if((int)m_PlayerQueueList.size() < m_ServerSet.PlayerCount)
	{
		if(m_PlayerQueueList.empty()) return;

		if(!m_PlayerQueueList.empty())
		{
			m_PlayerQueueListLock.Acquire();
			std::map<uint32,CPlayer*>::iterator iter = m_PlayerQueueList.begin();
			for(;iter != m_PlayerQueueList.end();)
			{
				(*iter).second->SetState(PLAYERSTATE_NORAML);
				if(JoinPlayerToGameRoom((*iter).second) == false)
				{
					(*iter).second->SetState(PLAYERSTATE_QUEUE);
					break;
				}

				iter = m_PlayerQueueList.erase(iter);
			}
			m_PlayerQueueListLock.Release();
		}

		return;
	}

	CRoom *pRoom = ServerRoomManager.GetEmptyRoom();
	if(pRoom == NULL) return;

	m_PlayerQueueListLock.Acquire();
	int realCount = (int)m_PlayerQueueList.size();
	std::map<uint32,CPlayer*>::iterator iter = m_PlayerQueueList.begin();
	for(int index=0;iter != m_PlayerQueueList.end();index++)
	{
		if(index >= (realCount/m_ServerSet.PlayerCount) * m_ServerSet.PlayerCount)
			break;

		if(pRoom->IsFull())
			pRoom = ServerRoomManager.GetEmptyRoom();

		if(pRoom == NULL || pRoom->GetRoomState() == ROOMSTATE_GAMING) break;

		(*iter).second->SetRoomId(pRoom->GetID());
		(*iter).second->SetState(PLAYERSTATE_NORAML);

		int playerIndex = pRoom->AddPlayer((*iter).second);

		(*iter).second->SetChairIndex(playerIndex);

		// ��������������е��������ͨ����ҽ�����Ϸ�������Ϣ
        Json::Value root;
        root["MsgId"] = IDD_MESSAGE_FRAME;
        root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
        root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_SUCC;
        root["ID"] = (*iter).second->GetID();
        root["RoomId"] = (*iter).second->GetRoomId();
        root["ChairIndex"] = (*iter).second->GetChairIndex();

		ServerPlayerManager.SendMsgToEveryone(root);

		if(pRoom) pRoom->OnProcessEnterRoomMsg((*iter).second->GetChairIndex());

		iter = m_PlayerQueueList.erase(iter);
	}
	m_PlayerQueueListLock.Release();

	if(!m_PlayerQueueList.empty())
	{
		m_PlayerQueueListLock.Acquire();
		std::map<uint32,CPlayer*>::iterator iter = m_PlayerQueueList.begin();
		for(;iter != m_PlayerQueueList.end();)
		{
			(*iter).second->SetState(PLAYERSTATE_NORAML);
			if(JoinPlayerToGameRoom((*iter).second) == false)
			{
				(*iter).second->SetState(PLAYERSTATE_QUEUE);

                //�������״̬
                ServerDBOperator.SetPlayerGameState((*iter).second);

				break;
			}

            //�������״̬
            ServerDBOperator.SetPlayerGameState((*iter).second);

			iter = m_PlayerQueueList.erase(iter);
		}
		m_PlayerQueueListLock.Release();
	}
}
