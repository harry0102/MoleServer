#ifndef _DEFINES_H_INCLUDE_
#define _DEFINES_H_INCLUDE_

#include <string>

#define IDD_MESSAGE_HEART_BEAT                  100                   // ������Ϣ

/*********************************************************************************************/

#define IDD_MESSAGE_REGISTER_GAME               200                   // ע����Ϸ
#define IDD_MESSAGE_REGISTER_SUCCESS            201                   // ע����Ϸ�ɹ�
#define IDD_MESSAGE_REGISTER_FAIL               202                   // ע����Ϸʧ��
#define IDD_MESSAGE_RE_REGISTER                 203                   // �ظ�ע��
#define IDD_MESSAGE_REGISTER_EXIST              204                   // ����������

/*********************************************************************************************/

#define IDD_MESSAGE_CONNECT                     300                   // �û�������Ϣ
#define IDD_MESSAGE_CONNECT_SUCESS              301                   // �û����ӳɹ�
#define IDD_MESSAGE_CONNECT_FAIL                302                   // �û�����ʧ��
#define IDD_MESSAGE_CONNECT_EXIST               303                   // �û����Ӵ���

/*********************************************************************************************/

#define IDD_MESSAGE_CENTER_LOGIN                400                   // �û���¼��Ϣ
#define IDD_MESSAGE_CENTER_LOGIN_SUCESS         401                   // �û���¼�ɹ�
#define IDD_MESSAGE_CENTER_LOGIN_FAIL           402                   // �û���¼ʧ��
#define IDD_MESSAGE_CENTER_LOGIN_BUSY           403                   // ϵͳæ,�û���¼����Ƶ��
#define IDD_MESSAGE_CENTER_BANK                 404                   // ���в���
#define IDD_MESSAGE_CENTER_BANK_GET_MONEY       405                   // �õ��û���Ǯ
#define IDD_MESSAGE_CENTER_BANK_TRANSFER        406                   // ��ȡ����
#define IDD_MESSAGE_CENTER_BANK_UPDATEBANKPWD   407                   // �޸���������
#define IDD_MESSAGE_CENTER_BANK_SUCESS          408                   // ���в����ɹ�
#define IDD_MESSAGE_CENTER_BANK_FAIL            409                   // ���в���ʧ��
#define IDD_MESSAGE_CENTER_UPDATEUSER           410                   // �����Ϣ����
#define IDD_MESSAGE_CENTER_UPDATEUSER_SUCESS    411                   // �����Ϣ�����ɹ�
#define IDD_MESSAGE_CENTER_UPDATEUSER_FAIL      412                   // �����Ϣ����ʧ��
#define IDD_MESSAGE_CENTER_UPDATEUSER_INFO      413                   // ���������Ϣ
#define IDD_MESSAGE_CENTER_UPDATEUSER_PWD       414                   // ������ҵ�½����
#define IDD_MESSAGE_CENTER_LOGIN_BAN            415                   // �˺ű���
#define IDD_MESSAGE_CENTER_BANK_TRANSFERACCOUNT 416                   // ����ת��
#define IDD_MESSAGE_CENTER_GETGAMEONLINECOUNT   417                   // ��ȡָ����Ϸ����������
#define IDD_MESSAGE_CENTER_LOCKMACHINE          418                   // ��������
#define IDD_MESSAGE_CENTER_LOCKMACHINE_SUC      419                   // ���������ɹ�
#define IDD_MESSAGE_CENTER_LOCKMACHINE_FAI      420                   // ��������ʧ��
#define IDD_MESSAGE_CENTER_BANK_FAIL_LASTMONEY  421                   // ת�˽�����ڹ涨ֵ
#define IDD_MESSAGE_CENTER_BANK_FAIL_LASTCOUNT  422                   // һ��ֻ��ת3��
#define IDD_MESSAGE_CENTER_SIGNIN               423                   // ÿ��ǩ��
#define IDD_MESSAGE_CENTER_SINGIN_GETMES        424                   // �õ�ǩ����Ϣ
#define IDD_MESSAGE_CENTER_SINGIN_START         425                   // ��ʼǩ��
#define IDD_MESSAGE_CENTER_SINGIN_SUCESS        426                   // ǩ���ɹ�
#define IDD_MESSAGE_CENTER_SINGIN_FAIL          427                   // ǩ��ʧ��
#define IDD_MESSAGE_CENTER_BUYGOODS             428                   // ����Ʒ
#define IDD_MESSAGE_CENTER_WEEKSIGNIN           429                   // ÿ��ǩ��
#define IDD_MESSAGE_CENTER_WEEKSINGIN_GETMES    430                   // ��ȡÿ�ܵ�ǩ����Ϣ
#define IDD_MESSAGE_CENTER_BANK_CARDCHONGZHI    431                   // �㿨��ֵ
#define IDD_MESSAGE_CENTER_BANK_CARDCZ_SUC      432                   // �㿨��ֵ�ɹ�
#define IDD_MESSAGE_CENTER_BANK_CARDCZ_FAIL     433                   // �㿨��ֵʧ��
#define IDD_MESSAGE_CHAT_SENDMSG                434                   // ����������Ϣ
#define IDD_MESSAGE_CHAT_SENDHONGBAO            435                   // ���ͺ��
#define IDD_MESSAGE_CHAT_SENDHONGBAO_SUC        436                  // ���ͺ���ɹ�
#define IDD_MESSAGE_CHAT_SENDHONGBAO_FAIL       437                   // ���ͺ��ʧ��
#define IDD_MESSAGE_CHAT_SENDHONGBAO_GET        438                   // �����
#define IDD_MESSAGE_CHAT_SENDHONGBAO_GET_SUC    439                   // ������ɹ�
#define IDD_MESSAGE_CHAT_SENDHONGBAO_GET_FAIL   440                   // �����ʧ��
#define IDD_MESSAGE_CENTER_BANK_OTHERCHONGZHI   441                   // ������ֵ
#define IDD_MESSAGE_CENTER_BANK_OTHERCZ_FAIL    442                   // ������ֵʧ��
#define IDD_MESSAGE_CENTER_BANK_OTHERCZ_SUC     443                   // ������ֵ�ɹ�
#define IDD_MESSAGE_CENTER_BANK_GETDUIHUANRATE  444                   // �õ���Ҷһ�����
#define IDD_MESSAGE_CENTER_BANK_STARTDUIHUAN    445                   // ��ʼ��Ҷһ�
#define IDD_MESSAGE_CENTER_BANK_GETCHONGZHIRATE 446                   // �õ���ֵ����
#define IDD_MESSAGE_CHAT_SHISHIGUANGGAO         447                   // ��Ϸʵʱ���
#define IDD_MESSAGE_CHAT_SHISHIGUANGGAO_GET     448                   // �õ��������
#define IDD_MESSAGE_CHAT_SHISHIGUANGGAO_SEND    449                   // ���͹��
#define IDD_MESSAGE_CENTER_LOGIN_BANLOGIN       450                   // ��������ֹ��¼
#define IDD_MESSAGE_GAME_LOGIN_USERBANLOGIN     451                   // ����˺ű���

