#ifndef _C_PLAYER_H_INCLUDE_
#define _C_PLAYER_H_INCLUDE_

#include <string>

#include "../gameserver_common.h"

#include "../../include/molnet/Network.h"
#include "../../include/Common/Player.h"
#include "../../include/Common/Robotlogicframe.h"

#include "../../include/molnet/netallocator/NedAllocatedObject.h"

//class CPlayer;
//class RobotLogicFrame;

class CPlayer : public Player , public NedAllocatedObject
{
public:
	CPlayer(PlayerType type);
	CPlayer(int id,uint32 conid);
	~CPlayer();

	/// �������ID
	virtual void SetID(int id) { m_Id = id; }
	/// �õ����ID
	virtual int GetID(void) { return m_Id; }
	/// �����������ID
	virtual void SetConnectID(uint32 id) { m_ConnectId = id; }
	/// �õ��������ID
	virtual uint32 GetConnectID(void) { return m_ConnectId; }
	/// ����������ڵķ���ID
	virtual void SetRoomId(int id) { m_roomId = id; }
	/// �õ�������ڵķ���ID
	virtual int GetRoomId(void) { return m_roomId; }
	/// �����������
	virtual void SetType(PlayerType type) { m_PlayerType = type; }
	/// �õ��������
	virtual PlayerType GetType(void) { return m_PlayerType; }
	/// ��������豸����
	virtual void SetDeviceType(PlayerDeviceType type) { m_PlayerDeviceType = type; }
	/// �õ�����豸����
	virtual PlayerDeviceType GetDeviceType(void) { return m_PlayerDeviceType; }
	/// ��������ڷ����е�����
	virtual void SetChairIndex(int index) { m_ChairIndex = index; }
	/// �õ�����ڷ����е�����
	virtual int GetChairIndex(void) { return m_ChairIndex; }
	/// �����������
	virtual void SetName(std::string name) { m_Name = name; }
	/// �õ��������
	virtual std::string GetName(void) { return m_Name; }
	/// �õ����״̬
	virtual PlayerState GetState(void) { return m_PlayerState; }
	/// �������״̬
	virtual void SetState(PlayerState state)
	{
		m_PlayerState = state;
	}
	/// �����Ƿ��Թ�
	virtual void SetLookOn(bool isLook) { m_isLookOn = isLook; }
	/// �õ��Ƿ��Թ�
	virtual bool IsLookOn(void) { return m_isLookOn; }

	/// ������ҽ�Ǯ����
	virtual void SetMoney(int64 money) { m_Money = money; }
	/// �õ���ҽ������
	virtual int64 GetMoney(void) { return m_Money; }
	/// ����������н�Ǯ����
	virtual void SetBankMoney(int64 money) { m_BankMoney = money; }
	/// �õ�������н������
	virtual int64 GetBankMoney(void) { return m_BankMoney; }
	/// �������˰��
	virtual void SetRevenue(int64 revenue) { m_Revenue = revenue; }
	/// �õ����˰��
	virtual int64 GetRevenue(void) { return m_Revenue; }
	/// ���������Ӯֵ
	virtual void SetTotalResult(int64 result) { m_TotalResult = result; }
	/// �õ������Ӯֵ
	virtual int64 GetTotalResult(void) { return m_TotalResult; }
	/// ������ҵȼ�
	virtual void SetLevel(int level) { m_level = level; }
	/// �õ���ҵȼ�
	virtual int GetLevel(void) { return m_level; }
	/// ������Ҿ���ֵ
	virtual void SetExperience(unsigned int exp) { m_experience = exp; }
	/// �õ���Ҿ���ֵ
	virtual unsigned int GetExperience(void) { return m_experience; }
	/// ��������ܾ�
	virtual void SetTotalBureau(int bureau) { m_totalbureau = bureau; }
	/// �õ�����ܾ�
	virtual int GetTotalBureau(void) { return m_totalbureau; }
	/// �������Ӯ��
	virtual void SetSuccessBureau(int bureau) { m_successbureau = bureau; }
	/// �õ����Ӯ��
	virtual int GetSuccessBureau(void) { return m_successbureau; }
	/// ����������
	virtual void SetFailBureau(int bureau) { m_failbureau = bureau; }
	/// �õ�������
	virtual int GetFailBureau(void) { return m_failbureau; }
	/// �������ܴ���
	virtual void SetRunawayBureau(int bureau) { m_RunawayBureau = bureau; }
	/// �õ����ܴ���
	virtual int GetRunawayBureau(void) { return m_RunawayBureau; }
	/// �������ʤ��
	virtual void SetSuccessRate(float rate) { m_successrate = rate; }
	/// �õ����ʤ��
	virtual float GetSuccessRate(void) { return m_successrate; }
	/// �������������
	virtual void SetRunawayrate(float rate) { m_runawayrate = rate; }
	/// �õ����������
	virtual float GetRunawayrate(void) { return m_runawayrate; }
	/// ��������Ա�
	virtual void SetSex(int psex) { sex = psex; }
	/// �õ�����Ա�
	virtual int GetSex(void) { return sex; }
	/// ���������ʵ����
	virtual void SetRealName(std::string rn) { realname = rn; }
	/// �õ������ʵ����
	virtual std::string GetRealName(void) { return realname; }
	/// �������ϵͳ����
	virtual void SetSysType(int type) { gtype = type; }
	/// �õ����ϵͳ����
	virtual int GetSysType(void) { return gtype; }
	/// ����IP��ַ
	virtual void SetLoginIP(uint32 ip) { ipaddress = ip; }
	/// �õ�IP��ַ
	virtual uint32 GetLoginIP(void) { return ipaddress; }

