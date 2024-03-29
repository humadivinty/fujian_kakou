#ifndef CAMERRESULT
#define CAMERRESULT
#include <string>
#include<memory.h>

#include"commondef.h"

/* 识别结果大图类型定义 */
#define RECORD_BIGIMG_BEST_SNAPSHOT			0x0001	/**< 最清晰识别图 */
#define RECORD_BIGIMG_LAST_SNAPSHOT			0x0002	/**< 最后识别图 */
#define RECORD_BIGIMG_BEGIN_CAPTURE			0x0003	/**< 开始抓拍图 */
#define RECORD_BIGIMG_BEST_CAPTURE			0x0004	/**< 最清晰抓拍图 */
#define RECORD_BIGIMG_LAST_CAPTURE			0x0005	/**<  最后抓拍图 */

#define  MAX_IMG_SIZE 10*1024*1024
#define COMPRESS_BIG_IMG_SIZE 150*1024
#define COMPRESS_PLATE_IMG_SIZE 50*1024

//车种类型
#define UNKOWN_TYPE 0
#define BUS_TYPE_1 1
#define BUS_TYPE_2 2
#define BUS_TYPE_3 3
#define BUS_TYPE_4 4
#define BUS_TYPE_5 5
#define TRUCK_TYPE_1 11
#define TRUCK_TYPE_2 12
#define TRUCK_TYPE_3 13
#define TRUCK_TYPE_4 14
#define TRUCK_TYPE_5 15

//车辆大小类型
#define CAR_TYPE_SIZE_SMALL 1
#define CAR_TYPE_SIZE_MIDDLE 6
#define CAR_TYPE_SIZE_BIG 2
#define CAR_TYPE_SIZE_UNKNOWN 0

//行驶方向
#define DIRECTION_UP_TO_DOWN 0      //下行
#define DIRECTION_DOWN_TO_UP 1      //上行
#define DIRECTION_EAST_TO_WEST 2    //由东向西
#define DIRECTION_WEST_TO_EAST 3    //由西向东
#define DIRECTION_SOUTH_TO_NORTH 4    //由南向北
#define DIRECTION_NORTH_TO_SOUTH 5    //由北向南

//车牌颜色类型
#define  COLOR_UNKNOW (9)
#define  COLOR_BLUE 0
#define  COLOR_YELLOW 1
#define  COLOR_BLACK 2
#define  COLOR_WHITE 3
#define  COLOR_GRADIENT_CREEN 4
#define  COLOR_YELLOW_GREEN 5
#define  COLOR_BLUE_WHITE 6
#define  COLOR_RED 7
#define  COLOR_GREEN 8
#define  COLOR_GREY 10
#define  COLOR_PINK 11
#define  COLOR_PUEPLE 12
#define  COLOR_BROWN 13

//车牌颜色类型
#define  PLATE_TYPE_NONE 0      //0-无类型
#define  PLATE_TYPE_NORMAL 1      //1-92式民用车牌类型
#define  PLATE_TYPE_POLICE 2      //2-警用车牌类型
#define  PLATE_TYPE_UP_DOWN_ARMY 3      //3-上下军车（现在没有这种车牌类型）
#define  PLATE_TYPE_ARMED_POLICE 4      //4-92式武警车牌类型(现在没有这种车牌类型)
#define  PLATE_TYPE_LEFT_RIGHT_ARMY 5      //5-左右军车车牌类型(一行结构)
//#define  PLATE_TYPE_NONE 6      //
#define  PLATE_TYPE_CUSTOM 7      //7-02式个性化车牌类型
#define  PLATE_TYPE_DOUBLE_YELLOW_LINE 8      //8-黄色双行尾牌（货车或公交车尾牌）
#define  PLATE_TYPE_NEW_ARMY 9      //9-04式新军车类型(两行结构)
#define  PLATE_TYPE_EMBASSY 10      //10-使馆车牌类型
#define  PLATE_TYPE_ONE_LINE_ARMED_POLICE 11      //11-一行结构的新WJ车牌类型
#define  PLATE_TYPE_TWO_LINE_ARMED_POLICE  12      //12-两行结构的新WJ车牌类型
#define  PLATE_TYPE_YELLOW_1225_FARMER 13      //13-黄色1225农用车
#define  PLATE_TYPE_GREEN_1325_FARMER 14      //14-绿色1325农用车
#define  PLATE_TYPE_YELLOW_1325_FARMER 15      //15-黄色1325农用车
#define  PLATE_TYPE_MOTORCYCLE 16      //16-摩托车
#define  PLATE_TYPE_NEW_ENERGY 17      //17-新能源

