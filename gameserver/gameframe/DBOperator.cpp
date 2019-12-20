#include "DBOperator.h"
#include "dataproviderfactory.h"
#include "CPlayer.h"

initialiseSingleton(DBOperator);

/**
 * ���캯��
 */
DBOperator::DBOperator()
: m_DataProvider(NULL)
{
	m_DataProvider = DataProviderFactory::createDataProvider();
}

/**
 * ��������
 */
DBOperator::~DBOperator()
{
	Shutdown();

	if(m_DataProvider)
		delete m_DataProvider;
	m_DataProvider = NULL;
}

/**
 * ��ʼ���ݿ�
 *
 * @param host Ҫ���ӵ����ݿ��IP��ַ
 * @param user �������ݿ���û���
 * @param pass �������ݿ���û�����
 * @param db Ҫ���ӵ����ݿ�����
 * @param port ���ݿ�˿ں�
 *
 * @return ������ݿ����ӳɹ������棬���򷵻ؼ�
 */
bool DBOperator::Initilize(std::string host,std::string user,std::string pass,std::string db,int port)
{
	if(m_DataProvider == NULL)
		return false;

	return m_DataProvider->connect(host,user,pass,db,port);
}

/**
 * �ر����ݿ�����
 */
void DBOperator::Shutdown(void)
{
	if(m_DataProvider == NULL)
		return;

	m_DataProvider->disconnect();
}

/// ����ά����ǰ���ݿ�����
void DBOperator::Update(void)
{
	if(m_DataProvider == NULL) return;

	m_DataProvider->Update();
}

/// �õ��������ò���
std::string DBOperator::GetRoomParamaters(uint32 KindID,uint32 ServerID)
{
	if(m_DataProvider == NULL || ServerID <= 0 || KindID <= 0) return "";

	std::ostringstream sqlstr;
	sqlstr << "select serverparameter from mol_gameroom where gameid=" << KindID << " and id=" << ServerID << ";";

	RecordSetList pRecord = m_DataProvider->execSql(sqlstr.str());
	if(pRecord.isEmpty()) return "";

	return pRecord(0)(0,0);
}

bool DBOperator::GetRobotsOfGameServer(uint32 KindID,uint32 ServerID,std::vector<uint32>& pUserList)
{
	if(m_DataProvider == NULL || ServerID <= 0 || KindID <= 0) return false;

	std::ostringstream sqlstr;
	sqlstr << "select userid from mol_androiduserinfo where kindid=" << KindID << " and serverid=" << ServerID << " and nullity=0;";

	RecordSetList pRecord = m_DataProvider->execSql(sqlstr.str());
	if(pRecord.isEmpty()) return false;

	for(int i=0;i<(int)pRecord(0).rows();i++)
	{
		uint32 pUserID = atoi(pRecord(0)(i,0).c_str());

		pUserList.push_back(pUserID);
	}

	return true;
}

/// ���õ�ǰ�����Ϸ״̬
bool DBOperator::SetPlayerGameState(CPlayer *pPlayer)
{
	if(m_DataProvider == NULL || pPlayer == NULL)
		return false;

	std::ostringstream sqlstr;
	sqlstr << "call updateplayergamestate("
		<< pPlayer->GetID()
		<< "," << pPlayer->getCurTableIndex()
		<< "," << pPlayer->getCurChairIndex()
		<< "," << pPlayer->getCurGameID()
		<< "," << pPlayer->getCurServerId()
		<< "," << (int)pPlayer->GetState()
		<< ");";

	m_DataProvider->execSql(sqlstr.str());

	return true;
}

bool DBOperator::InsertPlayerGameRecord(uint32 UserId,int64 Score,int64 Revenue,uint32 gameId,uint32 ServerID,
							std::string RoomName,int tableid,int chairid,int64 lastmoney,std::string gametip,int64 pAgentmoney,int64 pcurJetton)
{
	if(m_DataProvider == NULL || UserId <= 0/* || Score == 0*/) return false;

	std::ostringstream sqlstr;

	sqlstr << "call gameserver_insertgamerecord("
		   << UserId
		   << "," << Score
		   << "," << Revenue
		   << "," << gameId
		   << "," << ServerID
		   << ",'"<< RoomName
		   << "'," << tableid
		   << "," << chairid
		   << "," << lastmoney
		   << ",'" << gametip
		   << "'," << pAgentmoney
		   << "," << pcurJetton
		   << ");";

	m_DataProvider->execSql(sqlstr.str());

	return true;
}