	/// ������ҵ�ǰ������ϷID
	virtual void setCurGameID(uint32 gameid) { m_CurGameType = gameid; }
	/// �õ���ҵ�ǰ������ϷID
	virtual uint32 getCurGameID(void) { return m_CurGameType; }
	/// ������ҵ�ǰ��������ID
	virtual void setCurTableIndex(int32 tableindex) { m_CurTableIndex = tableindex; }
	/// �õ���ҵ�ǰ��������ID
	virtual int32 getCurTableIndex(void) { return m_CurTableIndex; }
	/// �õ���ҵ�ǰ��������ID
	virtual void setCurChairIndex(int32 chairindex) { m_CurChairIndex = chairindex; }
	/// �õ���ҵ�ǰ��������ID
	virtual int32 getCurChairIndex(void) { return m_CurChairIndex; }
	/// ������ҵ�ǰ���ڷ�����ID
	virtual void setCurServerId(uint32 serverid) { m_CurServerPort = serverid; }
	/// �õ���ҵ�ǰ���ڷ�����ID
	virtual uint32 getCurServerId(void) { return m_CurServerPort; }
	/// ������ҵ�ǰ�Ƿ�����Ϸ��
	virtual void setCurGamingState(bool state) { m_CurGamingState = state; }
	/// �õ���ҵ�ǰ�Ƿ�����Ϸ��
	virtual bool getCurGamingState(void) { return m_CurGamingState; }
	/// �����Ƿ���Գ齱
	virtual void setIsChouJiang(bool iscj) { m_isChoujiang = iscj; }
	/// �õ��Ƿ���Գ齱
	virtual bool IsChouJiang(void) { return m_isChoujiang; }

	/// ���÷����������
	inline void SetEnterPassword(std::string pwd) { m_roomenterpwd = pwd; }
	/// �õ������������
	inline std::string getEnterPassword(void) { return m_roomenterpwd; }
	/// ���÷�������ҷ�Χ
	inline void SetEnterMoneyRect(int64 first,int64 second) { m_roomentermoneyfirst = first; m_roomentermoneysecond = second; }
	/// �õ���������ҷ�Χ
	inline void GetEnterMoneyRect(int64 *first,int64 *second) { *first = m_roomentermoneyfirst; *second = m_roomentermoneysecond; }


	/// ����׼��ʱ��
	inline void SetReadyTime(DWORD time) { m_RealyTime = time; }
	/// �õ�׼��ʱ��
	inline DWORD GetReadyTime(void) { return m_RealyTime; }

	/// ��ʼһ����ʱ��
	virtual bool StartTimer(int timerId,int space);
	/// �ر�һ����ʱ��
	virtual void StopTimer(int id);
	/// �ر����еĶ�ʱ��
	virtual void StopAllTimer(void);

