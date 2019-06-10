#include <stdio.h>
#include"testfunc.h"

extern bool g_bExit;
int main(int argc, char *argv[])
{
    printf("mgs_stdvlpr demo  is running....\n");

    while (!g_bExit)
        {
            printf_menu();
            printf("input a text:\n");
            char ch;
            scanf("%s", &ch);
            switch(ch)
            {
            case '1':
                InitCamera();
                break;
            case '2':
                ReleaseCameraResource();
                break;
            case '3':
                ConnectToCamera();
                break;
            case '4':
                DisConnectCamera();
                break;
            case '5':
                SetResultCallBack();
                break;
            case '6':
                SetStatusCallback();
                break;
            case '7':
                TakeCapture();
                break;
            case '8':
                SynDeviceTime();
                break;
            case '9':
                GetDeviceStatus();
                break;
            case '10':
                GetDeviceStatusMessage();
                break;
            case '11':
                GetDeviceVersion();
                break;
            case 'q':
                ReleaseCameraResource();
                g_bExit = true;
                break;
            default:
                continue;
                break;
            }
        }

        g_bExit = true;
    return 0;
}
