#include"testfunc.h"
//#include"vpr_ctzy.h"
#include<stdio.h>
#include<memory.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<cstring>
#include <unistd.h>
#include<arpa/inet.h>
#include "mgs_stdvlpr_fj.h"

//#include"inifile.h"

#define BUFLEN 512	//Max length of buffer
#define PORT 10010	//The port on which to listen for incoming data
bool g_bExit = false;

int g_iCameraID = 0;

int  g_fStdVLPRRegResultCallback(int camID ,mgs_stdvlpr_info* info,void*  userData)
{
    printf("fStdVLPRRegResultCallback begin.. camera handle = %d, info = %p, userData = %p \n", camID, info, userData);
    if(info == NULL)
    {
        printf("the mgs_stdvlpr_info data is null. \n");
        return 0;
    }
    printf("nPlaceNo = %d \t  time = %s \t nIsTail = %d \n", info->nPlaceNo, info->time, info->nIsTail);
    printf("vehType = %d \t  plateColor = %c%c \t plateText = %s \n", info->vehType, info->plateColor[0], info->plateColor[1], info->plateText);
    printf("reliability = %u \t \n ", info->reliability);
    printf("image[0] = %p \t length = %u\n ", info->image[0], info->imageLength[0]);
    printf("image[1] = %p \t length = %u\n ", info->image[1], info->imageLength[1]);
    printf("image[2] = %p \t length = %u\n ", info->image[2], info->imageLength[2]);
    printf("vehSpeed = %d \t  vehBodyColorNo = %u \t vehBodyDeepNo = %u \n", info->vehSpeed, info->vehBodyColorNo, info->vehBodyDeepNo);
    printf("vehClassTypeNo = %u \t  plateTypeNo = %u  \n", info->vehClassTypeNo, info->plateTypeNo);
    printf("szCarBrand = %s \t  reserved = %s  \n", info->szCarBrand, info->reserved);
    printf("g_fStdVLPRRegResultCallback finish.\n");

    return 0;
}

int g_fStdVLPRDevStatusCallback(int camHandle ,int status, void* userData)
{
    printf("fStdVLPRDevStatusCallback : camera handle = %d, status = %d, userData = %p \n", camHandle, status, userData);
    return 0;
}


void  printf_menu()
{
    char szMenu[1024] = {0};
    sprintf(szMenu,"************menu***************\n");

    strcat(szMenu, "1 MGS_STDVLPR_Init, init camera source.\n");
    strcat(szMenu, "2 MGS_STDVLPR_UnInit, release camera source.\n");
    strcat(szMenu, "3 MGS_STDVLPR_Login, connect to camera.\n");
    strcat(szMenu, "4 MGS_STDVLPR_Logout , disconnect camera \n");
    strcat(szMenu, "5 MGS_STDVLPR_SetRegResultCallBack , set result call back function \n");
    strcat(szMenu, "6 MGS_STDVLPR_SetDevStatusCallBack , set status call back function \n");
    strcat(szMenu, "7 MGS_STDVLPR_ManualSnap, take capture.\n");
    strcat(szMenu, "8 MGS_STDVLPR_SetNTPInfo, synTime.\n");
    strcat(szMenu, "9 MGS_STDVLPR_GetStatus\n");
    strcat(szMenu, "10 MGS_STDVLPR_GetStatusMsg\n");
    strcat(szMenu, "11 MGS_STDVLPR_GetVersion\n");
    strcat(szMenu, "q 退出\n");
    strcat(szMenu, "********************************\n");

    printf("%s", szMenu);
}

void InitCamera()
{
    printf("begin to init camere resurce.");
    int iRet = MGS_STDVLPR_Init();
    printf("MGS_STDVLPR_Init finish, return code = %d \n", iRet);
}

void ConnectToCamera()
{
    printf("begin to connect to  camere , please input ip address first: .");
    char chIP[64] = {0};
    scanf("%s", chIP);
    chIP[63] = '\0';
    if(strlen(chIP) <= 4
            || strstr(chIP, ".") == NULL)
    {
        printf("the input parameter %s is invalid. \n",chIP);
        return;
    }

    mgs_stdvlpr_dev_info info;
    memset(&info, 0, sizeof(info));
    sprintf(info.szDevIp, "%s", chIP );
    g_iCameraID = MGS_STDVLPR_Login(&info);
    printf("MGS_STDVLPR_Login finish, return code = %d \n", g_iCameraID);
}

