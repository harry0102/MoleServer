#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#include "cdefines.h"
#include <map>
#include <string>

//////////////////////////////////////////////////////////////////////////

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

//��Ϸ�߼�
class CGameLogic
{
	//��������
private:
	static const uint8				m_cbCardListData[52*8];				//�˿˶���

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���ͺ���
public:
	//��ȡ��ֵ
	uint8 GetCardValue(uint8 cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//��ȡ��ɫ
	uint8 GetCardColor(uint8 cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }
	uint8 GetCardLogicValue(uint8 cbCardData)
	{
		return GetCardValue(cbCardData);
		////�˿�����
		//uint8 cbCardColor=GetCardColor(cbCardData);
		//uint8 cbCardValue=GetCardValue(cbCardData);

		////ת����ֵ
		//if (cbCardColor==4) return cbCardValue+2;
		//return (cbCardValue<=2)?(cbCardValue+13):cbCardValue;
	}

	//���ƺ���
public:
	//�����˿�
	void RandCardList(uint8 cbCardBuffer[], int cbBufferCount);
	//�������͵õ���
	tagCardType GetCardByResultType(tagCardType pResultType,uint8 *firstcard,uint8 *secondcard);
};

//////////////////////////////////////////////////////////////////////////

#endif