bool DBOperator::getplayercontrolconfig(Player *pPlayer,int64 *curresult,int64 *decresult,int *iscontroluser)
{
	if(m_DataProvider == NULL || pPlayer == NULL)
		return false;

	std::ostringstream sqlstr;
	sqlstr << "select totalresult,dectotalresult,isenableusercontrol from mol_userdata where userid=" << pPlayer->GetID() << ";";

	RecordSetList pRecord = m_DataProvider->execSql(sqlstr.str());
	if(pRecord.isEmpty()) return false;

	*curresult = _atoi64(pRecord(0)(0,0).c_str());
	*decresult = _atoi64(pRecord(0)(0,1).c_str());
	*iscontroluser = atoi(pRecord(0)(0,2).c_str());

	return true;
}

/// ���������Ϣ
bool DBOperator::UpdateUserData(Player *pPlayer)
{
	if(m_DataProvider == NULL || pPlayer == NULL) return false;

	std::ostringstream sqlstr;
	sqlstr << "update mol_userdata set"
		   << " money=" << pPlayer->GetMoney()
		  // << ",bankmoney=" << pPlayer->GetBankMoney()
		   << ",revenue=" << pPlayer->GetRevenue()
		   << ",totalresult=" << pPlayer->GetTotalResult()
		   << ",level=" << pPlayer->GetLevel()
		   << ",experience=" << pPlayer->GetExperience()
		   << ",totalbureau=" << pPlayer->GetTotalBureau()
		   << ",sbureau=" << pPlayer->GetSuccessBureau()
		   << ",failbureau=" << pPlayer->GetFailBureau()
		   << ",runawaybureau=" << pPlayer->GetRunawayBureau()
		   << ",successrate=" << pPlayer->GetSuccessRate()
		   << ",runawayrate=" << pPlayer->GetRunawayrate()
		   << " where userid=" << pPlayer->GetID() << ";";

	m_DataProvider->execSql(sqlstr.str());

	return true;
}

/// ����ָ����ҵ����Ͻ������
bool DBOperator::UpdateUserMoney(CPlayer *pPlayer)
{
	if(m_DataProvider == NULL || pPlayer == NULL) return false;

	std::ostringstream sqlstr;
	sqlstr << "update mol_userdata set"
		<< " money=" << pPlayer->GetMoney()
		<< " where userid=" << pPlayer->GetID() << ";";

	m_DataProvider->execSql(sqlstr.str());

	return true;
}

/// ���ָ������Ƿ�����Ϸ��
bool DBOperator::IsExistUserGaming(uint32 UserId,uint32 *serverid,int32 *roomid,int32 *chairid,uint32 *gametype)
{
	if(UserId <= 0 || m_DataProvider == NULL)
		return false;

	std::ostringstream sqlstr;
	sqlstr << "select curtableindex,curchairindex,curgametype,curserverport from mol_userdata where userid=" << UserId << ";";

	RecordSetList pRecord = m_DataProvider->execSql(sqlstr.str());
	if(pRecord.isEmpty()) return false;

	*roomid = atoi(pRecord(0)(0,0).c_str());
	*chairid = atoi(pRecord(0)(0,1).c_str());
	*gametype = atoi(pRecord(0)(0,2).c_str());
	*serverid = atoi(pRecord(0)(0,3).c_str());

	return true;
}

/**
 * �����û�ID�õ��û�����Ϸ����
 *
 * @param UserId Ҫȡ�õ��û���ID
 * @param UserData ���ȡ���û����ݳɹ����������ڴ洢ȡ�õ��û�����
 *
 * @return ���ȡ���û����ݳɹ������棬���򷵻ؼ�
 */
