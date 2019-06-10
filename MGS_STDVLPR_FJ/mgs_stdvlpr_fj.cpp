#include "mgs_stdvlpr_fj.h"
#include"cameraModule/DeviceListManager.h"
#include"utilityTool/tool_function.h"
#include"utilityTool/inifile.h"
#include <stdio.h>

#define MAX_CAMRA_COUNT (50)

#define ERROR_CODE_OK (0)
#define ERROR_CODE_LOGIN_FAILED (-100)
#define ERROR_CODE_NO_RESPONSE (-1000)
#define ERROR_CODE_PARAMETER_INVALID (-1001)
#define ERROR_CODE_OCCUPIED (-1002)
#define ERROR_CODE_OPEN_FAILED (-1003)
#define ERROR_CODE_OTHER (-1004)


int MGS_STDVLPR_Init()
{
    Tool_WriteFormatLog("MGS_STDVLPR_Init:: begin.");
    DeviceListManager* pDeviceManager = DeviceListManager::GetInstance();
    Tool_WriteFormatLog("MGS_STDVLPR_Init:: finish.");
}

int MGS_STDVLPR_UnInit()
{
    Tool_WriteFormatLog("MGS_STDVLPR_UnInit:: begin.");
    DeviceListManager::GetInstance()->ClearAllDevice();
    Tool_WriteFormatLog("MGS_STDVLPR_UnInit:: finish.");
}

int MGS_STDVLPR_Login(mgs_stdvlpr_dev_info *pDevInfo)
{
    Tool_WriteFormatLog("MGS_STDVLPR_Login:: begin,mgs_stdvlpr_dev_info = %p, device ip = %s .", pDevInfo, pDevInfo->szDevIp);
    int iCamID = -1;
    Camera6467_plate* pCamera = (Camera6467_plate*)DeviceListManager::GetInstance()->GetDeviceByIpAddress(pDevInfo->szDevIp);
    if(pCamera)
    {
        Tool_WriteFormatLog("the camera %s is already exist, get the camera ID %d.",pDevInfo->szDevIp,  pCamera->GetLoginID());
        iCamID = pCamera->GetLoginID();
    }
    else
    {
        for(int index = 1; index< MAX_CAMRA_COUNT; index++)
        {
            if(!DeviceListManager::GetInstance()->FindIfExsit(index))
            {
                pCamera = new Camera6467_plate();
                pCamera->SetCameraIP(pDevInfo->szDevIp);
                if(pCamera->ConnectToCamera() == 0)
                {
                    Tool_WriteFormatLog("connect to camera success.");
                }
                else
                {
                    Tool_WriteFormatLog("connect to camera failed.");
                }
                pCamera->SetLoginID(index);
                DeviceListManager::GetInstance()->AddOneDevice(index, pCamera);
                iCamID = index;
                break;
            }
        }
    }
    Tool_WriteFormatLog("MGS_STDVLPR_Login:: finish, camera id = %d.", iCamID);
    return iCamID;
}

int MGS_STDVLPR_Logout(int nHandle)
{
    Tool_WriteFormatLog("MGS_STDVLPR_Logout:: begin, nHandle = %d .",nHandle);
    Camera6467_plate* pCamera = (Camera6467_plate*)DeviceListManager::GetInstance()->GetDeviceById(nHandle);
    if(pCamera)
    {
        Tool_WriteFormatLog("the camera[%d] %s is  exist, erase it.", nHandle, pCamera->GetCameraIP() );
        DeviceListManager::GetInstance()->EraseDevice(nHandle);
    }
    Tool_WriteFormatLog("MGS_STDVLPR_Logout:: finish .");
    return ERROR_CODE_OK;
}

int MGS_STDVLPR_SetRegResultCallBack(int nHandle, fStdVLPRRegResultCallback cbRegResult, void *pUser)
{
    Tool_WriteFormatLog("MGS_STDVLPR_SetRegResultCallBack:: begin, nHandle = %d, fStdVLPRRegResultCallback = %p, pUser = %p .",nHandle, cbRegResult, pUser);
    int iRetCode = ERROR_CODE_OTHER;
    Camera6467_plate* pCamera = (Camera6467_plate*)DeviceListManager::GetInstance()->GetDeviceById(nHandle);
    if(pCamera)
    {
        Tool_WriteFormatLog("find the camera %s, set result callback.",  pCamera->GetCameraIP() );
        pCamera->SetResultCallbackFunc(pUser,(void*)cbRegResult );
        iRetCode = ERROR_CODE_OK;
    }
    else
    {
        Tool_WriteFormatLog("can not find the camer.");
        iRetCode = ERROR_CODE_PARAMETER_INVALID;
    }
    Tool_WriteFormatLog("MGS_STDVLPR_Logout:: finish, return code = %d .", iRetCode);
    return iRetCode;
}

