#ifndef _C_COMMON_RENDER_HALL_H_INCLUDE_
#define _C_COMMON_RENDER_HALL_H_INCLUDE_

#include "ccommon.h"

class CCommonRenderDevice;
class CCommonSocket;
class CMolMessageIn;
class Player;
class Room;

/** 
 * ͨ�û�ͼ��ܣ�ֻ���ڴ����Ļ���
 */
class CCommonRenderHall
{
public:
	/// ���캯��
	CCommonRenderHall() { }
	/// ��������
	virtual ~CCommonRenderHall() {}

	/// ������Ⱦ�豸
	virtual void SetDevice(CCommonRenderDevice *pDevice) = 0;
	/// ��Ϸ��Դ����
	virtual bool LoadGameResources(void) = 0;
	/// ж����Ϸ��Դ
	virtual bool ShutdownGameResources(void) = 0;
	/// ������Ϸ����
	virtual void ResetGameScene(void) = 0;
	/// ��Ϸ��������
	virtual void DrawGameScene(void) = 0;
	/// ��Ϸ���¼�����
	virtual bool OnEvent(const SEvent& event) = 0;
	/// �����û���ʱ����Ϣ
	virtual void OnProcessTimerMsg(int timerId,int curTimer) = 0;

	/// �����û�������½��Ϣ
	virtual void OnProcessUserFrameLoginMes(CMolMessageIn *mes) = 0;
	/// �����û�������ȡ������Ϸ�������б���Ϣ
	virtual void OnProcessUserFrameGameServerListMes(CMolMessageIn *mes) = 0;
	/// �����û�������ȡ������Ϸ�б���Ϣ
	virtual void OnProcessUserFrameGameListMes(CMolMessageIn *mes) = 0;
	/// �����û���½��Ϸ��������Ϣ
	virtual void OnProcessUserGameServerLoginMes(CMolMessageIn *mes) = 0;
	/// ������Ϸ�����������Ϣ
	virtual void OnProcessGameFrameMes(CMolMessageIn *mes) = 0;
	/// ������Ϸ������������Ϣ
	virtual void OnProcessGameRoomMes(CMolMessageIn *mes) = 0;
	/// �����ȡ��ǰ��Ϸ�����߷���������
	virtual void OnProcessGetGameServerOnlineCount(CMolMessageIn *mes) = 0;
};

#endif