bool DBOperator::GetUserData(unsigned int UserId,UserDataStru &UserData)
{
	if(m_DataProvider == NULL || UserId <= 0) return false;

	std::ostringstream sqlstr;
	sqlstr << "call gameserver_getuserdata(" << UserId << ");";

	RecordSetList pRecord = m_DataProvider->execSql(sqlstr.str());
	if(pRecord.isEmpty()) return false;

	UserData.UserId = atol(pRecord(0)(0,0).c_str());
	UserData.Money = _atoi64(pRecord(0)(0,1).c_str());
	UserData.BankMoney = _atoi64(pRecord(0)(0,2).c_str());
	UserData.Revenue = _atoi64(pRecord(0)(0,3).c_str());
	UserData.TotalResult = _atoi64(pRecord(0)(0,4).c_str());
	UserData.Level = atoi(pRecord(0)(0,5).c_str());
	UserData.Experience = atoi(pRecord(0)(0,6).c_str());
//	strncpy(UserData.UserAvatar , pRecord(0)(0,7).c_str(),CountArray(UserData.UserAvatar));
	UserData.TotalBureau = atoi(pRecord(0)(0,8).c_str());
	UserData.SBureau = atoi(pRecord(0)(0,9).c_str());
	UserData.FailBureau = atoi(pRecord(0)(0,10).c_str());
	UserData.RunawayBureau = atoi(pRecord(0)(0,11).c_str());
	UserData.SuccessRate = (float)atof(pRecord(0)(0,12).c_str());
	UserData.RunawayRate = (float)atof(pRecord(0)(0,13).c_str());

	UserData.gType = atoi(pRecord(1)(0,0).c_str());
	UserData.genable = atoi(pRecord(1)(0,1).c_str());
	strncpy(UserData.UserName , pRecord(1)(0,2).c_str(),CountArray(UserData.UserName));
	UserData.sex = atoi(pRecord(1)(0,3).c_str());
	strncpy(UserData.realName , pRecord(1)(0,4).c_str(),CountArray(UserData.realName));
	strncpy(UserData.UserIP , pRecord(1)(0,5).c_str(),CountArray(UserData.UserIP));

	return true;
}

/**
 * ����������ƺ��������������Ƿ����
 *
 * @param name Ҫ������ҵ�����
 * @param password Ҫ������ҵ�����
 *
 * @return �����Ҵ��ڷ�����ҵ�ID�������ھͷ���-1
 */
uint32 DBOperator::IsExistUser(std::string name,std::string password)
{
	if(m_DataProvider == NULL || name.empty() || password.empty()) return 0;

	std::ostringstream sqlstr;
	sqlstr << "call isexistuser('" << name << "','" << password << "');";

	RecordSetList pRecord = m_DataProvider->execSql(sqlstr.str());
	if(pRecord.isEmpty()) return 0;

	return atol(pRecord(0)(0,0).c_str());
}

/// ���·���״̬
bool DBOperator::UpdateRootState(uint32 pRoomId,int pstate)
{
	if(m_DataProvider == NULL) return false;

	std::ostringstream sqlstr;
	sqlstr << "update mol_gameroom set state=" << pstate << " where id=" << pRoomId << ";";

	m_DataProvider->execSql(sqlstr.str());

	return true;
}