int MGS_STDVLPR_SetDevStatusCallBack(int nHandle, int nTimeInvl, fStdVLPRDevStatusCallback cbDevStatus, void *pUser)
{
    Tool_WriteFormatLog("MGS_STDVLPR_SetDevStatusCallBack:: begin, nHandle = %d, fStdVLPRRegResultCallback = %p, pUser = %p .",nHandle, cbDevStatus, pUser);
    int iRetCode = ERROR_CODE_OTHER;
    Camera6467_plate* pCamera = (Camera6467_plate*)DeviceListManager::GetInstance()->GetDeviceById(nHandle);
    if(pCamera)
    {
        Tool_WriteFormatLog("find the camera %s, set result callback.",  pCamera->GetCameraIP() );
        pCamera->SetConnectStatusCallbackFunc(pUser,(void*)cbDevStatus );
        iRetCode = ERROR_CODE_OK;
    }
    else
    {
        Tool_WriteFormatLog("can not find the camer.");
        iRetCode = ERROR_CODE_PARAMETER_INVALID;
    }
    Tool_WriteFormatLog("MGS_STDVLPR_SetDevStatusCallBack:: finish, return code = %d .", iRetCode);
    return iRetCode;
}

int MGS_STDVLPR_ManualSnap(int nHandle)
{
    Tool_WriteFormatLog("MGS_STDVLPR_ManualSnap:: begin, nHandle = %d .",nHandle);
    int iRetCode = ERROR_CODE_OTHER;
    Camera6467_plate* pCamera = (Camera6467_plate*)DeviceListManager::GetInstance()->GetDeviceById(nHandle);
    if(pCamera)
    {
        Tool_WriteFormatLog("find the camera %s, take capture.",  pCamera->GetCameraIP() );
        if(pCamera->TakeCapture())
        {
            Tool_WriteFormatLog("take capture success.");
            iRetCode = ERROR_CODE_OK;
        }
        else
        {
            Tool_WriteFormatLog("take capture failed.");
            iRetCode = ERROR_CODE_NO_RESPONSE;
        }
    }
    else
    {
        Tool_WriteFormatLog("can not find the camer.");
        iRetCode = ERROR_CODE_PARAMETER_INVALID;
    }
    Tool_WriteFormatLog("MGS_STDVLPR_ManualSnap:: finish, return code = %d .", iRetCode);
    return iRetCode;
}

int MGS_STDVLPR_SetNTPInfo(int nHandle, mgs_stdvlpr_ntp_info *pNtpInfo)
{
    Tool_WriteFormatLog("MGS_STDVLPR_SetNTPInfo:: begin, nHandle = %d, mgs_stdvlpr_ntp_info = %p, ip = %s, interval = %d .",
                        nHandle,
                        pNtpInfo,
                        pNtpInfo->szNtpServeIp,
                        pNtpInfo->nTimeInvl);
    int iRetCode = ERROR_CODE_OTHER;
    Camera6467_plate* pCamera = (Camera6467_plate*)DeviceListManager::GetInstance()->GetDeviceById(nHandle);
    if(pCamera)
    {
        Tool_WriteFormatLog("find the camera %s, take capture.",  pCamera->GetCameraIP() );
        pCamera->SetSynTimeInerval(pNtpInfo->nTimeInvl);
        Tool_WriteFormatLog("SetSynTimeInerval success.");
        iRetCode = ERROR_CODE_OK;
    }
    else
    {
        Tool_WriteFormatLog("can not find the camer.");
        iRetCode = ERROR_CODE_PARAMETER_INVALID;
    }
    Tool_WriteFormatLog("MGS_STDVLPR_SetNTPInfo:: finish, return code = %d .", iRetCode);
    return iRetCode;
}

