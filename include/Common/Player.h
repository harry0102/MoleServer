#ifndef _PLAYER_H_INCLUDE_
#define _PLAYER_H_INCLUDE_

#include <string>

#include "ccommon.h"

/**
 * ���״̬
 */
enum PlayerState
{
	PLAYERSTATE_NORAML = 0,             // ����״̬
	PLAYERSTATE_READY,                  // ׼��״̬
	PLAYERSTATE_GAMING,                 // ��Ϸ��״̬
	PLAYERSTATE_LOSTLINE,               // ����״̬
	PLAYERSTATE_QUEUE                   // �Ŷ�״̬
};

/**
 * �������
 */
enum PlayerType
{
	PLAYERTYPE_NORMAL = 0,              // ��ͨ�û�
	PLAYERTYPE_ROBOT                    // �������û�
};

/**
 * ����豸����
 */
enum PlayerDeviceType
{
	PLAYERDEVICETYPE_PC = 0,            // PC�û�
	PLAYERDEVICETYPE_ANDROID,           // ��׿�û�
	PLAYERDEVICETYPE_IOS,               // ios�û�
	PLAYERDEVICETYPE_FLASH,             // flash�û�
	PLAYERDEVICETYPE_NULL
};

class Player
{
public:
	/// �������ID
	virtual void SetID(int id) = 0;
	/// �õ����ID
	virtual int GetID(void) = 0;
	/// �����������ID
	virtual void SetConnectID(uint32 id) = 0;
	/// �õ��������ID
	virtual uint32 GetConnectID(void) = 0;
	/// ����������ڵķ���ID
	virtual void SetRoomId(int id) = 0;
	/// �õ�������ڵķ���ID
	virtual int GetRoomId(void) = 0;
	/// �����������
	virtual void SetType(PlayerType type) = 0;
	/// �õ��������
	virtual PlayerType GetType(void) = 0;
	/// ��������豸����
	virtual void SetDeviceType(PlayerDeviceType type) = 0;
	/// �õ�����豸����
	virtual PlayerDeviceType GetDeviceType(void) = 0;
	/// ��������ڷ����е�����
	virtual void SetChairIndex(int index) = 0;
	/// �õ�����ڷ����е�����
	virtual int GetChairIndex(void) = 0;
	/// �����������
	virtual void SetName(std::string name) = 0;
	/// �õ��������
	virtual std::string GetName(void) = 0;
	/// �õ����״̬
	virtual PlayerState GetState(void) = 0;
	/// �������״̬
	virtual void SetState(PlayerState state) = 0;
	/// �����Ƿ��Թ�
	virtual void SetLookOn(bool isLook) = 0;
	/// �õ��Ƿ��Թ�
	virtual bool IsLookOn(void) = 0;

	/// ������ҽ�Ǯ����
	virtual void SetMoney(int64 money) = 0;
	/// �õ���ҽ������
	virtual int64 GetMoney(void) = 0;
	/// ����������н�Ǯ����
	virtual void SetBankMoney(int64 money) = 0;
	/// �õ�������н������
	virtual int64 GetBankMoney(void) = 0;
	/// �������˰��
	virtual void SetRevenue(int64 revenue) = 0;
	/// �õ����˰��
	virtual int64 GetRevenue(void) = 0;
	/// ���������Ӯֵ
	virtual void SetTotalResult(int64 result) = 0;
	/// �õ������Ӯֵ
	virtual int64 GetTotalResult(void) = 0;
	/// ������ҵȼ�
	virtual void SetLevel(int level) = 0;
	/// �õ���ҵȼ�
	virtual int GetLevel(void) = 0;
	/// ������Ҿ���ֵ
	virtual void SetExperience(unsigned int exp) = 0;
	/// �õ���Ҿ���ֵ
	virtual unsigned int GetExperience(void) = 0;
	/// ��������ܾ�
	virtual void SetTotalBureau(int bureau) = 0;
	/// �õ�����ܾ�
	virtual int GetTotalBureau(void) = 0;
	/// �������Ӯ��
	virtual void SetSuccessBureau(int bureau) = 0;
	/// �õ����Ӯ��
	virtual int GetSuccessBureau(void) = 0;
	/// ����������
	virtual void SetFailBureau(int bureau) = 0;
	/// �õ�������
	virtual int GetFailBureau(void) = 0;
	/// �������ܴ���
	virtual void SetRunawayBureau(int bureau) = 0;
	/// �õ����ܴ���
	virtual int GetRunawayBureau(void) = 0;
	/// �������ʤ��
	virtual void SetSuccessRate(float rate) = 0;
	/// �õ����ʤ��
	virtual float GetSuccessRate(void) = 0;
	/// �������������
	virtual void SetRunawayrate(float rate) = 0;
	/// �õ����������
	virtual float GetRunawayrate(void) = 0;
	/// ��������Ա�
	virtual void SetSex(int psex) = 0;
	/// �õ�����Ա�
	virtual int GetSex(void) = 0;
	/// ���������ʵ����
	virtual void SetRealName(std::string rn) = 0;
	/// �õ������ʵ����
	virtual std::string GetRealName(void) = 0;
	/// �������ϵͳ����
	virtual void SetSysType(int type) = 0;
	/// �õ����ϵͳ����
	virtual int GetSysType(void) = 0;
	/// ����IP��ַ
	virtual void SetLoginIP(uint32 ip) = 0;
	/// �õ�IP��ַ
	virtual uint32 GetLoginIP(void) = 0;

	/// ������ҵ�ǰ������ϷID
	virtual void setCurGameID(uint32 gameid) = 0;
	/// �õ���ҵ�ǰ������ϷID
	virtual uint32 getCurGameID(void) = 0;
	/// ������ҵ�ǰ��������ID
	virtual void setCurTableIndex(int32 tableindex) = 0;
	/// �õ���ҵ�ǰ��������ID
	virtual int32 getCurTableIndex(void) = 0;
	/// �õ���ҵ�ǰ��������ID
	virtual void setCurChairIndex(int32 chairindex) = 0;
	/// �õ���ҵ�ǰ��������ID
	virtual int32 getCurChairIndex(void) = 0;
	/// ������ҵ�ǰ���ڷ�����ID
	virtual void setCurServerId(uint32 serverid) = 0;
	/// �õ���ҵ�ǰ���ڷ�����ID
	virtual uint32 getCurServerId(void) = 0;
	/// ������ҵ�ǰ�Ƿ�����Ϸ��
	virtual void setCurGamingState(bool state) = 0;
	/// �õ���ҵ�ǰ�Ƿ�����Ϸ��
	virtual bool getCurGamingState(void) = 0;
	/// �����Ƿ���Գ齱
	virtual void setIsChouJiang(bool iscj) = 0;
	/// �õ��Ƿ���Գ齱
	virtual bool IsChouJiang(void) = 0;

	/// ��ʼһ����ʱ��
	virtual bool StartTimer(int timerId,int space) = 0;
	/// �ر�һ����ʱ��
	virtual void StopTimer(int id) = 0;
	/// �ر����еĶ�ʱ��
	virtual void StopAllTimer(void) = 0;

	/// ������Ϸ����(�˺���ֻ���ڻ�����ʹ�ã������û�ʹ����ʹ�÷�����Ϣ���ͺ���)
	virtual void SendGameMsg(Json::Value &msg) = 0;
	/// ����׼����Ϣ
	virtual void SendReadyMsg(void) = 0;
};

#endif
