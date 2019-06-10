#ifndef _MGS_STDVLPR_COMMONDEFINE_H_
#define _MGS_STDVLPR_COMMONDEFINE_H_

// ��־�ȼ��趨���ȼ�Խ�ߣ��������Խ��

#define MGSSTDVLPR_LEVEL_TRACE  -1
#define MGSSTDVLPR_DEBUG  0
#define MGSSTDVLPR_INFO   1
#define MGSSTDVLPR_WARN   2
#define MGSSTDVLPR_ERROR  3
#define MGSSTDVLPR_FATA   4

#ifdef WIN32
#define MGS_STDVLPR_DEVICE_CALL __stdcall
#else
#define MGS_STDVLPR_DEVICE_CALL
#endif

//�豸����״̬��
typedef enum
{
	RET_SUCCESS = 0,                          
	ERROR_DEVICE_NO_RESPONSE=-100,    //�豸����Ӧ������Ͽ�
	ERROR_INVAILID_PARAM=-1000,       //��Ч����
	ERROR_DEVICE_OCCUPY=-1001,        //�豸��ռ��
	ERROR_OPEN_FAIL=-1002,            //�򿪴���
	ERROR_OTHER=-1003                 //��������

}EnumDevError;

// �豸����
typedef struct _mgs_stdvlpr_dev_info
{
	char				szDevIp[64];
	int					nPort;
	char				szUser[64];
	char				szPwd[64];
	char                szFactory[32];
	int                 nConnType;              //0�������ӣ�1����
	char                reserved[12];
}mgs_stdvlpr_dev_info;

typedef struct  _mgs_stdvlpr_info
{
	int nPlaceNo;			          //ץ��ʶ��ͼƬ�г�����ʻ�ĳ����ţ���1��ʼ���������ó�����Ӧ������������������޷���ȡ�0                           
	char time[20];                    //ʶ��ʱ�䣬��ʽ��yyyyMMddHHmmss�� 
	int nIsTail;                      //�Ƿ�β����,0:��ͷ���ƣ�1:��β���ƣ��޷�ʶ��0
	int vehType;		           	//���ͣ�1��һ�Ϳͳ���2�����Ϳͳ���3�����Ϳͳ���4�����Ϳͳ���
	                                //      11:һ�ͻ�����12�����ͻ�����13�����ͻ�����14�����ͻ�����15�����ͻ���,16�����ͻ�����
	                                //      21:һ��ר����ҵ����22������ר����ҵ����23������ר����ҵ����24������ר����ҵ����25������ר����ҵ��,26������ר����ҵ�����޷�ʶ��0
	unsigned char plateColor[2];	//������ɫ(���ֱ���)
	                                //��00����ɫ����01����ɫ����02����ɫ����03����ɫ
	                                //��04�����̣���05�����̣���06�����ף���09��δȷ������11����ɫ����12����ɫ���޷�ʶ�𣺡�09��
	unsigned char plateText[16];	//�������֣�GBK����       //�޷�ʶ��ĬA00000
	unsigned int reliability;     //ʶ���ƿ��Ŷ�(������λ��ʾ9999��ʾΪ99.99%)
	unsigned int imageLength[3];   //ʶ��ͼƬ����
	                                //[0]=����ͼ���ȣ�[1]=����ͼ���ȣ�[2]=��ֵ��ͼ���ȣ��޳��ƻ��ֵ��ͼ��Ӧ�����0
	unsigned char* image[3];	   //ʶ��ͼƬ��[0]=����ͼ��[1]=����ͼ��[2]=��ֵ��ͼ
	unsigned int vehSpeed;        //����
	unsigned int vehBodyColorNo;  //������ɫ��� 
	                              //1-�ף�2-�ң�3-�ƣ�4-�ۣ�5-��
	                               //6-�ϣ�7-�̣�8-����9-�أ�10-�ڣ�0-����
	unsigned int vehBodyDeepNo;   //������ɫ��ȱ�ţ�0-ǳɫ��1-��ɫ
	unsigned int vehClassTypeNo;       //�������ͱ�� 
	                                  //1-�ͳ���2-������3-ǣ������4-ר����ҵ��
	                                  //5-�糵��6-Ħ�г���7-����������8-������
	                                  //9-��ʽ��е��10-ȫ�ҳ���11-��ҳ���0-����
	unsigned int plateTypeNo;      //�������ͱ�� 
	                               //0-������1-�Ƶ׺��֣�2-���װ��֣�3-�ڵװ���
                                	//4-�ڵ׺��֣�5-�׵׺��֣�6-�׵׺���

	char  szCarBrand[16];           //����Ʒ��

	char                reserved[16];
} mgs_stdvlpr_info;

typedef struct _mgs_stdvlpr_ntp_info
{
	char				szNtpServeIp[64];
	int					nNtpServerPort;
	int                 nTimeInvl;          //Уʱ���ڣ���λ��

}mgs_stdvlpr_ntp_info;

//ץ�Ľ���ص�����
typedef int ( * fStdVLPRRegResultCallback)(int ,mgs_stdvlpr_info* ,void* );
//�豸״̬�ص�����
typedef int ( * fStdVLPRDevStatusCallback)(int  ,int , void* );


#ifndef MAX_PATH
#define MAX_PATH (256)
#endif

#ifndef GET_LAST_ERROR
#define GET_LAST_ERROR (0)
#endif

#endif