int MGS_STDVLPR_GetStatus(int nHandle, int *pnStatusCode)
{
    Tool_WriteFormatLog("MGS_STDVLPR_GetStatus:: begin, nHandle = %d, pnStatusCode = %p .",nHandle, pnStatusCode);
    int iRetCode = ERROR_CODE_OTHER;
    Camera6467_plate* pCamera = (Camera6467_plate*)DeviceListManager::GetInstance()->GetDeviceById(nHandle);
    if(pCamera)
    {
        int iStatus = pCamera->GetCamStatus();
        Tool_WriteFormatLog("find the camera %s, and get the connect status = %d..",  pCamera->GetCameraIP(), iStatus );
        if(iStatus == 0)
        {
           *pnStatusCode = 0;
        }
        else
        {
            *pnStatusCode = 1;
        }
         iRetCode = ERROR_CODE_OK;
    }
    else
    {
        Tool_WriteFormatLog("can not find the camer.");
        *pnStatusCode = 1;
        iRetCode = ERROR_CODE_PARAMETER_INVALID;
    }
    Tool_WriteFormatLog("MGS_STDVLPR_ManualSnap:: finish, return code = %d .", iRetCode);
    return iRetCode;
}

int MGS_STDVLPR_GetStatusMsg(int nStatusCode, char *szStatusMsg, int nStatusMsgLen)
{
    Tool_WriteFormatLog("MGS_STDVLPR_GetStatusMsg:: begin, nStatusCode = %d, szStatusMsg = %p, nStatusMsgLen = %d ."
                        ,nStatusCode,
                        szStatusMsg,
                        nStatusMsgLen);
    switch (nStatusCode)
    {
    case ERROR_CODE_OK:
        sprintf(szStatusMsg, "operation success.");
        break;
    case ERROR_CODE_LOGIN_FAILED:
        sprintf(szStatusMsg, "device login failed..");
        break;
    case ERROR_CODE_NO_RESPONSE:
        sprintf(szStatusMsg, "the operation is no response.");
        break;
    case ERROR_CODE_PARAMETER_INVALID:
        sprintf(szStatusMsg, " parameters of operation is invalid.");
        break;
    case ERROR_CODE_OCCUPIED:
        sprintf(szStatusMsg, "device is busy, operation is occupied.");
        break;
    case ERROR_CODE_OPEN_FAILED:
        sprintf(szStatusMsg, "open device failed.");
        break;
    case ERROR_CODE_OTHER:
        sprintf(szStatusMsg, "unkonw error.");
        break;
    default:
        sprintf(szStatusMsg, "unkonw error.");
        break;
    }

    Tool_WriteFormatLog("MGS_STDVLPR_GetStatusMsg:: finish .");
    return ERROR_CODE_OK;
}

int MGS_STDVLPR_GetVersion(int nHandle, char *szHWVersion, int nHWVerMaxLen, char *szAPIVersion, int nAPIVerMaxLen)
{
    Tool_WriteFormatLog("MGS_STDVLPR_GetVersion:: begin, nHandle = %d, szHWVersion = %p, nHWVerMaxLen= %d,  szAPIVersion = %p, nAPIVerMaxLen = %d .",
                        nHandle,
                        szHWVersion,
                        nHWVerMaxLen,
                        szAPIVersion,
                        nAPIVerMaxLen);

    int iRetCode = ERROR_CODE_OTHER;
    Camera6467_plate* pCamera = (Camera6467_plate*)DeviceListManager::GetInstance()->GetDeviceById(nHandle);
    if(pCamera)
    {
        Tool_WriteFormatLog("find the camera %s, take capture.",  pCamera->GetCameraIP() );
        BasicInfo info;
        if(pCamera->GetHardWareInfo(info))
        {
            Tool_WriteFormatLog("GetHardWareInfo success.");
            iRetCode = ERROR_CODE_OK;

            memcpy(szHWVersion, info.szDevVersion, strlen(info.szDevVersion));
            szHWVersion[strlen(info.szDevVersion)] = '\0';

            sprintf(szAPIVersion, "%s", DLL_VERSION);
            Tool_WriteFormatLog("szHWVersion = %s,szAPIVersion = %s ",szHWVersion, szAPIVersion );
        }
        else
        {
            Tool_WriteFormatLog("take capture failed.");
            sprintf(szAPIVersion, "%s", DLL_VERSION);

            iRetCode = ERROR_CODE_NO_RESPONSE;
        }
    }
    else
    {
        Tool_WriteFormatLog("can not find the camer.");
        iRetCode = ERROR_CODE_PARAMETER_INVALID;
    }
    Tool_WriteFormatLog("MGS_STDVLPR_GetVersion:: finish, return code = %d .", iRetCode);
    return iRetCode;
}