#define CAR_BODY_LIGHTCOLOR 0       //浅色
#define CAR_BODY_DARKCOLOR 1        //深色


#define CAR_FRONT_MODE 0        //车头
#define CAR_TAIL_MODE 1       //车尾

class CameraIMG
{
public:
	CameraIMG();
	CameraIMG(const CameraIMG& CaIMG);
	~CameraIMG();

    unsigned int wImgWidth;
    unsigned int wImgHeight;
    unsigned long dwImgSize;
    unsigned int  wImgType;
	char chSavePath[256];
    unsigned char* pbImgData;

	CameraIMG& operator = (const CameraIMG& CaIMG);
};

class CameraResult
{
public:

	CameraResult();
	CameraResult(const CameraResult& CaRESULT);
	~CameraResult();

    float fVehLenth;			//车长
    float fDistanceBetweenAxles;		//轴距
    float fVehHeight;		//车高
    float fVehWidth;    //che gao
    float fConfidenceLevel;     //可信度

    unsigned long long dw64TimeMS;
    unsigned int dwCarID;
    int iDeviceID;
    int iPlateColor;
    int iPlateTypeNo;
    int iSpeed;
    int iResultNo;
    int iVehTypeNo;		//车型代码
    int iVehClassType;      //车辆类型编号 1-客车，2-货车，3-牵引车，4-专项作业车    //5-电车，6-摩托车，7-三轮汽车，8-拖拉机    //9-轮式机械，10-全挂车，11-半挂车，0-其他
    int iVehSizeType;       //车型大小代码
    int iVehBodyColorNo;
    int iVehBodyDeepNo;
    int iVehLogoType;
    int iAreaNo;
    int iRoadNo;
    int iLaneNo;
    int iDirection;
    int iWheelCount;		//轮数
    int iAxletreeCount;		//轴数
    int iAxletreeGroupCount;//轴组数
    int iAxletreeType;		//轴型
    int iReliability;       //可信度

    bool bBackUpVeh;		//是否倒车
    bool bFrontMode;		//车头模式

    char chDeviceIp[64];
    char chServerIP[64];
	char chPlateNO[64];
	char chPlateColor[64];
	char chListNo[256];
	char chPlateTime[256];
	char chSignStationID[256];
	char chSignStationName[256];
    char chSignDirection[256];
    char chDeviceID[256];
    char chLaneID[256];
    char chCarFace[256];
    char chChileLogo[256];
    char pcAppendInfo[10240];

    //std::string strAppendInfo;

	CameraIMG CIMG_BestSnapshot;	/**< 最清晰识别图 */
	CameraIMG CIMG_LastSnapshot;	/**< 最后识别图 */
	CameraIMG CIMG_BeginCapture;	/**< 开始抓拍图 */
	CameraIMG CIMG_BestCapture;		/**< 最清晰抓拍图 */
	CameraIMG CIMG_LastCapture;		/**< 最后抓拍图 */
	CameraIMG CIMG_PlateImage;		/**< 车牌小图 */
	CameraIMG CIMG_BinImage;			/**< 车牌二值图 */

	CameraResult& operator = (const CameraResult& CaRESULT);

    bool SerializationToDisk(const char* chFilePath);
    bool SerializationFromDisk(const char* chFilePath);
};

typedef struct _tagSafeModeInfo
{
	int iEableSafeMode;
    int index;
    int DataInfo;
    char chBeginTime[256];
    char chEndTime[256];

	_tagSafeModeInfo()
	{
		iEableSafeMode = 0;
		memset(chBeginTime, 0, sizeof(chBeginTime));
		memset(chEndTime, 0, sizeof(chEndTime));
		index = 0;
		DataInfo = 0;
	}
}_tagSafeModeInfo;

#endif