/// �õ���Ϸ������Ϣ
bool DBOperator::GetGameRoomInfo(uint32 pRoomId,tagGameRoom *ptagGameRoom)
{
	if(m_DataProvider == NULL || ptagGameRoom == NULL) return false;

	std::ostringstream sqlstr;
	sqlstr << "select gameroom.*,game.processname from mol_gameroom as gameroom,mol_game as game where game.id=gameroom.gameid and gameroom.id=" << pRoomId << ";";

	RecordSetList pRecord = m_DataProvider->execSql(sqlstr.str());
	if(pRecord.isEmpty()) return false;

	ptagGameRoom->gameid = atoi(pRecord(0)(0,1).c_str());
	ptagGameRoom->gamingtype = atoi(pRecord(0)(0,7).c_str());
	ptagGameRoom->serverport = atoi(pRecord(0)(0,3).c_str());
	ptagGameRoom->lastmoney = atol(pRecord(0)(0,8).c_str());
	ptagGameRoom->pielement = atol(pRecord(0)(0,9).c_str());
	ptagGameRoom->roomrevenue = atoi(pRecord(0)(0,10).c_str());
	ptagGameRoom->tablecount = atoi(pRecord(0)(0,4).c_str());
	ptagGameRoom->tableplayercount = atoi(pRecord(0)(0,5).c_str());
	ptagGameRoom->gstate = atoi(pRecord(0)(0,17).c_str());
	ptagGameRoom->QueueGaming = atoi(pRecord(0)(0,15).c_str());

	strncpy(ptagGameRoom->processname , pRecord(0)(0,18).c_str(),CountArray(ptagGameRoom->processname));
	strncpy(ptagGameRoom->serverip , pRecord(0)(0,6).c_str(),CountArray(ptagGameRoom->serverip));
	strncpy(ptagGameRoom->servername , pRecord(0)(0,2).c_str(),CountArray(ptagGameRoom->servername));

	return true;
}

/// ������Ϸ������������
bool DBOperator::UpdateGameRoomOnlinePlayerCount(uint32 pRoomId,int currealplayercount,int currobotplayercount)
{
	if(m_DataProvider == NULL) return false;

	std::ostringstream sqlstr;
	sqlstr << "update mol_gameroom set"
		<< " currealplayercount=" << currealplayercount << ","
		<< " currobotplayercount=" << currobotplayercount << ","
		<< "createtime=NOW()"
		<< " where id=" << pRoomId << ";";

	m_DataProvider->execSql(sqlstr.str());

	return true;
}

/// �õ������˽��뷿��ʱ���
bool DBOperator::GetRobotEnterRoomTimes(void)
{
	if(m_DataProvider == NULL) return false;

	std::ostringstream sqlstr;
	sqlstr << "call isrobotincontroltimes();";

	RecordSetList pRecord = m_DataProvider->execSql(sqlstr.str());
	if(pRecord.isEmpty()) return false;

	return atoi(pRecord(0)(0,0).c_str()) > 0 ? true : false;
}

/// ��������ܵý��ֵ
bool DBOperator::UpdateGamingUserTotalResult(int64 probotresult,int64 pplayerresult)
{
	if(m_DataProvider == NULL) return false;

	// ������mol_userdata����ֹ�û���ȡ
	std::ostringstream sqlstr;
	sqlstr << "call updategametotalmoney(" << pplayerresult << "," << probotresult << ");";

	m_DataProvider->execSql(sqlstr.str());

	return true;
}

/// �õ����л������ܵ���Ӯֵ
int64 DBOperator::GetPlayersTotalResult(int usertype)
{
	if(m_DataProvider == NULL)
		return false;

	std::ostringstream sqlstr;

	if(usertype == 1)
		sqlstr << "call getrobottotalresult(1);";
	else
		sqlstr << "call getrobottotalresult(0);";

	RecordSetList pRecord = m_DataProvider->execSql(sqlstr.str());
	if(pRecord.isEmpty()) return false;

	return _atoi64(pRecord(0)(0,0).c_str());
}

/// �õ������˿�������
bool DBOperator::getrobotcontrolconfig(int64 *probotwinmax, int64 *probotlostmax, int isupdate)
{
	if(m_DataProvider == NULL || probotwinmax == NULL || probotlostmax == NULL)
		return false;

	std::ostringstream sqlstr;
	sqlstr << "call getrobotcontrolconfig(" << isupdate << ");";

	RecordSetList pRecord = m_DataProvider->execSql(sqlstr.str());
	if(pRecord.isEmpty()) return false;

	*probotwinmax = _atoi64(pRecord(0)(0,0).c_str());
	*probotlostmax = _atoi64(pRecord(0)(0,1).c_str());

	return true;
}

