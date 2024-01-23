#include <stdio.h>
#include <windows.h>
#include "math.h"
#include "battery_SOC.h"

static void copyToClipboard(const float *data, int data_len) {
    char buff[10240] = "";
    int n = 0;
    for (int i = 0; i < data_len; i++)
        n += sprintf(buff + n, "%0.2f \n", data[i]);

    const size_t len = n + 1;
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
    memcpy(GlobalLock(hMem), buff, len);
    GlobalUnlock(hMem);
    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
}


int main() {
    float SOH = 0.8f;
    float SOC[600];
    float newSOC[600];
    int lenSOC = sizeof(SOC) / sizeof(float);
    int change = -1;

    //Data generation
    for (int i = 0; i < lenSOC; ++i) {
        float currentSOC = (acosf(-cosf(0.038f*(float)i))/(float)M_PI);
        currentSOC += 0.5f*tanhf(sinf(0.038f*(float)i/3.0f));
        currentSOC = min(max(currentSOC,0.0f),1.0f);

        //SOH
        if(currentSOC<=(0.0f+SOC_MARGIN))
            change = +1;
        else if(currentSOC>=(1.0f-SOC_MARGIN))
            change = -1;
        if(currentSOC<=(1-SOH) && (change==-1))
            currentSOC = 0;
        else if(currentSOC>SOH && (change==+1))
            currentSOC = 1.0f;

        SOC[i] = currentSOC;
    }


    //Testing
    BatterySOC battery = StaticBatterySOC.new(SOH, SOC[0], -1);
    for (int i = 0; i < lenSOC; ++i) {
        newSOC[i]=StaticBatterySOC.thread(&battery, SOC[i], SOH);
    }

//    copyToClipboard(SOC, lenSOC);
    copyToClipboard(newSOC, lenSOC);


    return 0;
}

