#ifndef CAMERA6467_PLATE_H
#define CAMERA6467_PLATE_H

#include "BaseCamera.h"
//#include "CameraModule/ResultListManager.h"
//#include "CameraModule/ThreadSafeList.h"
#include <map>
#include"cameraModule/CameraResult.h"
//#include"c_udpclient.h"

#define MAX_THREAD_COUNT 5

class Camera6467_plate :    public BaseCamera
{
public:
    Camera6467_plate();
    virtual ~Camera6467_plate();

    bool GetOneJpegImg(CameraIMG &destImg);    

    //获取发送状态回调的时间间隔
    int GetTimeInterval();

    bool GetResultComplete();
    void SetResultComplete(bool bfinish);

    CameraResult* GetOneResult();
    bool SaveCIMG(CameraIMG& imgStruct, struct tm& dateTime, unsigned long tickCount, int iType, std::string& outputPath);
    void SaveResult();

	void SetDeviceId(unsigned int uID);
	unsigned int GetDeviceID();

    void SetMsgThreadId(unsigned long id);
    unsigned long GetMsgThreadId();

	void AnalysisAppendInfo(CameraResult* pResult);

    void SetResultCallbackFunc(void* pUserData, void* pFunc);
    void SetConnectStatusCallbackFunc(void* pUserData, void* pFunc);
	
    void SetSynTimeInerval(int intervalTime);
    int GetSynTimeInterval();
private:
    int AnalysisVelchType(const char* vehType);
    bool AnalysisVelchBodyColor(const char* srcData, int& ColorShade, int& bodyColor);
    int AnalysisFrontMode(const char* srcData );
    int AnalysisPlateColorType(const char* srcData);

    void ReadConfig();

    void SetCheckThreadExit(bool  bExit);
    bool GetCheckThreadExit();

    void SendResultByCallback();
    void SendConnetStateMsg(bool isConnect);
private:

    int m_iTimeInvl;
    int m_iCompressBigImgSize;
    int m_iCompressSamllImgSize;
	int m_iCompressBigImgWidth;
	int m_iCompressBigImgHeight;
    int m_iSynTimInterval;

	unsigned int m_iDeviceID;

    unsigned long m_dwLastCarID;
    unsigned long m_dwMsgThreadID;

    bool m_bResultComplete;
    bool m_bJpegComplete;
    bool m_bSaveToBuffer;
    bool m_bOverlay;
    bool m_bCompress;
    bool m_bStatusCheckThreadExit;
	bool m_bProvideBigImg;
	bool m_bUseHwnd;
	bool m_bCompressEnable;

    unsigned char* m_pTempBin;
    unsigned char* m_pTempBig1;
    unsigned char* m_pCaptureImg;
    unsigned char* m_pTempBig;

    char m_chResultPath[MAX_PATH];

    CameraResult* m_CameraResult;
    CameraResult* m_BufferResult;

    CameraIMG m_CIMG_StreamJPEG;

    ThreadMutex m_csResult;
    CameraIMG m_Bin_IMG_Temp;
    CameraIMG m_Small_IMG_Temp;

    void* m_pResultUserData;
    void* m_pResultCallbackFunc;

    void* m_pStatusUserData;
    void* m_pStatusCallbackFunc;

private:

    virtual int RecordInfoBegin(unsigned long dwCarID);

    virtual int RecordInfoEnd(unsigned long dwCarID);

    virtual int RecordInfoPlate(unsigned long dwCarID,
        const char* pcPlateNo,
        const char* pcAppendInfo,
        unsigned long dwRecordType,
        unsigned long long dw64TimeMS);

    virtual int RecordInfoBigImage(unsigned long dwCarID,
        unsigned short  wImgType,
        unsigned short  wWidth,
        unsigned short  wHeight,
        unsigned char* pbPicData,
        unsigned long dwImgDataLen,
        unsigned long dwRecordType,
        unsigned long long dw64TimeMS);

    virtual int RecordInfoSmallImage(unsigned long dwCarID,
        unsigned short wWidth,
        unsigned short wHeight,
        unsigned char* pbPicData,
        unsigned long dwImgDataLen,
        unsigned long dwRecordType,
        unsigned long long dw64TimeMS);

    virtual int RecordInfoBinaryImage(unsigned long dwCarID,
        unsigned short wWidth,
        unsigned short wHeight,
        unsigned char* pbPicData,
        unsigned long dwImgDataLen,
        unsigned long dwRecordType,
        unsigned long long dw64TimeMS);

    virtual int DeviceJPEGStream(
        unsigned char* pbImageData,
        unsigned long dwImageDataLen,
        unsigned long dwImageType,
        const char* szImageExtInfo);

    virtual void CheckStatus();
};

#endif
