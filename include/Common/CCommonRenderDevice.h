#ifndef _C_COMMON_RENDER_DEVICE_H_INCLUDE_
#define _C_COMMON_RENDER_DEVICE_H_INCLUDE_

class CCommonRenderFrame;
class CMolMessageOut;
class Player;

/** 
 * ������Ϸʲô�׶�
 */
enum GameStage
{
	GAMESTATE_MAINHALL = 0,               // ����
	GAMESTATE_GAMEROOM,                   // ����
	GAMESTATE_NULL 
};

/** 
 * ¼��״̬
 */
enum VideoState
{
	VIDEO_RECORDING = 0,                       //¼��
	VIDEO_PLAYBACK,                            //�ط�
	VIDEO_NULL
};

/** 
 * ͨ�õ���Ⱦ�豸
 */
class CCommonRenderDevice 
{
public:
	/// �õ���Ⱦ�豸
	virtual irr::IrrlichtDevice* GetDevice(void) = 0;
	/// ��ʾͼƬ
	virtual void draw2DImage(irr::video::ITexture* texture, irr::core::rect<irr::s32> sourceRect, const core::matrix4& mat, bool useAlphaChannel, irr::video::SColor color, bool useFiltering/* = true*/, bool usePremultipliedAlpha/* = false*/) = 0;
	/// �õ���ǰ��Ϸ��������
	virtual GameStage GetCurGameSceneStage(void) = 0;
	/// �õ���ǰ��Ϸ����
	virtual CCommonRenderFrame* GetCurrentGameScene(void) = 0;
	/// �뿪��Ϸ����
	virtual void LeaveGameRoom(bool isTip=true) = 0;
	/// �����Ƿ�Ҫ�˳���Ϸ��ʾ
	virtual void SetIsCloseTip(bool tip) = 0;
	/// �õ���ǰӦ�ó���
	virtual CWinApp* GetCurWinApp(void) = 0;
	/// ������Ϸ֡��
	virtual void SetFPS(int frame=60) = 0;
	/// �������ID�õ������Ϣ
	virtual Player* GetOnlinePlayer(int userid) = 0;
	/// �õ���ǰ�������������
	virtual int GetOnlinePlayerCount(void) = 0;
	/// ȫ�������л�
	virtual void SwitchToFullscreen(void) = 0;
	/// ������ҵõ���ҵ�ͷ��ID
	virtual int GetAvatarIndexByPlayer(CString avatarIndex) = 0;
	/// �õ���ǰϵͳͼ���б�
	virtual gui::IGUISpriteBank* GetAvatarSpriteBank(void) = 0;
	/// �õ���ǰҪʹ�õ��Զ���ͷ���������ַ
	virtual CString GetCustomAvatarUrl(void) = 0;
	/// �������͵õ���ǰѡ�е���ַ(0:��ҳ;1:��̳;2:�ͷ�;3:��ֵ;4:�һ�����;5:�û�ͷ��;6:�̳�;7:��Ϸ����)
	virtual CString GetSelectedUrlByType(int type,bool isOpen=false) = 0;
	/// �õ�ָ���ļ������ĵ�ǰ����·��
	virtual CString GetCurrentWorkingPath(CString filename) = 0;
	/// ���һ��ͼ�굽ϵͳ��
	virtual void addTextureAsSprite(CString icon,CString path) = 0;
	/// ��Ϸ״̬��ʾ
	virtual BOOL ShowBalloonTip(LPCTSTR szMessage,LPCTSTR szTitle,UINT uTimeout=5,DWORD dwInfoFlags=1,bool isBalloon=false) = 0;
	/// ��ʾͷ��
	virtual video::ITexture* DrawUserAvatar(Player *pPlayer,core::rect<s32> pRect) = 0;
	/// �õ���Ϸ�г�����ʾ��
	virtual CString GetGamingCommonUseTip(CString tip) = 0;
	/// �õ��û�����Ϸ�гƺ�
	virtual CString GetPlayerPraised(Player *pPlayer) = 0;
	/// �����Ϣ��ʾ��
	virtual gui::IGUITextArea* AddInformationBox(const core::rect<s32>& rectangle,IGUIElement* parent=0) = 0;
	/// ��ӱ���ѡ���
	virtual gui::IGUIChatShow* addChatShow(const core::rect<s32>& rectangle,IGUIElement* parent=0) = 0;
	/// ������쳣����ʾ���
	virtual gui::IGUIChatTip* addChatTip(const core::rect<s32>& rectangle,IGUIElement* parent=0) = 0;
	/// ��Ӱ���ѡ���
	virtual gui::IGUIPackage* addPackage(const core::rect<s32>& rectangle,IGUIElement* parent=0) = 0;
	/// �����Ϣ������
	virtual gui::IGUIScrollBox* addScrollBox(const core::rect<s32>& rectangle,IGUIElement* parent=0) = 0;
	/// ��ʾ��Ϣ
	virtual void ShowMessage(CString msg,video::SColor col=video::SColor(255,255,255,255),int msgType=IDD_MESSAGE_TYPE_NULL,CString sender=TEXT(""),CString reciver=TEXT("")) = 0;

	/// ��ʼһ����ʱ��
	virtual bool StartTimer(int timerId,int space) = 0;
	/// �ر�һ����ʱ��
	virtual void StopTimer(int id) = 0;
	/// �ر����еĶ�ʱ��
	virtual void StopAllTimer(void) = 0;

	/// ����ָ���������ļ�
	virtual void MusicPlay(CString sound,bool isLoop=false) = 0;
	/// ����ָ������Ч�ļ�
	virtual void EffectPlay(CString sound) = 0;
	/// ָֹͣ���������ļ�
	virtual void SoundStop(CString sound) = 0;
	/// ֹͣ�������е�����
	virtual void SoundStopAll(void) = 0;

	/// ������Ϸ����
	virtual void SendGameMsg(CMolMessageOut &msg) = 0;
	/// ����׼����Ϣ
	virtual void SendReadyMsg(void) = 0;
	/// ���ͻ�����Ϣ
	virtual void SendChangeTableMsg(void) = 0;
	/// ���ͽ��뵽��һ�α�����Ϣ
	virtual void SendEnterNextMatching(void) = 0;
	/// �ر�����
	virtual void CloseSocket(void) = 0;
	/// �Ƿ�������
	virtual bool IsConnected(void) = 0;
	/// ����ָ���ķ�����
	virtual bool Connect(std::string ipaddress,int port) = 0;

	/// �ַ�������ת��
	virtual std::wstring ConverToWideChar(const std::string& str) = 0;
	virtual std::string ConverToMultiChar(const std::wstring& str) = 0;
	virtual CString Utf8ConverToWideChar(const std::string& str) = 0;
	virtual std::string WideCharConverToUtf8(CString& str) = 0;
	/// �����û�IP��ַ�õ��û�ʵ�ʵ���λ��
	virtual CString GetIPPosition(unsigned __int32 wIPPosition) = 0;
	/// ���ù�������
	virtual void SetCinematicsType(int type) = 0;

	/// ��ʼ¼��
	virtual void StartVideoTape(Player *pPlayer,Room *pRoom)=NULL;
	/// ����¼��
	virtual void EndVideoTape(void)=NULL;
	/// �õ���ǰ״̬
	virtual VideoState GetVideoState(void) = NULL;
	//��¼��Ϸ��Ϣ
	virtual void OnTapeGameMessage(const void * pBuffer, WORD wDataSize)=NULL;
};

#endif