#ifndef _MGS_STDVLPR_COMMONDEFINE_H_
#define _MGS_STDVLPR_COMMONDEFINE_H_

// 日志等级设定，等级越高，输出内容越少

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

//设备返回状态码
typedef enum
{
	RET_SUCCESS = 0,                          
	ERROR_DEVICE_NO_RESPONSE=-100,    //设备无响应，网络断开
	ERROR_INVAILID_PARAM=-1000,       //无效参数
	ERROR_DEVICE_OCCUPY=-1001,        //设备被占用
	ERROR_OPEN_FAIL=-1002,            //打开错误
	ERROR_OTHER=-1003                 //其他错误

}EnumDevError;

// 设备参数
typedef struct _mgs_stdvlpr_dev_info
{
	char				szDevIp[64];
	int					nPort;
	char				szUser[64];
	char				szPwd[64];
	char                szFactory[32];
	int                 nConnType;              //0网络连接，1串口
	char                reserved[12];
}mgs_stdvlpr_dev_info;

typedef struct  _mgs_stdvlpr_info
{
	int nPlaceNo;			          //抓拍识别图片中车辆行驶的车道号，从1开始按照由中置车道向应急车道方向按序递增，无法获取填：0                           
	char time[20];                    //识别时间，格式“yyyyMMddHHmmss” 
	int nIsTail;                      //是否车尾车牌,0:车头车牌，1:车尾车牌，无法识别：0
	int vehType;		           	//车型，1：一型客车，2：二型客车，3：三型客车，4：四型客车；
	                                //      11:一型货车，12：二型货车，13：三型货车，14：四型货车，15：五型货车,16：六型货车；
	                                //      21:一型专项作业车，22：二型专项作业车，23：三型专项作业车，24：四型专项作业车，25：五型专项作业车,26：六型专项作业车；无法识别：0
	unsigned char plateColor[2];	//车牌颜色(数字编码)
	                                //“00”蓝色，“01”黄色，“02”黑色，“03”白色
	                                //“04”渐绿，“05”黄绿，“06”蓝白，“09”未确定，“11”绿色，“12”红色，无法识别：“09”
	unsigned char plateText[16];	//车牌文字，GBK编码       //无法识别，默A00000
	unsigned int reliability;     //识别车牌可信度(采用四位表示9999表示为99.99%)
	unsigned int imageLength[3];   //识别图片长度
	                                //[0]=场景图长度，[1]=车牌图长度，[2]=二值化图长度，无车牌或二值化图对应长度填：0
	unsigned char* image[3];	   //识别图片：[0]=场景图，[1]=车牌图，[2]=二值化图
	unsigned int vehSpeed;        //车速
	unsigned int vehBodyColorNo;  //车体颜色编号 
	                              //1-白，2-灰，3-黄，4-粉，5-红
	                               //6-紫，7-绿，8-蓝，9-棕，10-黑，0-其他
	unsigned int vehBodyDeepNo;   //车体颜色深度编号：0-浅色，1-深色
	unsigned int vehClassTypeNo;       //车辆类型编号 
	                                  //1-客车，2-货车，3-牵引车，4-专项作业车
	                                  //5-电车，6-摩托车，7-三轮汽车，8-拖拉机
	                                  //9-轮式机械，10-全挂车，11-半挂车，0-其他
	unsigned int plateTypeNo;      //车牌类型编号 
	                               //0-其他，1-黄底黑字，2-蓝底白字，3-黑底白字
                                	//4-黑底红字，5-白底红字，6-白底黑字

	char  szCarBrand[16];           //车辆品牌

	char                reserved[16];
} mgs_stdvlpr_info;

typedef struct _mgs_stdvlpr_ntp_info
{
	char				szNtpServeIp[64];
	int					nNtpServerPort;
	int                 nTimeInvl;          //校时周期，单位秒

}mgs_stdvlpr_ntp_info;

//抓拍结果回调函数
typedef int ( * fStdVLPRRegResultCallback)(int ,mgs_stdvlpr_info* ,void* );
//设备状态回调函数
typedef int ( * fStdVLPRDevStatusCallback)(int  ,int , void* );


#ifndef MAX_PATH
#define MAX_PATH (256)
#endif

#ifndef GET_LAST_ERROR
#define GET_LAST_ERROR (0)
#endif

#endif
