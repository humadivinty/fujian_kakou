#ifndef MGS_STDVLPR_FJ_H
#define MGS_STDVLPR_FJ_H

//#include "mgs_stdvlpr_fj_global.h"

//#ifndef _MGS_STDVLPR_DEVICE_LIB_
//#define _MGS_STDVLPR_DEVICE_LIB_

#include "CommonDef.h"

// 定义函数调用方式.
//#ifdef WIN32
//#define MGS_STDVLPR_DEVICE_CALL __stdcall
//#else
//#define MGS_STDVLPR_DEVICE_CALL
//#endif




// 导出所有接口函数.
// 包含此头文件的dll工程需要定义预编译宏PROXY_EXPORTS !.
#ifdef WIN32
#ifdef MGS_STDVLPR_DEVICEDLL_EXPORTS
#define MGS_STDVLPR_DEVICE_API __declspec(dllexport)
#else
#define MGS_STDVLPR_DEVICE_API __declspec(dllimport)
#endif
#else
#define MGS_STDVLPR_DEVICE_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

// 初始化
MGS_STDVLPR_DEVICE_API int	MGS_STDVLPR_DEVICE_CALL MGS_STDVLPR_Init();
// 反初始化
MGS_STDVLPR_DEVICE_API int	MGS_STDVLPR_DEVICE_CALL MGS_STDVLPR_UnInit();
// 设备登录
MGS_STDVLPR_DEVICE_API int	MGS_STDVLPR_DEVICE_CALL MGS_STDVLPR_Login(mgs_stdvlpr_dev_info* pDevInfo);
// 设备登出
MGS_STDVLPR_DEVICE_API int	MGS_STDVLPR_DEVICE_CALL MGS_STDVLPR_Logout(int nHandle);
////抓拍结果回调函数
//typedef int (MGS_STDVLPR_DEVICE_CALL *fStdVLPRRegResultCallback)(int nHandle,mgs_stdvlpr_info* pVlprResult,void *pUser);
////设备状态回调函数
//typedef int (MGS_STDVLPR_DEVICE_CALL *fStdVLPRDevStatusCallback)(int nHandle,int nStatus,void* pUser);
//设置抓拍回调接口
MGS_STDVLPR_DEVICE_API int	MGS_STDVLPR_DEVICE_CALL MGS_STDVLPR_SetRegResultCallBack(int nHandle,fStdVLPRRegResultCallback cbRegResult,void *pUser);
//设置设备状态回调接口
MGS_STDVLPR_DEVICE_API int	MGS_STDVLPR_DEVICE_CALL MGS_STDVLPR_SetDevStatusCallBack(int nHandle,int nTimeInvl,  fStdVLPRDevStatusCallback cbDevStatus,void* pUser);
//手动触发抓拍接口
MGS_STDVLPR_DEVICE_API int	MGS_STDVLPR_DEVICE_CALL MGS_STDVLPR_ManualSnap(int nHandle);
//设置设备的ntp校时配置信息，并立即根据配置校时一次，之后按照周期自动从ntp服务端校时。
MGS_STDVLPR_DEVICE_API int	MGS_STDVLPR_DEVICE_CALL MGS_STDVLPR_SetNTPInfo(int nHandle, mgs_stdvlpr_ntp_info *pNtpInfo);
//获取设备状态
MGS_STDVLPR_DEVICE_API int	MGS_STDVLPR_DEVICE_CALL MGS_STDVLPR_GetStatus(int nHandle, int* pnStatusCode);
//获取状态码详细文本信息
MGS_STDVLPR_DEVICE_API int	MGS_STDVLPR_DEVICE_CALL MGS_STDVLPR_GetStatusMsg(int nStatusCode,char* szStatusMsg,int nStatusMsgLen);
//获取设备硬件版本和固件版本
MGS_STDVLPR_DEVICE_API int	MGS_STDVLPR_DEVICE_CALL MGS_STDVLPR_GetVersion(int nHandle,char* szHWVersion,int nHWVerMaxLen,char* szAPIVersion,int nAPIVerMaxLen);


//打印函数
typedef int (MGS_STDVLPR_DEVICE_CALL *LogPrintf)(int level, const char *fmt, ...);

//设置打印函数地址
MGS_STDVLPR_DEVICE_API int MGS_STDVLPR_DEVICE_CALL MGS_STDVLPR_SetLogPrint(LogPrintf cbLogPrintf) ;



#ifdef __cplusplus
}
#endif

//#endif

#endif // MGS_STDVLPR_FJ_H
