//#include "stdafx.h"
#include "Camera6467_plate.h"
#include "libHvDevice/HvDeviceBaseType.h"
#include "libHvDevice/HvDeviceCommDef.h"
//#include "libHvDevice/HvDeviceNew.h"
//#include "libHvDevice/HvCamera.h"
#include "utilityTool/tool_function.h"
#include"utilityTool/inifile.h"
//#include "utilityTool/log4z.h"
//#include <process.h>
#include<time.h>
#include<stdio.h>
#include"CommonDef.h"


#define CHECK_ARG(arg)\
if (arg == NULL) \
{\
    WriteFormatLog("%s is NULL", #arg); \
    return 0; \
}

Camera6467_plate::Camera6467_plate() :
BaseCamera(),
m_iTimeInvl(10),
m_iSynTimInterval(30),
m_iCompressBigImgSize(COMPRESS_BIG_IMG_SIZE),
m_iCompressSamllImgSize(COMPRESS_PLATE_IMG_SIZE),
m_pTempBin(NULL),
m_pTempBig1(NULL),
m_pCaptureImg(NULL),
m_pTempBig(NULL),
m_CameraResult(NULL),
m_BufferResult(NULL),
m_pResultUserData(NULL),
m_pResultCallbackFunc(NULL),
m_pStatusUserData(NULL),
m_pStatusCallbackFunc(NULL),
m_bResultComplete(false),
m_bJpegComplete(false),
m_bSaveToBuffer(false),
m_bOverlay(false),
m_bCompress(false),
m_bStatusCheckThreadExit(false)
{
	m_bProvideBigImg = true;
	m_bUseHwnd = false;
    memset(m_chResultPath, '\0', sizeof(m_chResultPath));
    ReadConfig();

    int iRet_thrd = pthread_create(&m_threadCheckStatus, NULL, &Camera_StatusCheckThread, (void *) this);
    m_bThreadCreateSussess = (iRet_thrd == 0) ? true :false;
    WriteFormatLog("pthread_create = %d", iRet_thrd);
}

Camera6467_plate::~Camera6467_plate()
{
    SetCheckThreadExit(true);
    InterruptionConnection();

    SAFE_DELETE_OBJ(m_CameraResult);
    SAFE_DELETE_OBJ(m_BufferResult);

    SAFE_DELETE_ARRAY(m_pTempBin);
    SAFE_DELETE_ARRAY(m_pTempBig1);
    SAFE_DELETE_ARRAY(m_pCaptureImg);
    SAFE_DELETE_ARRAY(m_pTempBig);

    if(m_bThreadCreateSussess)
    {
        pthread_join(m_threadCheckStatus, NULL);
    }
}

int Camera6467_plate::AnalysisVelchType(const char* vehType)
{
    if (vehType == NULL)
    {
        return UNKOWN_TYPE;
    }
    if (strstr(vehType, "客1"))
    {
        return BUS_TYPE_1;
    }
    else if (strstr(vehType, "客2"))
    {
        return BUS_TYPE_2;
        //printf("the Vehicle type code is 2.\n");
    }
    else if (strstr(vehType, "客3"))
    {
        return BUS_TYPE_3;
    }
    else if (strstr(vehType, "客4"))
    {
        return BUS_TYPE_4;
    }
    else if (strstr(vehType, "客5"))
    {
        return BUS_TYPE_5;
    }
    else if (strstr(vehType, "货1"))
    {
        return TRUCK_TYPE_1;
    }
    else if (strstr(vehType, "货2"))
    {
        return TRUCK_TYPE_2;
    }
    else if (strstr(vehType, "货3"))
    {
        return TRUCK_TYPE_3;
    }
    else if (strstr(vehType, "货4"))
    {
        return TRUCK_TYPE_4;
    }
    else if (strstr(vehType, "货5"))
    {
        return TRUCK_TYPE_5;
    }
    else
    {
        return UNKOWN_TYPE;
    }
}

bool Camera6467_plate::AnalysisVelchBodyColor(const char *srcData, int &ColorShade, int &bodyColor)
{
    if(srcData == NULL)
    {
        WriteFormatLog("AnalysisiVelchBodyColor:: the source data is NULL. ");
        return false;
    }
    if(strstr(srcData, "深"))
    {
        ColorShade = CAR_BODY_DARKCOLOR;
    }
    else
    {
        ColorShade = CAR_BODY_LIGHTCOLOR;
    }

    if (strstr(srcData, "蓝"))
    {
        bodyColor = COLOR_BLUE;
    }
    else if (strstr(srcData, "黄"))
    {
        bodyColor = COLOR_YELLOW;
    }
    else if (strstr(srcData, "黑"))
    {
        bodyColor = COLOR_BLACK;
    }
    else if (strstr(srcData, "绿"))
    {
        bodyColor = COLOR_GREEN;
    }
    else if (strstr(srcData, "白"))
    {
        bodyColor = COLOR_WHITE;
    }
    else if (strstr(srcData, "红"))
    {
        bodyColor = COLOR_RED;
    }
    else if (strstr(srcData, "灰"))
    {
        bodyColor = COLOR_GREY;
    }
    else if (strstr(srcData, "粉"))
    {
        bodyColor = COLOR_PINK;
    }
    else if (strstr(srcData, "紫"))
    {
        bodyColor = COLOR_PUEPLE;
    }
    else if (strstr(srcData, "棕"))
    {
        bodyColor = COLOR_BROWN;
    }
    else
    {
        bodyColor = COLOR_UNKNOW;
    }
    return true;
}

int Camera6467_plate::AnalysisFrontMode(const char *srcData)
{
    if(srcData == NULL)
    {
        return CAR_FRONT_MODE;
    }
    else
    {
        if(strstr(srcData, "头"))
        {
            return CAR_FRONT_MODE;
        }
        else
        {
            return CAR_TAIL_MODE;
        }
    }
}

int Camera6467_plate::AnalysisPlateColorType(const char *srcData)
{
    if(srcData == NULL)
    {
        return 0;       //orther
    }
    else
    {
        if(strstr(srcData, "黄"))
        {
            return 1;   //Black on yellow background
        }
        else if(strstr(srcData, "蓝"))
        {
            return 2;   //Blue on white background
        }
        else if(strstr(srcData, "黑"))
        {
            return 3;   //white on black background
        }
        else if(strstr(srcData, "使")
                || strstr(srcData, "领") )
        {
            return 4;   //red on black background
        }
        else if(strstr(srcData, "白"))
        {
            return 6;   //red on white background
        }
        else if(strstr(srcData, "警"))
        {
            return 5;   //red on white background
        }
        else
        {
            return 0;       //orther
        }
    }
}

void Camera6467_plate::ReadConfig()
{
    char szPath[MAX_PATH] = {0};
    //sprintf_s(szPath, sizeof(szPath), "%s\\Result\\", Tool_GetCurrentPath());
    sprintf(szPath, "%s/Result/", Tool_GetCurrentPath());
    Tool_ReadKeyValueFromConfigFile(INI_FILE_NAME, "Result", "Path", szPath, MAX_PATH);
    //strcpy_s(m_chResultPath, sizeof(m_chResultPath), szPath);
	strcpy(m_chResultPath, szPath);

	
	int iValue = 0;
	Tool_ReadIntValueFromConfigFile(INI_FILE_NAME, "COMPRESS_SET", "CompressEnable",  iValue);
	m_bCompressEnable = (bool)iValue;

	m_iCompressBigImgSize = COMPRESS_BIG_IMG_SIZE;
	Tool_ReadIntValueFromConfigFile(INI_FILE_NAME, "COMPRESS_SET", "CompressSize",  m_iCompressBigImgSize);

	m_iCompressBigImgWidth = 1600;
	Tool_ReadIntValueFromConfigFile(INI_FILE_NAME, "COMPRESS_SET", "BigImgWidth", m_iCompressBigImgWidth);

	m_iCompressBigImgHeight = 1200;
	Tool_ReadIntValueFromConfigFile(INI_FILE_NAME, "COMPRESS_SET", "BigImgHeight", m_iCompressBigImgWidth);

    BaseCamera::ReadConfig();
}

void Camera6467_plate::SetCheckThreadExit(bool bExit)
{
    m_csFuncCallback.lock();
    m_bStatusCheckThreadExit = bExit;
    m_csFuncCallback.unlock();
}

bool Camera6467_plate::GetCheckThreadExit()
{
    bool bExit = false;
    m_csFuncCallback.lock();
    bExit = m_bStatusCheckThreadExit;
    m_csFuncCallback.unlock();
    return bExit;
}

void Camera6467_plate::SendResultByCallback()
{
    m_csFuncCallback.lock();
    if(m_pResultCallbackFunc != NULL)
    {
        m_csFuncCallback.unlock();
        WriteFormatLog("SendResultByCallback:: prepare to send result by callback func, userData = %p, Plate number = %s, time = %s.",
                       m_pResultUserData,
                       m_BufferResult->chPlateNO,
                       m_BufferResult->chPlateTime);
        mgs_stdvlpr_info resultInfo;

        resultInfo.nPlaceNo = m_BufferResult->iLaneNo;
        strcpy(resultInfo.time, m_BufferResult->chPlateTime);
        resultInfo.nIsTail = m_BufferResult->bFrontMode ? 0 : 1;
        resultInfo.vehType = m_BufferResult->iVehTypeNo;

        WriteFormatLog("SendResultByCallback::analysisPlateColot");
        switch(m_BufferResult->iPlateColor)
        {
        case COLOR_BLUE:
            memcpy(resultInfo.plateColor,"00", 2 );
            break;
        case COLOR_YELLOW:
            memcpy(resultInfo.plateColor,"01", 2 );
            break;
        case COLOR_BLACK:
            memcpy(resultInfo.plateColor,"02", 2 );
            break;
        case COLOR_WHITE:
            memcpy(resultInfo.plateColor,"03", 2 );
            break;
        case COLOR_GRADIENT_CREEN:
            memcpy(resultInfo.plateColor,"04", 2 );
            break;
        case COLOR_YELLOW_GREEN:
            memcpy(resultInfo.plateColor,"05", 2 );
            break;
        case COLOR_BLUE_WHITE:
            memcpy(resultInfo.plateColor,"06", 2 );
            break;
        case COLOR_UNKNOW:
            memcpy(resultInfo.plateColor,"09", 2 );
            break;
        case COLOR_GREEN:
            memcpy(resultInfo.plateColor,"11", 2 );
            break;
        case COLOR_RED:
            memcpy(resultInfo.plateColor,"12", 2 );
            break;
        default:
            memcpy(resultInfo.plateColor,"09", 2 );
            break;
        }
        WriteFormatLog("SendResultByCallback:: get the plate number.");
        if(strstr(m_BufferResult->chPlateNO, "无"))
        {
            memcpy(resultInfo.plateText, "默A00000\0", strlen("默A00000\0")+1);
        }
        else
        {
            memcpy(resultInfo.plateText, m_BufferResult->chPlateNO+2, strlen(m_BufferResult->chPlateNO+2)+1);
        }

        resultInfo.reliability = m_BufferResult->fConfidenceLevel*10000;
        WriteFormatLog("SendResultByCallback::  get the image.");

        resultInfo.imageLength[0] = m_BufferResult->CIMG_LastSnapshot.dwImgSize;
        resultInfo.image[0] = m_BufferResult->CIMG_LastSnapshot.pbImgData;

        resultInfo.imageLength[1] = m_BufferResult->CIMG_PlateImage.dwImgSize;
        resultInfo.image[1] = m_BufferResult->CIMG_PlateImage.pbImgData;

        resultInfo.imageLength[2] = m_BufferResult->CIMG_BinImage.dwImgSize;
        resultInfo.image[2] = m_BufferResult->CIMG_BinImage.pbImgData;

        resultInfo.vehSpeed = m_BufferResult->iSpeed;

        WriteFormatLog("SendResultByCallback::  analysis body color.");
        switch(m_BufferResult->iVehBodyColorNo)
        {
        case COLOR_BLUE:
            resultInfo.vehBodyColorNo = 8;
            break;
        case COLOR_YELLOW:
            resultInfo.vehBodyColorNo =3;
            break;
        case COLOR_BLACK:
            resultInfo.vehBodyColorNo = 10;
            break;
        case COLOR_WHITE:
            resultInfo.vehBodyColorNo = 1;
            break;
        case COLOR_RED:
            resultInfo.vehBodyColorNo = 5;
            break;
        case COLOR_GREEN:
            resultInfo.vehBodyColorNo = 7;
            break;
        case COLOR_GREY:
            resultInfo.vehBodyColorNo = 2;
            break;
        case COLOR_PINK:
            resultInfo.vehBodyColorNo = 4;
            break;
        case COLOR_PUEPLE:
            resultInfo.vehBodyColorNo = 6;
            break;
        case COLOR_BROWN:
            resultInfo.vehBodyColorNo = 9;
            break;
        case COLOR_UNKNOW:
            resultInfo.vehBodyColorNo = 0;
            break;
        default:
            resultInfo.vehBodyColorNo = 8;
            break;
        }
        resultInfo.vehBodyDeepNo = m_BufferResult->iVehBodyDeepNo;
        resultInfo.vehClassTypeNo = m_BufferResult->iVehClassType;
        resultInfo.plateTypeNo = m_BufferResult->iPlateTypeNo;

        WriteFormatLog("SendResultByCallback::  get the car brand info.");
        sprintf(resultInfo.szCarBrand,m_BufferResult->chCarFace);
        sprintf(resultInfo.reserved," ");

        WriteFormatLog("SendResultByCallback:: begin to call Function.");
        (fStdVLPRRegResultCallback(m_pResultCallbackFunc))(GetLoginID(),& resultInfo, m_pResultUserData);

         WriteFormatLog("SendResultByCallback:: finish  sending result by callback func, userData = %p.", m_pResultUserData);
    }
    else
    {
        m_csFuncCallback.unlock();
        WriteFormatLog("SendResultByCallback:: result call back func is null.");
    }

}

int Camera6467_plate::RecordInfoBegin(unsigned long dwCarID)
{
    WriteFormatLog("RecordInfoBegin -begin- dwCarID = %lu", dwCarID);

    if (dwCarID == m_dwLastCarID)
    {
        WriteLog("相同carID,丢弃该结果");
        return 0;
    }
    SetResultComplete(false);
    SAFE_DELETE_OBJ(m_CameraResult);

    m_CameraResult = new CameraResult();
    if (m_CameraResult)
    {
        //sprintf_s(m_CameraResult->chServerIP, sizeof(m_CameraResult->chServerIP), "%s", m_strIP.c_str());
		sprintf(m_CameraResult->chServerIP,"%s", m_strIP.c_str());
        m_CameraResult->dwCarID = dwCarID;
    }

    WriteFormatLog("RecordInfoBegin -end- dwCarID = %lu", dwCarID);
    return 0;
}

int Camera6467_plate::RecordInfoEnd(unsigned long dwCarID)
{
    WriteFormatLog("RecordInfoEnd begin, dwCarID = %d", dwCarID);

    if (dwCarID != m_dwLastCarID)
    {
        m_dwLastCarID = dwCarID;
    }
    else
    {
        WriteLog("相同CarID, 丢弃该结果");
        return 0;
    }

    if (NULL == m_CameraResult)
    {
        return 0;
    }

    //std::shared_ptr<CameraResult> pResult = std::shared_ptr<CameraResult>(m_CameraResult);
    //if (pResult)
    //{
    //    if (m_lsResultList.size() < 200)
    //    {
    //        m_lsResultList.push_back(pResult);
    //    }
    //    else
    //    {
    //        WriteFormatLog("RecordInfoEnd end, the result list is larger than 200, save the result direct.");
    //        //SerializationResult(pResult);
    //        SerializationResultEx(pResult);
    //    }        
    //    m_CameraResult = NULL;
    //}
    //SAFE_DELETE_OBJ(m_CameraResult);

    m_csResult.lock();
	if (m_BufferResult != NULL)
	{
		delete m_BufferResult;
		m_BufferResult = NULL;
	}
	m_BufferResult = new CameraResult(*m_CameraResult);
    m_csResult.unlock();

	SetResultComplete(true);
    SendResultByCallback();

    //WriteFormatLog("RecordInfoEnd end, dwCarID = %lu", dwCarID, m_lsResultList.size());
	WriteFormatLog("RecordInfoEnd end, dwCarID = %lu", dwCarID);
    return 0;
}

int Camera6467_plate::RecordInfoPlate(unsigned long dwCarID,
    const char* pcPlateNo,
    const char* pcAppendInfo,
    unsigned long dwRecordType,
    unsigned long long dw64TimeMS)
{
    SetResultComplete(false);
    CHECK_ARG(m_CameraResult);

    WriteFormatLog("RecordInfoPlate -begin- dwCarID = %lu", dwCarID);

    if (dwCarID == m_dwLastCarID)
    {
        BaseCamera::WriteLog("相同carID,丢弃该结果");
        return 0;
    }
//#ifdef DEBUG
//    char chName[256] = { 0 };
//    sprintf_s(chName, "%lu.xml", GetTickCount());
//    Tool_SaveFileToPath(chName, (void*)pcAppendInfo, strlen(pcAppendInfo));
//#endif // DEBUG

    if (m_CameraResult->dwCarID == dwCarID)
    {
        m_CameraResult->dw64TimeMS = dw64TimeMS;
        //sprintf_s(m_CameraResult->chPlateNO, sizeof(m_CameraResult->chPlateNO), "%s", pcPlateNo);
		sprintf(m_CameraResult->chPlateNO, "%s", pcPlateNo);
        //m_CameraResult->strAppendInfo = std::string(pcAppendInfo);
        if (strlen(pcAppendInfo) < sizeof(m_CameraResult->pcAppendInfo) )
        {
            memset(m_CameraResult->pcAppendInfo, '\0', sizeof(m_CameraResult->pcAppendInfo));
            memcpy(m_CameraResult->pcAppendInfo, pcAppendInfo, strlen(pcAppendInfo));
        }
        else
        {
            WriteFormatLog("RecordInfoPlate -begin- strlen(pcAppendInfo) %u < %u sizeof(m_CameraResult->pcAppendInfo)",
                strlen(pcAppendInfo),
                sizeof(m_CameraResult->pcAppendInfo));
        }

        BaseCamera::WriteLog(m_CameraResult->chPlateNO);  
        BaseCamera::WriteLog(pcAppendInfo);
    }
	AnalysisAppendInfo(m_CameraResult);
    WriteFormatLog("RecordInfoPlate -end- dwCarID = %lu", dwCarID);
    return 0;
}

int Camera6467_plate::RecordInfoBigImage(unsigned long dwCarID,
    unsigned short wImgType,
    unsigned short wWidth,
    unsigned short wHeight,
    unsigned char* pbPicData,
    unsigned long dwImgDataLen,
    unsigned long dwRecordType,
    unsigned long long dw64TimeMS)
{
    SetResultComplete(false);

    CHECK_ARG(m_CameraResult);

    WriteFormatLog("RecordInfoBigImage -begin- dwCarID = %ld, dwRecordType = %#x， ImgType=%d, size = %ld",
        dwCarID, 
        dwRecordType, 
        wImgType,
        dwImgDataLen);

    if (dwCarID == m_dwLastCarID)
    {
        WriteLog("相同carID,丢弃该结果");
        return 0;
    }
    if (m_CameraResult->dwCarID == dwCarID)
    {
        if (wImgType == RECORD_BIGIMG_BEST_SNAPSHOT)
        {
            WriteLog("RecordInfoBigImage BEST_SNAPSHO  ");

            CopyDataToIMG(m_CameraResult->CIMG_BestSnapshot, pbPicData, wWidth, wHeight, dwImgDataLen, wImgType);
        }
        else if (wImgType == RECORD_BIGIMG_LAST_SNAPSHOT)
        {
            WriteLog("RecordInfoBigImage LAST_SNAPSHOT  ");

            CopyDataToIMG(m_CameraResult->CIMG_LastSnapshot, pbPicData, wWidth, wHeight, dwImgDataLen, wImgType);
        }
        else if (wImgType == RECORD_BIGIMG_BEGIN_CAPTURE)
        {
            WriteLog("RecordInfoBigImage BEGIN_CAPTURE  ");

            CopyDataToIMG(m_CameraResult->CIMG_BeginCapture, pbPicData, wWidth, wHeight, dwImgDataLen, wImgType);
        }
        else if (wImgType == RECORD_BIGIMG_BEST_CAPTURE)
        {
            WriteLog("RecordInfoBigImage BEST_CAPTURE  ");

            CopyDataToIMG(m_CameraResult->CIMG_BestCapture, pbPicData, wWidth, wHeight, dwImgDataLen, wImgType);
        }
        else if (wImgType == RECORD_BIGIMG_LAST_CAPTURE)
        {
            WriteLog("RecordInfoBigImage LAST_CAPTURE  ");

            CopyDataToIMG(m_CameraResult->CIMG_LastCapture, pbPicData, wWidth, wHeight, dwImgDataLen, wImgType);
        }
        else
        {
            WriteLog("RecordInfoBigImage other Image, put it to  LAST_CAPTURE .");
            CopyDataToIMG(m_CameraResult->CIMG_LastCapture, pbPicData, wWidth, wHeight, dwImgDataLen, wImgType);
        }
    }

    WriteFormatLog("RecordInfoBigImage -end- dwCarID = %lu", dwCarID);

    return 0;
}

int Camera6467_plate::RecordInfoSmallImage(unsigned long dwCarID,
    unsigned short wWidth,
    unsigned short wHeight,
    unsigned char* pbPicData,
    unsigned long dwImgDataLen,
    unsigned long dwRecordType,
    unsigned long long dw64TimeMS)
{
    SetResultComplete(false);
    if (NULL == m_CameraResult)
    {
        return -1;
    }
    WriteFormatLog( "RecordInfoSmallImage  -begin- dwCarID = %lu", dwCarID);

    if (dwCarID == m_dwLastCarID)
    {
        WriteLog("相同carID,丢弃该结果");
        return 0;
    }

    if (m_CameraResult->dwCarID == dwCarID)
    {
        CopyDataToIMG(m_CameraResult->CIMG_PlateImage, pbPicData, wWidth, wHeight, dwImgDataLen, 0x006);
    }
    else
    {
        WriteFormatLog("current car ID  %lu is not same wit result carID %lu.", dwCarID, m_CameraResult->dwCarID);
    }

    WriteFormatLog("RecordInfoSmallImage  -end- dwCarID = %lu", dwCarID);
    return 0;
}

int Camera6467_plate::RecordInfoBinaryImage(unsigned long dwCarID,
    unsigned short wWidth,
    unsigned short wHeight,
    unsigned char* pbPicData,
    unsigned long dwImgDataLen,
    unsigned long dwRecordType,
    unsigned long long dw64TimeMS)
{
    SetResultComplete(false);

    if (NULL == m_CameraResult)
    {
        return -1;
    }
    WriteFormatLog("RecordInfoBinaryImage -begin- dwCarID = %lu", dwCarID);


    if (dwCarID == m_dwLastCarID)
    {
        WriteLog("相同carID,丢弃该结果");
        return 0;
    }
    //int iBufferlength = 1024 * 1024;
    //if (m_pTempBin == NULL)
    //{
    //    m_pTempBin = new BYTE[1024 * 1024];
    //    memset(m_pTempBin, 0x00, iBufferlength);
    //}
    //if (m_pTempBin)
    //{
    //    memset(m_pTempBin, 0x00, iBufferlength);

    //    HRESULT hRet = HVAPIUTILS_BinImageToBitmapEx(pbPicData, m_pTempBin, &iBufferlength);
    //    if (hRet == S_OK)
    //    {
    //        if (m_Bin_IMG_Temp.pbImgData == NULL)
    //        {
    //            m_Bin_IMG_Temp.pbImgData = new BYTE[MAX_IMG_SIZE];
    //            memset(m_Bin_IMG_Temp.pbImgData, 0x00, MAX_IMG_SIZE);
    //        }
    //        if (m_Bin_IMG_Temp.pbImgData)
    //        {
    //            DWORD iDestLenth = MAX_IMG_SIZE;
    //            memset(m_Bin_IMG_Temp.pbImgData, 0x00, MAX_IMG_SIZE);
    //            WriteLog("bin, convert bmp to jpeg , begin .");
    //            bool bScale = Tool_Img_ScaleJpg(m_pTempBin,
    //                iBufferlength,
    //                m_Bin_IMG_Temp.pbImgData,
    //                &iDestLenth,
    //                wWidth,
    //                wHeight,
    //                90);
    //            if (bScale)
    //            {
    //                WriteLog("bin, convert bmp to jpeg success, begin copy.");
    //                CopyDataToIMG(m_CameraResult->CIMG_BinImage, m_Bin_IMG_Temp.pbImgData, wWidth, wHeight, iDestLenth, 0);
    //                WriteLog("bin, convert bmp to jpeg success, finish copy.");
    //            }
    //            else
    //            {
    //                WriteLog("bin, convert bmp to jpeg failed, use default.");
    //            }
    //        }
    //        else
    //        {
    //            WriteLog("m_Bin_IMG_Temp  is null.");
    //        }
    //    }
    //    else
    //    {
    //        WriteLog("HVAPIUTILS_BinImageToBitmapEx, failed, use default.");
    //        CopyDataToIMG(m_CameraResult->CIMG_BinImage, pbPicData, wWidth, wHeight, dwImgDataLen, 0);
    //    }
    //}
    //else
    {
        //WriteLog("m_pTempBin is NULL ,  use default.");
        CopyDataToIMG(m_CameraResult->CIMG_BinImage, pbPicData, wWidth, wHeight, dwImgDataLen, 0);
    }
    WriteFormatLog("RecordInfoBinaryImage -end- dwCarID = %lu", dwCarID);
    return 0;
}

int Camera6467_plate::DeviceJPEGStream(unsigned char* pbImageData,
    unsigned long dwImageDataLen,
    unsigned long dwImageType,
    const char* szImageExtInfo)
{
    static int iCout = 0;
    if (iCout++ > 100)
    {
        WriteLog("receive one jpeg frame.");
        iCout = 0;
    }

    m_csResult.lock();
    m_bJpegComplete = false;

    m_CIMG_StreamJPEG.dwImgSize = dwImageDataLen;
    m_CIMG_StreamJPEG.wImgWidth = 1920;
    m_CIMG_StreamJPEG.wImgHeight = 1080;
    if (NULL == m_CIMG_StreamJPEG.pbImgData)
    {
        m_CIMG_StreamJPEG.pbImgData = new unsigned char[MAX_IMG_SIZE];
        memset(m_CIMG_StreamJPEG.pbImgData, 0, MAX_IMG_SIZE);
    }
    if (m_CIMG_StreamJPEG.pbImgData)
    {
        memset(m_CIMG_StreamJPEG.pbImgData, 0, MAX_IMG_SIZE);
        memcpy(m_CIMG_StreamJPEG.pbImgData, pbImageData, dwImageDataLen);
        m_bJpegComplete = true;
    }
    m_csResult.unlock();

    return 0;
}

bool Camera6467_plate::GetOneJpegImg(CameraIMG &destImg)
{
    WriteLog("GetOneJpegImg::begin.");
    bool bRet = false;

    if (!destImg.pbImgData)
    {
        WriteLog("GetOneJpegImg:: allocate memory.");
        destImg.pbImgData = new unsigned char[MAX_IMG_SIZE];
        memset(destImg.pbImgData, 0, MAX_IMG_SIZE);
        WriteLog("GetOneJpegImg:: allocate memory success.");
    }

    m_csResult.lock();
    if (m_bJpegComplete)
    {
        if (destImg.pbImgData)
        {
            memset(destImg.pbImgData, 0, MAX_IMG_SIZE);
            memcpy(destImg.pbImgData, m_CIMG_StreamJPEG.pbImgData, m_CIMG_StreamJPEG.dwImgSize);

            destImg.dwImgSize = m_CIMG_StreamJPEG.dwImgSize;
            destImg.wImgHeight = m_CIMG_StreamJPEG.wImgHeight;
            destImg.wImgWidth = m_CIMG_StreamJPEG.wImgWidth;
            bRet = true;
            WriteLog("GetOneJpegImg success.");
            m_bJpegComplete = false;
        }
        else
        {
            WriteLog("GetOneJpegImg:: allocate memory failed.");
        }
    }
    else
    {
        WriteLog("GetOneJpegImg the image is not ready.");
    }
    m_csResult.unlock();
    WriteLog("GetOneJpegImg:: end.");

    return bRet;
}

bool Camera6467_plate::GetResultComplete()
{
    bool bFinish = false;
    m_csResult.lock();
    bFinish = m_bResultComplete;
    m_csResult.unlock();
    return bFinish;
}

CameraResult* Camera6467_plate::GetOneResult()
{
    CameraResult* tempResult = NULL;
    //EnterCriticalSection(&m_csResult);	
    //if (m_ResultList.size() > 0)
    //{
    //	tempResult = m_ResultList.front();
    //	m_ResultList.pop_front();
    //}
    //LeaveCriticalSection(&m_csResult);

    if (GetResultComplete())
    {
        m_csResult.lock();
        tempResult = new CameraResult(*m_BufferResult);
        m_csResult.unlock();
    }
    return tempResult;
}

bool Camera6467_plate::SaveCIMG(CameraIMG& imgStruct,
                                struct tm& systime,
                                unsigned long tickCount,
                                int iType,
                                std::string& outputPath)
{
#define BIN_IMG  0
#define SMALL_IMG 1
#define  BIG_IMG 2
#define  SP_IMG 3
    bool bRet = false;
    if (imgStruct.dwImgSize > 0)
    {
        char szImgPath[MAX_PATH] = { 0 };
        //sprintf_s(szImgPath, sizeof(szImgPath), "%s\\%04d-%02d-%02d-%u_s",
        //    m_chResultPath,
        //    systime.wYear,
        //    systime.wMonth,
        //    systime.wDay,
        //    tickCount);
		sprintf(szImgPath,  "%s\\%04d-%02d-%02d-%u_s",
			m_chResultPath,
            systime.tm_year,
            systime.tm_mon,
            systime.tm_yday,
			tickCount);

        switch (iType)
        {
        case BIN_IMG:
            //strcat_s(szImgPath, sizeof(szImgPath), ".bin");
			strcat(szImgPath, ".bin");
            break;
        case SMALL_IMG:
            //strcat_s(szImgPath, sizeof(szImgPath), "_small.jpg");
			strcat(szImgPath, "_small.jpg");
            break;
        case BIG_IMG:
            //strcat_s(szImgPath, sizeof(szImgPath), "_big1.jpg");
			strcat(szImgPath,  "_big1.jpg");
            break;
        case SP_IMG:
            //strcat_s(szImgPath, sizeof(szImgPath), "_big2.jpg");
			strcat(szImgPath, "_big2.jpg");
            break;
        default:
            //strcat_s(szImgPath, sizeof(szImgPath), "_big1.jpg");
			strcat(szImgPath,  "_big1.jpg");
            break;
        }
        outputPath = szImgPath;
        bRet = SaveImgToDisk(szImgPath, imgStruct.pbImgData, imgStruct.dwImgSize);
    }
    return bRet;
}

void Camera6467_plate::SaveResult()
{
    //std::shared_ptr<CameraResult> pTempResult;
    //while (!GetCheckThreadExit())
    //{
    //    if (m_lsResultList.empty())
    //    {
    //        Sleep(50);
    //        continue;
    //    }
    //    m_lsResultList.front(pTempResult);
    //    if (pTempResult)
    //    {            
    //        SerializationResultEx(pTempResult);
    //    }
    //    Sleep(10);
    //}
}

void Camera6467_plate::SetResultComplete(bool bfinish)
{
    m_csResult.lock();
    m_bResultComplete = bfinish;
    m_csResult.unlock();
}

int Camera6467_plate::GetTimeInterval()
{
    int iTimeInterval = 1;
    m_csFuncCallback.lock();
    iTimeInterval = m_iTimeInvl;
    m_csFuncCallback.unlock();
    return iTimeInterval;
}

void Camera6467_plate::SendConnetStateMsg(bool isConnect)
{
    //if (m_hWnd == NULL)
    //	return;

    if (isConnect)
    {
        m_csFuncCallback.lock();
        if (m_pStatusCallbackFunc)
        {
            m_csFuncCallback.unlock();
            //char chIP[32] = { 0 };
            //sprintf_s(chIP, "%s", m_strIP.c_str());
            //g_ConnectStatusCallback(m_iIndex, 0, g_pUser);
            ((fStdVLPRDevStatusCallback)m_pStatusCallbackFunc)(GetLoginID(), 0, m_pStatusUserData);
        }
        else
        {
            m_csFuncCallback.unlock();
        }

//        EnterCriticalSection(&m_csFuncCallback);
//        ::PostMessage(m_hWnd,m_iConnectMsg, NULL,NULL);
//        LeaveCriticalSection(&m_csFuncCallback);
    }
    else
    {
        m_csFuncCallback.lock();
        if (m_pStatusCallbackFunc)
        {
            m_csFuncCallback.unlock();
            //char chIP[32] = { 0 };
            //sprintf_s(chIP, "%s", m_strIP.c_str());
            //g_ConnectStatusCallback(m_iIndex, 0, g_pUser);
            ((fStdVLPRDevStatusCallback)m_pStatusCallbackFunc)(GetLoginID(), 1, m_pStatusUserData);
        }
        else
        {
            m_csFuncCallback.unlock();
        }

//        EnterCriticalSection(&m_csFuncCallback);
//        ::PostMessage(m_hWnd,m_iDisConMsg, NULL,NULL);
//        LeaveCriticalSection(&m_csFuncCallback);
    }
}

void Camera6467_plate::CheckStatus()
{
    int iLastStatus = -1;
    long iLastTick = 0, iCurrentTick = 0;
    int iFirstConnctSuccess = -1;

    while (!GetCheckThreadExit())
    {
        usleep(50*1000);
        iCurrentTick = Tool_GetTickCount();
        int iTimeInterval = GetTimeInterval();
        if ((iCurrentTick - iLastTick) >= (iTimeInterval * 1000))
        {
            //UpLoadCameraStatus();
            int iStatus = GetCamStatus();

            if (iStatus == 0)
            {
                //if (iStatus != iLastStatus)
                //{
                //	pThis->SendConnetStateMsg(true);
                //}
                SendConnetStateMsg(true);
                WriteLog("设备连接正常.");
                iFirstConnctSuccess = 0;
            }
            else
            {
                SendConnetStateMsg(false);
                WriteLog("设备连接失败, 尝试重连");

                if (iFirstConnctSuccess == -1)
                {
                    //pThis->ConnectToCamera();
                    ConnectToCamera();
                }
            }

            iLastStatus = iStatus;

            iLastTick = iCurrentTick;
        }
        int iSynTimeInterval = GetSynTimeInterval();
        if ((iCurrentTick - iLastTick) >= (iSynTimeInterval * 1000))
        {
            SynTime();
        }
    }
}

void Camera6467_plate::SetDeviceId( unsigned int uID )
{
    m_csFuncCallback.lock();
	m_iDeviceID = uID;
    m_csFuncCallback.unlock();
}

unsigned int Camera6467_plate::GetDeviceID()
{
	unsigned int iDevice = -1;
    m_csFuncCallback.lock();
	iDevice = m_iDeviceID;
    m_csFuncCallback.unlock();

	return iDevice;
}

void Camera6467_plate::SetMsgThreadId( unsigned long id )
{
    m_csFuncCallback.lock();
	m_dwMsgThreadID = id;
    m_csFuncCallback.unlock();
}

unsigned long Camera6467_plate::GetMsgThreadId()
{
    unsigned long iValue = 0;
    m_csFuncCallback.lock();
	iValue = m_dwMsgThreadID;
    m_csFuncCallback.unlock();

	return iValue;
}

void Camera6467_plate::AnalysisAppendInfo( CameraResult* pResult )
{
	if (pResult == NULL)
	{
		return;
	}

    if (0 != pResult->dw64TimeMS)
    {
        //CTime tm(CamResult->dw64TimeMS / 1000);
       struct tm *timeNow = NULL;
       time_t          timeSecond = pResult->dw64TimeMS / 1000;  // Seconds
       timeNow = localtime(&timeSecond);

       sprintf(pResult->chPlateTime, "%04d%02d%02d%02d%02d%02d",
           timeNow->tm_year+1900,
           timeNow->tm_mon+1,
           timeNow->tm_mday,
           timeNow->tm_hour,
           timeNow->tm_min,
           timeNow->tm_sec);

//       sprintf(pResult->chPlateTime, "%04d-%02d-%02d %02d:%02d:%02d:%03d",
//           timeNow->tm_year,
//           timeNow->tm_mon,
//           timeNow->tm_yday,
//           timeNow->tm_hour,
//           timeNow->tm_min,
//           timeNow->tm_sec,
//           pResult->dw64TimeMS%1000);
    }
    else
    {
        struct tm timeNow;
        long iTimeInMilliseconds = 0;
        Tool_GetTime(&timeNow, &iTimeInMilliseconds);

        sprintf(pResult->chPlateTime, "%04d%02d%02d%02d%02d%02d",
            timeNow.tm_year+1900,
            timeNow.tm_mon+1,
            timeNow.tm_mday,
            timeNow.tm_hour,
            timeNow.tm_min,
            timeNow.tm_sec);

//        sprintf(pResult->chPlateTime, "%04d-%02d-%02d %02d:%02d:%02d:%03d",
//            timeNow.tm_year+1900,
//            timeNow.tm_mon+1,
//            timeNow.tm_mday,
//            timeNow.tm_hour,
//            timeNow.tm_min,
//            timeNow.tm_sec,
//            iTimeInMilliseconds%1000);
    }
    WriteFormatLog("get the plate time: %s.",pResult->chPlateTime );

	pResult->iPlateColor = Tool_AnalysisPlateColorNo(pResult->chPlateNO);
    pResult->iPlateTypeNo = AnalysisPlateColorType(pResult->chPlateNO);

	int iVehicleType = 0;
	int iVehicleClass = 0;

	if (strstr(pResult->chPlateNO, "黄")  )
	{//蓝色车牌的时候，给客车，黄色的时候，给货车
		if (strstr(pResult->chPlateNO, "学") )
		{
			iVehicleType = 1;
			iVehicleClass = 1;
		}
		else
		{
			iVehicleType = 2;
			iVehicleClass = 5;
		}
	}
	else
	{
		if (strstr(pResult->chPlateNO, "绿")  )
		{
			int len = 0;
			char szorigePlateNo[64] = {0};
			strcpy(szorigePlateNo, pResult->chPlateNO);
			len = strlen(szorigePlateNo);
			if(szorigePlateNo[len - 1] == 'D' || szorigePlateNo[len - 1] == 'F')
			{
				//iColor = (int)color_yellowGreen;
				//sprintf(pCtrl->m_Result.pchPlate, "%s%s", "黄绿双拼", tmpPlate);
				iVehicleType = 2;
				iVehicleClass = 5;
			}
			else
			{
				//iColor = (int)color_gradientGreen;
				//sprintf(pCtrl->m_Result.pchPlate, "%s%s", "渐变绿", tmpPlate);
				iVehicleType = 1;
				iVehicleClass = 1;
			}
		}
		else
		{
			iVehicleType = 1;
			iVehicleClass = 1;
		}
	}
	pResult->iVehTypeNo = iVehicleType;
	pResult->iVehSizeType =iVehicleClass;
	
	char chBuffer[256] = {0};
	int iBufLenght = sizeof(chBuffer);
    if (Tool_GetDataFromAppenedInfo( pResult->pcAppendInfo, "IsTruck", chBuffer, &iBufLenght ))
	{
		if (strstr(chBuffer, "客"))
		{
			pResult->iVehTypeNo = 1;
		}
		else if (strstr(chBuffer, "货"))
		{
			pResult->iVehTypeNo = 2;
		}
		else
		{
			pResult->iVehTypeNo = 0;
		}
	}
	
	memset(chBuffer, '\0', sizeof(chBuffer));
	iBufLenght = sizeof(chBuffer);
    if (Tool_GetDataFromAppenedInfo( pResult->pcAppendInfo, "VehicleType", chBuffer, &iBufLenght ))
	{
		int iVehType =  AnalysisVelchType(chBuffer);
        pResult->iVehTypeNo = iVehType;
    }

    memset(chBuffer, '\0', sizeof(chBuffer));
    iBufLenght = sizeof(chBuffer);
    if (Tool_GetDataFromAppenedInfo( pResult->pcAppendInfo, "Confidence", chBuffer, &iBufLenght ))
    {
        float fConfidence = 0.0;
        sscanf(chBuffer,"%f", &fConfidence);
        pResult->fConfidenceLevel = fConfidence;
    }

    memset(chBuffer, '\0', sizeof(chBuffer));
    iBufLenght = sizeof(chBuffer);
    if (Tool_GetDataFromAppenedInfo( pResult->pcAppendInfo, "CarColor", chBuffer, &iBufLenght ))
    {
        AnalysisVelchBodyColor(chBuffer, pResult->iVehBodyDeepNo, pResult->iVehBodyColorNo);
    }

    memset(chBuffer, '\0', sizeof(chBuffer));
    iBufLenght = sizeof(chBuffer);
    if (Tool_GetDataFromAppenedInfo( pResult->pcAppendInfo, "RoadNumber", chBuffer, &iBufLenght ))
    {
        int iLaneNo =  0;
        sscanf(chBuffer,"%d", &iLaneNo);
        pResult->iLaneNo = iLaneNo;
    }

    memset(chBuffer, '\0', sizeof(chBuffer));
    iBufLenght = sizeof(chBuffer);
    if (Tool_GetDataFromAppenedInfo( pResult->pcAppendInfo, "FrontScene", chBuffer, &iBufLenght ))
    {
        pResult->bFrontMode = (AnalysisFrontMode(chBuffer) == 0 ) ? true : false;
    }

    memset(chBuffer, '\0', sizeof(chBuffer));
    iBufLenght = sizeof(chBuffer);
    if (Tool_GetDataFromAppenedInfo( pResult->pcAppendInfo, "VideoScaleSpeed", chBuffer, &iBufLenght ))
    {
        int iSpeed =  0;
        sscanf(chBuffer,"%d", &iSpeed);
        pResult->iSpeed = iSpeed;
    }

    memset(chBuffer, '\0', sizeof(chBuffer));
    iBufLenght = sizeof(chBuffer);
    if (Tool_GetDataFromAppenedInfo( pResult->pcAppendInfo, "StructBrand", chBuffer, &iBufLenght ))
    {
        sprintf(pResult->chCarFace, "%s", chBuffer);
    }

    memset(chBuffer, '\0', sizeof(chBuffer));
    iBufLenght = sizeof(chBuffer);
    if (Tool_GetDataFromAppenedInfo( pResult->pcAppendInfo, "StructCarType", chBuffer, &iBufLenght ))
    {
        if(strstr(chBuffer, "货"))
        {
            pResult->iVehClassType = 2;
        }
        else
        {
            pResult->iVehClassType = 1;
        }
    }

}

void Camera6467_plate::SetResultCallbackFunc(void *pUserData, void *pFunc)
{
    m_csFuncCallback.lock();
    m_pResultUserData = pUserData;
    m_pResultCallbackFunc = pFunc;
    m_csFuncCallback.unlock();
}

void Camera6467_plate::SetConnectStatusCallbackFunc(void *pUserData, void *pFunc)
{
    m_csFuncCallback.lock();
    m_pStatusUserData = pUserData;
    m_pStatusCallbackFunc = pFunc;
    m_csFuncCallback.unlock();
}

void Camera6467_plate::SetSynTimeInerval(int intervalTime)
{
    m_csFuncCallback.lock();
    m_iSynTimInterval = intervalTime;
    m_csFuncCallback.unlock();
}

int Camera6467_plate::GetSynTimeInterval()
{
    int iTimeInterval = 30;
    m_csFuncCallback.lock();
    iTimeInterval = m_iSynTimInterval;
    m_csFuncCallback.unlock();
    return iTimeInterval;
}
