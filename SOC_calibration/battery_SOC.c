//
// Created by peter on 11/16/2023.
//

#include "battery_SOC.h"

/**
 * This initiates the parameter of battery and return instant of battery
 * @param initialSOH    : Initial SOH (0.0-1.0)
 * @param initialSOC    : Initial SOC (0.0-1.0)
 * @param initialChange : Initial change (-1:falling from 100% SOC) or (+1:rising from 0% SOC) or (0:self update after 1 charge or discharge)
 * @return              : Instant of Battery
 */
static BatterySOC new(float initialSOH, float initialSOC, int initialChange) {
    BatterySOC battery;
    battery.newSOC=initialSOC;
    battery.change = initialChange;
    return battery;
}

/**
 * This calibrates the SOC and should be called continuously
 * @param battery   : Pointer to instant of battery
 * @param SOC       : SOC given by battery
 * @param SOH       : SOH at given time
 * @return          : Calibrated SOC
 */
static float thread(BatterySOC *battery, float SOC, float SOH) {
    //Old SOC update
    battery->SOC = SOC;
    //Change detect
    if(SOC<=(0.0f+SOC_MARGIN))
        battery->change = +1;//rising from 0% SOC
    else if(SOC>=(1.0f-SOC_MARGIN))
        battery->change = -1;//falling from 100% SOC

    /*
     * SOC Calibration
     * For falling from 100% SOC    : newSOC = (SOC-(1-SOH))/SOH
     * For rising from 0% SOC       : newSOC = SOC/SOH
     */
    float newSOC = SOC;
    if(battery->change!=0)//Update only if we actually know the rising from 0% or falling from 100%
        newSOC = min(max((SOC-(1-(battery->change==-1?SOH:1)))/SOH,0),1.0f);
    battery->newSOC = newSOC;
    return newSOC;
}


struct BatterySOCControl StaticBatterySOC = {
        .new = new,
        .thread = thread
};