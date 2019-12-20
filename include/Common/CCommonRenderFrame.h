#ifndef _C_COMMON_RENDER_FRAME_H_INCLUDE_
#define _C_COMMON_RENDER_FRAME_H_INCLUDE_

#include "ccommon.h"

class CCommonRenderDevice;
class CCommonSocket;
class CMolMessageIn;
class Player;
class Room;

/** 
 * ͨ�û�ͼ��ܣ����ڸ��ֳ����Ļ���
 */
class CCommonRenderFrame
{
public:
	/// ���캯��
	CCommonRenderFrame(SceneType type=SCENE_TYPE_GAMEROOM) { }
	/// ��������
	virtual ~CCommonRenderFrame() {}

	/// ������Ϸ����
	virtual void SetSceneType(SceneType type) = 0;
	/// �õ���Ϸ����
	virtual SceneType GetSceneType(void) = 0;
	/// ������Ⱦ�豸
	virtual void SetDevice(CCommonRenderDevice *pDevice) = 0;
	/// ��������
	virtual void SetGameRoom(Player *pPlayer,Room *pRoom) = 0;
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
	/// ����ϵͳ��Ϣ
	virtual void OnProcessSystemMsg(int msgType,CString msg) = 0;
	/// ���������Ϣ
	virtual void OnProcessMatchingMsg(int msgType,CMolMessageIn *mes) = 0;

	///////////////////////////���º�����Ϸ��ʹ��///////////////////////////////////////

	/// ���ڴ����û���ʼ��Ϸ��ʼ��Ϣ
	virtual void OnProcessPlayerGameStartMes(void) = 0;
	/// ���ڴ����û�������Ϸ��Ϣ
	virtual void OnProcessPlayerGameOverMes(void) = 0;
	/// ���ڴ����û�������Ϸ��������Ϣ
	virtual void OnProcessPlayerRoomMes(CMolMessageIn *mes) = 0;
	/// �����û����뷿����Ϣ
	virtual void OnProcessEnterRoomMsg(int pChairId) = 0;
	/// �����û��뿪������Ϣ
	virtual void OnProcessLeaveRoomMsg(int pChairId) = 0;
	/// �����û�������Ϣ
	virtual void OnProcessOfflineRoomMes(int pChairId) = 0;
	/// �����û�����������Ϣ
	virtual void OnProcessReEnterRoomMes(int pChairId,CMolMessageIn *mes) = 0;

	////////////////////////////////////////////////////////////////////////////////////
};

#endif