void ReleaseCameraResource()
{
    printf("begin to DeInit camere resurce.");
    int iRet = MGS_STDVLPR_UnInit();
    printf("MGS_STDVLPR_UnInit finish, return code = %d \n", iRet);
}

void DisConnectCamera()
{
    printf("MGS_STDVLPR_Logout begin.");

    int iRet = MGS_STDVLPR_Logout(g_iCameraID);
    printf("MGS_STDVLPR_Logout finish, return code = %d \n", iRet);
}

void SetResultCallBack()
{
    printf("begin to SetResultCallBack. \n");
    int iRet = MGS_STDVLPR_SetRegResultCallBack(g_iCameraID, g_fStdVLPRRegResultCallback, NULL);
    printf("SetResultCallBack finish, iRet = %d. \n", iRet);
}

void SetStatusCallback()
{
    printf("begin to SetStatusCallback. \n");
    int iRet = MGS_STDVLPR_SetDevStatusCallBack(g_iCameraID, 10, g_fStdVLPRDevStatusCallback, NULL);
    printf("MGS_STDVLPR_SetDevStatusCallBack finish, iRet = %d. \n", iRet);
}

void TakeCapture()
{
    printf("begin to MGS_STDVLPR_ManualSnap. \n");
    int iRet = MGS_STDVLPR_ManualSnap(g_iCameraID);
    printf("MGS_STDVLPR_ManualSnap finish, iRet = %d. \n", iRet);
}

void SynDeviceTime()
{
    printf("begin to MGS_STDVLPR_SetNTPInfo. \n");
    mgs_stdvlpr_ntp_info info;
    memset(&info, 0, sizeof(mgs_stdvlpr_ntp_info));
    info.nTimeInvl = 30;
    strcat(info.szNtpServeIp, "127.0.0.1");
    info.nNtpServerPort = 9998;
    int iRet = MGS_STDVLPR_SetNTPInfo(g_iCameraID, &info);
    printf("MGS_STDVLPR_SetNTPInfo finish, iRet = %d. \n", iRet);
}

void GetDeviceStatus()
{
    printf("begin to MGS_STDVLPR_GetStatus. \n");
    int nStatusCode = 1;
    int iRet = MGS_STDVLPR_GetStatus(g_iCameraID, &nStatusCode);
    printf("MGS_STDVLPR_GetStatus finish, iRet = %d, nStatusCode = %d. \n", iRet, nStatusCode);
}

void GetDeviceStatusMessage()
{
    printf("begin to MGS_STDVLPR_GetStatusMsg, please input the status code : \n");
    int nStatusCode = 1;
    scanf("%d", &nStatusCode);
    char chMessage[256] = {0};
    int iLength = sizeof(chMessage);
    int iRet = MGS_STDVLPR_GetStatusMsg(nStatusCode, chMessage, iLength);
    printf("MGS_STDVLPR_GetStatus(%d) finish, iRet = %d, message  = %s. \n", nStatusCode,iRet, chMessage);
}

void GetDeviceVersion()
{
    printf("begin to MGS_STDVLPR_GetVersion \n");

    char szHWVersion[256] = {0};
     char szAPIVersion[256] = {0};
    int iLength = sizeof(szHWVersion);
    int iRet = MGS_STDVLPR_GetVersion(g_iCameraID, szHWVersion, iLength, szAPIVersion, sizeof(szAPIVersion));
    printf("MGS_STDVLPR_GetVersionfinish, iRet = %d, szHWVersion  = %s. szAPIVersion = %s \n",
           iRet,
           iLength,
          szAPIVersion );
}

bool SaveFileToPath(const char *path, void *pData, size_t dataSize)
{
    FILE* pFile = NULL;
    //fopen_s(&pFile, chFileName, "wb");
    pFile = fopen(path, "wb+");

    if (pFile)
    {
        fwrite(pData, 1, dataSize, pFile);
        fclose(pFile);
        pFile = NULL;

        return true;
    }
    else
    {
        return false;
    }
}
