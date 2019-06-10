#ifndef TESTFUNC_H
#define TESTFUNC_H
#include<stdio.h>

void  printf_menu();

void InitCamera();

void ConnectToCamera();

void ReleaseCameraResource();
void DisConnectCamera();

void SetResultCallBack();

void SetStatusCallback();
void TakeCapture();

void SynDeviceTime();

void GetDeviceStatus();
void GetDeviceStatusMessage();
void GetDeviceVersion();

bool SaveFileToPath(const char* path, void* pData, size_t dataSize);

#endif // TESTFUNC_H
