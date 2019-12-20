#ifndef _C_COMMON_H_INCLUDE_
#define _C_COMMON_H_INCLUDE_

#include <jsoncpp/json/json.h>

//////////////////////////////////////////////////////////////////////////
//���ó���

//��Ч��ֵ
#define INVALID_BYTE				((unsigned char)(0xFF))						//��Ч��ֵ
#define INVALID_WORD				((unsigned short)(0xFFFF))					//��Ч��ֵ
#define INVALID_DWORD				((unsigned int)(0xFFFFFFFF))				//��Ч��ֵ

//////////////////////////////////////////////////////////////////////////
//���ó���

//��Ч��ֵ
#define INVALID_TABLE				INVALID_WORD						//��Ч����
#define INVALID_CHAIR				INVALID_WORD						//��Ч����

//////////////////////////////////////////////////////////////////////////

//����ά��
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))

//��Ч��ַ
#define INVALID_IP_ADDRESS(IPAddress) (((IPAddress==0L)||(IPAddress==INADDR_NONE)))

//////////////////////////////////////////////////////////////////////////

//�洢����
#ifdef _UNICODE
	#define CountStringBuffer CountStringBufferW
#else
	#define CountStringBuffer CountStringBufferA
#endif

//�洢����
#define CountStringBufferA(String) ((uint32)((lstrlenA(String)+1)*sizeof(CHAR)))
#define CountStringBufferW(String) ((uint32)((lstrlenW(String)+1)*sizeof(WCHAR)))

//////////////////////////////////////////////////////////////////////////

//�ӿ��ͷ�
#define  SafeRelease(pObject) { if (pObject!=NULL) { pObject->Release(); pObject=NULL; } }

//ɾ��ָ��
#define  SafeDelete(pData) { try { delete pData; } catch (...) { assert(false); } pData=NULL; }

//ɾ������
#define  SafeDeleteArray(pData) { try { delete [] pData; } catch (...) { assert(false); } pData=NULL; }

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                   ��Ϸ�ͻ���ʹ�õĶ�ʱ��1-1000                               //
//////////////////////////////////////////////////////////////////////////////////
#define IDD_TIMER_HALL_ADVERTISEMENT              100                   // ����������
#define IDD_TIMER_HALL_CONNECTGAMESERVER          101                   // ������Ϸ������
#define IDD_TIMER_HALL_CONTROL_CHAT               102                   // ������������
#define IDD_TIMER_HALL_RECONNECTGAMESERVER        103                   // ��������
#define IDD_TIMER_HALL_ENTER_GAMESERVER           104                   // ������Ϸ������
#define IDD_TIMER_HALL_HEART                      105                   // ������Ϣ
#define IDD_TIMER_HALL_GET_USER_ADDRESS           106                   // �õ���ҵ���λ��
#define IDD_TIMER_HALL_GETLASTGAMINGNEWS          107                   // ÿ�����ˢ��һ����Ϸ��Ϣ

//////////////////////////////////////////////////////////////////////////////////
//                   ��Ϸϵͳ�߼�ʹ�õĶ�ʱ��1000�Ժ�                           //
//////////////////////////////////////////////////////////////////////////////////
#define IDD_TIMER_GAME_START                 1000                  // ��Ϸ��ʱ����1000��ʼ

//////////////////////////////////////////////////////////////////////////////////
//                   ��Ϸϵͳ�߼�ʹ�õ�GUI��ID��1000�Ժ�                        //
//////////////////////////////////////////////////////////////////////////////////
#define IDD_GUI_ID_START                     1000                  // ��Ϸ��ʱ����1000��ʼ

//////////////////////////////////////////////////////////////////////////////////
//                   ��ϷϵͳҪ���͵���Ϣ����                                   //
//////////////////////////////////////////////////////////////////////////////////
#define IDD_MESSAGE_TYPE_SUPER_BIG_MSG       1                     // ��������Ա��������Ϣ
#define IDD_MESSAGE_TYPE_SUPER_SMAILL_MSG    2                     // ��������ԱС������Ϣ
#define IDD_MESSAGE_TYPE_GAMESERVER_ENTERTIP 3                     // ��Ϸ������������ʾ
#define IDD_MESSAGE_TYPE_CLOSE_SERVER        4                     // �ر���Ϸ������
#define IDD_MESSAGE_TYPE_OPEN_SERVER         5                     // ����Ϸ������
#define IDD_MESSAGE_TYPE_GAME_ADVERTISEMENT  6                     // ��Ϸ���
#define IDD_MESSAGE_TYPE_GAMESERVER_SYSTEM   7                     // ϵͳ��Ϣ
#define IDD_MESSAGE_TYPE_CHAT_COMMON         8                     // ����
#define IDD_MESSAGE_TYPE_CHAT_PRIVATE        9                     // ˽��
#define IDD_MESSAGE_TYPE_NULL                10                    // ʲôҲû��

//////////////////////////////////////////////////////////////////////////

/**
 * ��Ϸ��������
 */
enum SceneType
{
	SCENE_TYPE_HALL = 0,               // ����
	SCENE_TYPE_GAMEROOM,               // ��Ϸ����
	SCENE_TYPE_NULL
};

//��ʼģʽ
enum enStartMode
{
	enStartMode_FullReady,				//���˿�ʼ(��Ϸ��������������׼��)
	enStartMode_AllReady,				//����׼��(��Ϸ���������������˶�׼��)
	enStartMode_Symmetry,				//�Գƿ�ʼ
	enStartMode_TimeControl,			//ʱ�����
};

// ��־�ȼ�
enum enLogLevel
{
    BASIC = 0,
    DETAIL,
    ERROR,
    DEBUG
};

#endif