/*********************************************************************************************/

#define IDD_MESSAGE_GAME_LOGIN                  500                   // �û���¼��Ϣ
#define IDD_MESSAGE_GAME_LOGIN_SUCESS           501                   // �û���¼�ɹ�
#define IDD_MESSAGE_GAME_LOGIN_FAIL             502                   // �û���¼ʧ��
#define IDD_MESSAGE_GAME_LOGIN_BUSY             503                   // ϵͳæ,�û���¼����Ƶ��
#define IDD_MESSAGE_GAME_LOGIN_EXIST            504                   // �û��Ѿ���ϵͳ��
#define IDD_MESSAGE_GAME_LOGIN_FULL             505                   // ��������
#define IDD_MESSAGE_GAME_LOGIN_CLOSE_SERVER     506                   // �رյ�ǰ������
#define IDD_MESSAGE_GAME_LOGIN_MATCHING_NOSTART 507                   // ����δ��ʼ
#define IDD_MESSAGE_GAME_LOGIN_MATCHING_NOSCROE 508                   // û�дﵽ��������Ҫ��Ļ���
#define IDD_MESSAGE_GAME_LOGIN_MATCHING_NOLEVEL 509                   // û�дﵽ��������Ҫ��ĵȼ�
#define IDD_MESSAGE_GAME_LOGIN_BANLOGIN         510                   // ����������
#define IDD_MESSAGE_GAME_LOGIN_USERBANLOGIN     511                   // ����˺ű���

/*********************************************************************************************/

#define IDD_MESSAGE_UPDATE_GAME_SERVER          600                   // ������Ϸ��������Ϣ
#define IDD_MESSAGE_UPDATE_GAME_SUCCESS         601                   // ������Ϸ��������Ϣ�ɹ�
#define IDD_MESSAGE_UPDATE_GAME_FAIL            602                   // ������Ϸ��������Ϣʧ��

/*********************************************************************************************/

#define IDD_MESSAGE_USER_REGISTER               700                   // �û�ע��
#define IDD_MESSAGE_USER_REGISTER_SUCCESS       701                   // �û�ע��ɹ�
#define IDD_MESSAGE_USER_REGISTER_FAIL          702                   // �û�ע��ʧ��
#define IDD_MESSAGE_SUPER_BIG_MSG               703                   // �㲥��Ϣ

/*********************************************************************************************/

#define IDD_MESSAGE_GET_GAMESERVER              800                   // �õ���Ϸ�������б�
#define IDD_MESSAGE_GET_GAMEINFO                801                   // �õ���Ϸ��Ϣ�б�
#define IDD_MESSAGE_GET_GAMEINFO_SUCCESS        802                   // �õ���Ϸ��Ϣ�б�ɹ�
#define IDD_MESSAGE_GET_GAMEINFO_FAIL           803                   // �õ���Ϸ��Ϣ�б�ʧ��

