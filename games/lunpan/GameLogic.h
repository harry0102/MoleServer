#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#include "cdefines.h"
#include <map>
#include <string>

/*
 * Ѻע����
 */
enum YanZhuType
{
	TYPE_ZHIJIEZHU = 0,                      // ֱ��ע
	TYPE_FENZHU,                             // ��ע
	TYPE_JIEZHU,                             // ��ע
	TYPE_SANSHU,                             // ����
	TYPE_JIAOZHU,                            // ��ע
	TYPE_SIGEHAOMA,                          // 4������
	TYPE_XIANZHU,                            // ��ע
	TYPE_LIEZHU,                             // ��ע
	TYPE_XIAZHUYIDASHU,                      // ��עһ����
	TYPE_HONGHEI,                            // ���
	TYPE_DANSHUANG,                          // ��˫
	TYPE_DAXIAO,                             // ��С
	TYPE_NULL
};

/// �������͵õ�����
std::string GetNameByTypeMe(YanZhuType type);

/**
 * ������ע���ͣ���ע����ֵ������עֵ
 */
int getXiaZhuNumbers(YanZhuType type,int index,int *aarray);

/*
 * ������
 */
void chaijiechouma(int64 money,std::map<int,int> &pchouma);

/**
 * ���ָ��ֵ�Ƿ����Ǹ�����
 */
int GetXiazhuType(YanZhuType type,int index);
int GetXiaZhuAreaCount(YanZhuType type);
int GetXiaZhuArea(YanZhuType type,int index);

/*
 * �������е���ע
 */
class CXiaZhuManager
{
public:
	/// ���캯��
	CXiaZhuManager() { }
	/// ��������
	~CXiaZhuManager() { }

	/// ����ָ���������ע��
	inline void setXiaZhu(YanZhuType pYanZhuType,int index,int64 money)
	{
		m_xiazhus[(int)pYanZhuType][index] = money;
	}
	/// �ۼ�ָ���������ע��
	inline void addXiaZhu(YanZhuType pYanZhuType,int index,int64 money)
	{
		m_xiazhus[(int)pYanZhuType][index] += money;
	}
	/// ��ָ���������ע��
	inline void decXiaZhu(YanZhuType pYanZhuType,int index,int64 money)
	{
		if(m_xiazhus[(int)pYanZhuType][index] - money >= 0)
			m_xiazhus[(int)pYanZhuType][index] -= money;
	}
	/// ����ָ��������ע��Ϊ0
	inline void zeroXiaZhu(YanZhuType pYanZhuType,int index)
	{
		m_xiazhus[(int)pYanZhuType][index] = 0;
	}
	/// �õ�ָ���������ע��
	inline int64 getXiaZhu(YanZhuType pYanZhuType,int index)
	{
		return m_xiazhus[(int)pYanZhuType][index];
	}
	/// �õ�ָ�����������ע��
	inline int64 getXiaZhuBigarea(YanZhuType pYanZhuType)
	{
		int64 ptempMoney = 0;

		std::map<int,int64>::iterator iter = m_xiazhus[(int)pYanZhuType].begin();
		for(;iter != m_xiazhus[(int)pYanZhuType].end();++iter)
			ptempMoney+=(*iter).second;

		return ptempMoney;
	}
	/// �õ����е���ע
	inline std::map<int,std::map<int,int64> >& getXiaZhus(void)
	{
		return m_xiazhus;
	}
	/// �õ���ע����
	inline int getXiaZhuCount(void)
	{
		int pTotal = 0;

		std::map<int,std::map<int,int64> >::iterator iter = m_xiazhus.begin();
		for(;iter != m_xiazhus.end();++iter)
		{
			pTotal+=(int)(*iter).second.size();
		}

		return pTotal;
	}
	/// �õ�ָ�����͵���ע
	inline std::map<int,int64>& getXiaZhuByType(YanZhuType ptype)
	{
		return m_xiazhus[(int)ptype];
	}
	/// ���
	inline void clear(void)
	{
		m_xiazhus.clear();
		m_xiazhubeilv.clear();
	}
	/// ���㵱ǰ����ܵ���ע
	inline int64 getTotalXiaZhu(void)
	{
		int64 pTotal = 0;

		std::map<int,std::map<int,int64> >::iterator iter = m_xiazhus.begin();
		for(;iter != m_xiazhus.end();++iter)
		{
			std::map<int,int64>::iterator iter2 = (*iter).second.begin();
			for(;iter2 != (*iter).second.end();++iter2)
				pTotal+=(*iter2).second;
		}

		return pTotal;
	}

	//������ע����
	inline void setXiaZhuBeilv(YanZhuType pYanZhuType,int beilv)
	{
		m_xiazhubeilv[(int)pYanZhuType] = beilv;
	}
	/// �õ�ָ������ı���
	inline int getXiaZhuBeilv(YanZhuType pYanZhuType)
	{
		return m_xiazhubeilv[(int)pYanZhuType];
	}

private:
	std::map<int,std::map<int,int64> > m_xiazhus;
	std::map<int,int > m_xiazhubeilv;
};

//////////////////////////////////////////////////////////////////////////

#endif
