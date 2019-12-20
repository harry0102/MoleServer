#include "DBOperator.h"

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

/// ����ά����ǰ���ݿ�����
void DBOperator::Update(void)
{
	if(m_DataProvider == NULL) return;

	m_DataProvider->Update();
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

/**
 * ����������ƺ��������������Ƿ����
 *
 * @param name Ҫ������ҵ�����
 * @param password Ҫ������ҵ�����
 *
 * @return �����Ҵ��ڷ�����ҵ�ID�������ھͷ���-1
 */
unsigned int DBOperator::IsExistUser(std::string name,std::string password)
{
	if(m_DataProvider == NULL || name.empty() || password.empty()) return 0;

	std::ostringstream sqlstr;
	sqlstr << "call isexistuser('" << name << "','" << password << "');";

	RecordSetList pRecord = m_DataProvider->execSql(sqlstr.str());
	if(pRecord.isEmpty()) return 0;

	return atol(pRecord(0)(0,0).c_str());
}

/// �õ���ǰע���������
unsigned int DBOperator::GetTotalUserCount(void)
{
	if(m_DataProvider == NULL) return 0;

	std::ostringstream sqlstr;
	sqlstr << "select count(uid) from mol_member;";

	RecordSetList pRecord = m_DataProvider->execSql(sqlstr.str());
	if(pRecord.isEmpty()) return 0;

	return atol(pRecord(0)(0,0).c_str());
}

/// ����������Ƽ���������Ƿ����
unsigned int DBOperator::IsExistUser(std::string name)
{
	if(m_DataProvider == NULL || name.empty()) return 0;

	std::ostringstream sqlstr;
	sqlstr << "select uid from mol_member where username='" << name << "' and gtype=0 limit 1;";

	RecordSetList pRecord = m_DataProvider->execSql(sqlstr.str());
	if(pRecord.isEmpty()) return 0;

	return atol(pRecord(0)(0,0).c_str());
}

/// �������ID�����������
bool DBOperator::UpdateUserPassword(uint32 UserId,std::string pwd)
{
	if(m_DataProvider == NULL || UserId <= 0 || pwd.empty()) return false;

	std::ostringstream sqlstr;
	sqlstr << "update mol_member set"
		<< " password='" << pwd << "',"
		<< " bankpassword='" << pwd << "'"
		<< " where uid=" << UserId << ";";

	m_DataProvider->execSql(sqlstr.str());

	return true;
}

/// ����ָ����ҵ���������
bool DBOperator::UpdateUserBankPassword(uint32 UserID,std::string decPWD)
{
	if(m_DataProvider == NULL || UserID <= 0 || decPWD.empty()) return false;

	std::ostringstream sqlstr;

	sqlstr << "update mol_member set bankpassword='" << decPWD << "' where uid=" << UserID << ";";

	m_DataProvider->execSql(sqlstr.str());

	return true;
}

/// ����ָ����ҵĵ�½����
bool DBOperator::UpdateUserLoginPassword(uint32 UserId,std::string decPWD)
{
	if(m_DataProvider == NULL || UserId <= 0 || decPWD.empty()) return false;

	std::ostringstream sqlstr;

	sqlstr << "update mol_member set password='" << decPWD << "' where uid=" << UserId << ";";

	m_DataProvider->execSql(sqlstr.str());

	return true;
}


/**
 * ע���û�
 */
int32 DBOperator::RegisterGameUser(std::string name,std::string password,std::string email,
								  int sex,std::string realname,std::string telephone,std::string AvatorIndex,std::string Referrer,std::string ipaddress,std::string cardnumber)
{
	if(m_DataProvider == NULL)
		return -1;

	std::ostringstream sqlstr;
	sqlstr << "call registergameuser('"
		   << name << "','"
		   << password << "','"
		   << email << "',"
		   << sex << ",'"
		   << realname << "','"
		   << telephone << "','"
		   << AvatorIndex << "','"
		   << Referrer << "','"
		   << ipaddress << "','"
		   << cardnumber << "');";

	RecordSetList pRecord = m_DataProvider->execSql(sqlstr.str());
	if(pRecord.isEmpty()) return -1;

	return atoi(pRecord(0)(0,0).c_str());
}

/// �����û������¼IP�������¼ʱ��
bool DBOperator::UpdatePlayerLastLogin(uint32 UserID,std::string ipaddress,std::string machinecode)
{
	if(m_DataProvider == NULL || UserID <= 0 || ipaddress.empty()) return false;

	std::ostringstream sqlstr;
	sqlstr << "update mol_member set"
		   << " ipaddress='" << ipaddress << "',"
		   << " machinecode='" << machinecode
		   << "',lastlogintime=unix_timestamp(NOW())"
		   << " where uid=" << UserID << ";";

	m_DataProvider->execSql(sqlstr.str());

	return true;
}

/// ����ָ����ҵĻ���
bool DBOperator::LockMachineByUser(uint32 UserId,int operType)
{
	if(m_DataProvider == NULL || UserId <= 0 ) return false;

	std::ostringstream sqlstr;

	switch(operType)
	{
	case 1:               //����
		{
			sqlstr << "update mol_member set"
				<< " glockmachine=1"
				<< " where uid=" << UserId << ";";
		}
		break;
	case 2:               // ����
		{
			sqlstr << "update mol_member set"
				<< " glockmachine=0"
				<< " where uid=" << UserId << ";";
		}
		break;
	default:
		break;
	}

	m_DataProvider->execSql(sqlstr.str());

	return true;
}

/// ����ָ����ҵ���Ϣ
bool DBOperator::UpdateUserInfo(uint32 UserID,std::string nickname,std::string email,std::string telephone,std::string qq,std::string useravatar,int sex)
{
	if(m_DataProvider == NULL || UserID <= 0) return false;

	std::ostringstream sqlstr;
	sqlstr << "call updateuserinfo("
		<< UserID << ",'"
		<< nickname << "','"
		<< email << "','"
		<< telephone << "','"
		<< qq << "','"
		<< useravatar << "',"
		<< sex << ");";

	RecordSetList pRecord = m_DataProvider->execSql(sqlstr.str());
	if(pRecord.isEmpty()) return false;

	return atoi(pRecord(0)(0,0).c_str()) > 0 ? true : false;
}

/**
 * �����û�ID�õ��û�����Ϸ����
 *
 * @param UserId Ҫȡ�õ��û���ID
 * @param UserData ���ȡ���û����ݳɹ����������ڴ洢ȡ�õ��û�����
 *
 * @return ���ȡ���û����ݳɹ������棬���򷵻ؼ�
 */
bool DBOperator::GetUserData(uint32 UserId,MemberDataStru &UserData)
{
	if(m_DataProvider == NULL || UserId <= 0) return false;

	std::ostringstream sqlstr;
	sqlstr << "call getuserdata(" << UserId << ");";

	RecordSetList pRecord = m_DataProvider->execSql(sqlstr.str());
	if(pRecord.isEmpty()) return false;

	UserData.uid = atol(pRecord(0)(0,0).c_str());
	UserData.gtype = atoi(pRecord(0)(0,1).c_str());
	strncpy(UserData.username , pRecord(0)(0,2).c_str(),CountArray(UserData.username));
	strncpy(UserData.password , pRecord(0)(0,3).c_str(),CountArray(UserData.password));
	strncpy(UserData.bankpassword , pRecord(0)(0,5).c_str(),CountArray(UserData.bankpassword));
	strncpy(UserData.email , pRecord(0)(0,6).c_str(),CountArray(UserData.email));
	UserData.sex = atoi(pRecord(0)(0,7).c_str());
	strncpy(UserData.realname , pRecord(0)(0,8).c_str(),CountArray(UserData.realname));
	strncpy(UserData.homeplace , pRecord(0)(0,9).c_str(),CountArray(UserData.homeplace));
	strncpy(UserData.telephone , pRecord(0)(0,10).c_str(),CountArray(UserData.telephone));
	strncpy(UserData.QQ , pRecord(0)(0,11).c_str(),CountArray(UserData.QQ));
	strncpy(UserData.ipaddress , pRecord(0)(0,12).c_str(),CountArray(UserData.ipaddress));
	UserData.createtime = atol(pRecord(0)(0,13).c_str());
	UserData.lastlogintime = atol(pRecord(0)(0,14).c_str());
	UserData.state = atoi(pRecord(0)(0,15).c_str());
	strncpy(UserData.machinecode , pRecord(0)(0,18).c_str(),CountArray(UserData.machinecode));
	UserData.glockmachine = atoi(pRecord(0)(0,19).c_str());

	UserData.money = atol(pRecord(0)(0,21).c_str());
	UserData.bankmoney = atol(pRecord(0)(0,22).c_str());
	UserData.level = atoi(pRecord(0)(0,23).c_str());
	UserData.experience = atoi(pRecord(0)(0,24).c_str());

	return true;
}

/// �õ���ǰ���ߵ����з�����
bool DBOperator::GetOnlineGameRooms(std::vector<tagGameRoom> &pgamerooms)
{
	if(m_DataProvider == NULL) return false;

	std::ostringstream sqlstr;
	sqlstr << "select * from mol_gameroom where state=0;";

	RecordSetList pRecord = m_DataProvider->execSql(sqlstr.str());
	if(pRecord.isEmpty()) return false;

	for(int i=0;i<pRecord(0).rows();i++)
    {
        tagGameRoom ptagGameRoom;
        ptagGameRoom.gameid = atoi(pRecord(0)(i,1).c_str());
        ptagGameRoom.gamingtype = atoi(pRecord(0)(i,7).c_str());
        ptagGameRoom.serverport = atoi(pRecord(0)(i,3).c_str());
        ptagGameRoom.lastmoney = atol(pRecord(0)(i,8).c_str());
        ptagGameRoom.pielement = atol(pRecord(0)(i,9).c_str());
        ptagGameRoom.roomrevenue = atoi(pRecord(0)(i,10).c_str());
        ptagGameRoom.tablecount = atoi(pRecord(0)(i,4).c_str());
        ptagGameRoom.tableplayercount = atoi(pRecord(0)(i,5).c_str());
        ptagGameRoom.state = atoi(pRecord(0)(i,14).c_str());

        strncpy(ptagGameRoom.serverip , pRecord(0)(i,6).c_str(),CountArray(ptagGameRoom.serverip));
        strncpy(ptagGameRoom.servername , pRecord(0)(i,2).c_str(),CountArray(ptagGameRoom.servername));

        pgamerooms.push_back(ptagGameRoom);
    }

	return true;
}

/// ���֮��תǮ
int32 DBOperator::TransferAccounts(uint32 UserID,std::string receiverUser,int64 money)
{
	if(m_DataProvider == NULL || UserID <= 0 || money <= 0 || receiverUser.empty()) return 0;

	std::ostringstream sqlstr;
	sqlstr << "call transferaccounts("
		<< UserID << ",'"
		<< receiverUser << "',"
		<< money << ");";

	RecordSetList pRecord = m_DataProvider->execSql(sqlstr.str());
	if(pRecord.isEmpty()) return 0;

	return atoi(pRecord(0)(0,0).c_str());
}

/// �õ���ҵĽ��
bool DBOperator::GetUserMoney(uint32 UserId,int64 *money,int64 *bankmoney)
{
	if(m_DataProvider == NULL || UserId <= 0) return false;

	std::ostringstream sqlstr;
	sqlstr << "select money,bankmoney from mol_userdata where userid=" << UserId << ";";

	RecordSetList pRecord = m_DataProvider->execSql(sqlstr.str());
	if(pRecord.isEmpty()) return false;

	*money = _atoi64(pRecord(0)(0,0).c_str());
	*bankmoney = _atoi64(pRecord(0)(0,1).c_str());

	return true;
}

/// �õ����е���Ϸ��Ϣ
bool DBOperator::GetGameInfos(std::vector<GameDataInfo> &pGameDataInfos)
{
	if(m_DataProvider == NULL) return false;

	std::ostringstream sqlstr;
	sqlstr << "select * from mol_game;";

	RecordSetList pRecord = m_DataProvider->execSql(sqlstr.str());
	if(pRecord.isEmpty()) return false;

	for(int i=0;i<pRecord(0).rows();i++)
    {
        GameDataInfo pGameDataInfo;
        pGameDataInfo.GameID = atoi(pRecord(0)(i,0).c_str());
        pGameDataInfo.GameType = atoi(pRecord(0)(i,2).c_str());
        pGameDataInfo.MaxVersion = atoi(pRecord(0)(i,3).c_str());
        pGameDataInfo.GameState = atol(pRecord(0)(i,7).c_str());
        pGameDataInfo.showindex = atol(pRecord(0)(i,8).c_str());

        strncpy(pGameDataInfo.GameName , pRecord(0)(i,1).c_str(),CountArray(pGameDataInfo.GameName));
        strncpy(pGameDataInfo.ProcessName , pRecord(0)(i,4).c_str(),CountArray(pGameDataInfo.ProcessName));
        strncpy(pGameDataInfo.GameLogo , pRecord(0)(i,5).c_str(),CountArray(pGameDataInfo.GameLogo));

        pGameDataInfos.push_back(pGameDataInfo);
    }

	return true;
}

///�õ��˺ŷ�����������
bool DBOperator::GetAccountServerConfig(uint32 serverid,tagServerSet &ptagServerSet)
{
	if(m_DataProvider == NULL) return false;

	std::ostringstream sqlstr;
	sqlstr << "select * from mol_accountserver where id=" << serverid << ";";

	RecordSetList pRecord = m_DataProvider->execSql(sqlstr.str());
	if(pRecord.isEmpty()) return false;

	ptagServerSet.id = atoi(pRecord(0)(0,0).c_str());
	strncpy(ptagServerSet.servername , pRecord(0)(0,1).c_str(),CountArray(ptagServerSet.servername));
	ptagServerSet.serverport = atoi(pRecord(0)(0,2).c_str());
	strncpy(ptagServerSet.serverip , pRecord(0)(0,3).c_str(),CountArray(ptagServerSet.serverip));
	ptagServerSet.serverstate = atoi(pRecord(0)(0,5).c_str());
	ptagServerSet.curplayercount = atoi(pRecord(0)(0,6).c_str());

	return true;
}

///���·�������������
bool DBOperator::UpdateAccountServerOnlinePlayerCount(uint32 serverid,int playercount)
{
	if(m_DataProvider == NULL || playercount == -1) return false;

	std::ostringstream sqlstr;
	sqlstr << "update mol_accountserver set"
		   << " curplayercount=" << playercount
		   << ",createtime=NOW()"
		   << " where id=" << serverid << ";";

	m_DataProvider->execSql(sqlstr.str());

	return true;
}