/*********************************************************************************************/

#define IDD_MESSAGE_FRAME                       900                   // �����Ϣ
#define IDD_MESSAGE_ENTER_ROOM                  901                   // ������Ϸ����
#define IDD_MESSAGE_LEAVE_ROOM                  902                   // �뿪��Ϸ����
#define IDD_MESSAGE_REENTER_ROOM                903                   // �û����ߺ����»ص���Ϸ����
#define IDD_MESSAGE_CHANGER_ROOM                904                   // ����Ϸ����
#define IDD_MESSAGE_FALLLINE_ROOM               905                   // �û�����Ϸ�����е���
#define IDD_MESSAGE_GET_ROOM_LIST               906                   // �õ������б�
#define IDD_MESSAGE_LOOKON_ENTER_ROOM           907                   // �Թ۽�����Ϸ����
#define IDD_MESSAGE_GET_ONLINE_PLAYERS          908                   // �õ���ǰ�����������
#define IDD_MESSAGE_LEAVE_SERVER                909                   // �뿪��Ϸ������
#define IDD_MESSAGE_GET_ONLINE_PLAYERS_END      910                   // ��ȡ������ҽ���
#define IDD_MESSAGE_GET_ROOM_LIST_END           911                   // ��ȡ�����б����
#define IDD_MESSAGE_ENTER_ROOM_FAIL             912                   // ������Ϸ����ʧ��
#define IDD_MESSAGE_ENTER_ROOM_SUCC             913                   // ������Ϸ����ɹ�
#define IDD_MESSAGE_ENTER_ROOM_LASTMONEY        914                   // ��Ҳ������㷿��Ҫ��
#define IDD_MESSAGE_READY_START                 915                   // ���׼����ʼ��Ϸ
#define IDD_MESSAGE_GAME_START                  916                   // ��Ϸ��ʼ
#define IDD_MESSAGE_GAME_END                    917                   // ��Ϸ����
#define IDD_MESSAGE_UPDATE_USER_DATA            918                   // �����û���Ϣ
#define IDD_MESSAGE_USER_CHAT                   919                   // �û�������Ϣ
#define IDD_MESSAGE_ENTER_ROOM_EXIST            920                   // �Ѿ�����һ����Ϸ����
#define IDD_MESSAGE_UPDATE_USER_MONEY           921                   // �����û����
#define IDD_MESSAGE_FRAME_SUPER_KICK_PLAYER     922                   // ��ָ��������߲����
#define IDD_MESSAGE_FRAME_SUPER_BIG_MSG         923                   // ��������Ϣ
#define IDD_MESSAGE_FRAME_ROOM_UNDERFULL        924                   // δ��������ʾ��Ϣ
#define IDD_MESSAGE_FRAME_MATCH                 925                   // ������Ϣ
#define IDD_MESSAGE_FRAME_MATCH_START           926                   // ������ʼ��Ϣ
#define IDD_MESSAGE_FRAME_MATCH_OVER            927                   // ����������Ϣ
#define IDD_MESSAGE_FRAME_MATCH_GETRAKING       928                   // ��ȡ��������
#define IDD_MESSAGE_FRAME_MATCH_CONTINUE        929                   // ��������
#define IDD_MESSAGE_FRAME_MATCH_WAITINGCOUNT    930                   // �����ȴ�����
#define IDD_MESSAGE_ENTER_ROOM_FULL             931                   // ������Ϸ����ʧ��,��������
#define IDD_MESSAGE_FRAME_MATCH_TIME            932                   // ���ĵ�ǰ����ʱ��
#define IDD_MESSAGE_FRAME_ROOM_SITDOWN          933                   // ����
#define IDD_MESSAGE_FRAME_ROOM_STANDUP          934                   // ����
#define IDD_MESSAGE_FRAME_JACKPOT               935                   // ������Ϣ
#define IDD_MESSAGE_FRAME_JACKPOT_REFRESH       936                   // ���زʽ�ˢ��
#define IDD_MESSAGE_FRAME_JACKPOT_WINTIP        937                   // ���ػ���ʾ
#define IDD_MESSAGE_FRAME_ROOM_JOINSET          938                   // �����������
#define IDD_MESSAGE_FRAME_MATCH_GETPRIZE        939                   // �õ�������Ʒ
#define IDD_MESSAGE_FRAME_ENTER_MATCHINGTIP     940                   // ��������ʾ
#define IDD_MESSAGE_ENTER_ROOM_BAOMING_SUC      941                   // �����ɹ�
#define IDD_MESSAGE_ENTER_ROOM_TUISAI_SUC       942                   // �����ɹ�
#define IDD_MESSAGE_PERSONROOM_ENTER_ROOM_FAIL  943                   // ˽����Ϸ�������ʧ��

/*******************************************��Ϸ��Ϣ**************************************************/

#define IDD_MESSAGE_ROOM					    1000                  // ��Ϸ������Ϣ

#endif