	/// ������Ϸ����
	virtual void SendGameMsg(Json::Value &msg);
	/// ����׼����Ϣ
	virtual void SendReadyMsg(void);

	/// ���û�������Ϸ�߼�����ӿ�
	void SetRobotLogicFrame(RobotLogicFrame *frame);
	/// �õ���������Ϸ�߼�����ӿ�
	RobotLogicFrame* GetRobotLogicFrame(void) { return m_RobotLogicFrame; }
	///ʱ���¼�
	bool OnEventTimer(DWORD dwTimerID, DWORD wBindParam);

	/// ���õ�ǰ��Ϸ����
	void SetCurGameRoom(Room* pRoom);
	/// ���õ��ߺ������ID
	inline void SetOffLineConnectId(uint32 connid) { m_offlineConnectId = connid; }
	/// �õ����ߺ������ID
	inline uint32 GetOffLineConnectId(void) { return m_offlineConnectId; }

	/// ���õ�ǰ��Ϸ�����Ӯֵ
	inline void setCurrentGamingResult(int64 presult) { m_curGamingResult = presult; }
	/// �õ���ǰ��Ϸ�����Ӯֵ
	inline int64 getCurrentGamingResult(void) { return m_curGamingResult; }

private:
	/// ɾ�����еĶ�ʱ��
	void DeleteAllTimer(void);

	//ʱ�����
protected:
	CTimerItemArray					m_TimerItemActive;					//�����
	Mutex                           m_TimerLock;                        /**< ���ڱ�������еĶ�ʱ�� */

private:
	int m_Id;         /**< ���ID */
	uint32 m_ConnectId;  /**< ��ҵ�����ID */
	uint32 ipaddress;    /**< ���IP */
	uint32 m_offlineConnectId;  /**< ���ߺ������ID */
	int m_roomId;     /**< ������ڵķ���ID */
	int m_ChairIndex;   /**< �û��ڷ����е����� */
	PlayerState m_PlayerState;       /**< ��ҵ�ǰ״̬ */
	PlayerType m_PlayerType;           /**< ������� */
	PlayerDeviceType m_PlayerDeviceType; /**< ����豸���� */
	bool m_isLookOn;                   /**< �Ƿ�Ϊ�Թ� */

	std::string m_Name;  /**< ��ҵ����� */

	std::string m_roomenterpwd; /**< ����������� */
	int64 m_roomentermoneyfirst,m_roomentermoneysecond;  /**< ��������ҷ�Χ */

	int64 m_Money;              /**< ��ҽ�Ǯ���� */
	int64 m_BankMoney;                /**< ���н�Ǯ���� */
	int64 m_TotalResult;              /**< ����ܵ���Ӯֵ */
	int64 m_Revenue;                  /**< ���˰�� */
	int m_level;                       /**< ��ҵȼ� */
	unsigned int m_experience;         /**< ��Ҿ���ֵ */
	std::string m_useravatar;         /**< �û�ͷ�� */
	int m_totalbureau;                 /**< ����ܾ� */
	int m_successbureau;               /**< ���Ӯ�� */
	int m_RunawayBureau;			   /**< ������ܴ��� */
	int m_failbureau;                  /**< ������ */
	float m_successrate;               /**< ���ʤ�� */
	float m_runawayrate;               /**< ��������� */
	int sex;                      //����Ա�
	std::string realname;           //��ʵ����
	int gtype;                    //�������

	int64 m_curGamingResult;                         /**< ��ǰ��Ϸ��Ӯֵ */

	uint32 m_CurGameType;                            /**< ��ǰ������Ϸ���� */
	uint32 m_CurServerPort;                          /**< ��ǰ������Ϸ�������˿� */
	int32 m_CurTableIndex;                          /**< ��ǰ������Ϸ����ID */
	int32 m_CurChairIndex;                          /**< ��ǰ������Ϸ����ID */
	bool m_CurGamingState;                          /**< ��ǰ�����Ϸ״̬ */
	bool m_IsMatchSignUp;                           /**< �Ƿ�����ǰ���� */
	bool m_isChoujiang;                                 /**< �Ƿ������Ϸ��齱 */

	DWORD           m_RealyTime;                  /**< ׼��ʱ�� */

	RobotLogicFrame *m_RobotLogicFrame;             /**< �����˴����� */
};

#endif
