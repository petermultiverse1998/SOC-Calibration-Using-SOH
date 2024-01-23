//
// Created by peter on 11/16/2023.
//

#ifndef BATTERY_BATTERY_SOC_H
#define BATTERY_BATTERY_SOC_H

#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#define SOC_MARGIN 0.01f

typedef struct {
    float SOC;
    float newSOC;
    int change;// rising from 0% SOC=>+1, //falling from 100% SOC=>-1
} BatterySOC;

struct BatterySOCControl {
    /**
     * This initiates the parameter of battery and return instant of battery
     * @param initialSOH    : Initial SOH (0.0-1.0)
     * @param initialSOC    : Initial SOC (0.0-1.0)
     * @param initialChange : Initial change (-1:falling from 100% SOC) or (+1:rising from 0% SOC) or (0:self update after 1 charge or discharge)
     * @return              : Instant of Battery
     */
    BatterySOC (*new)(float initialSOH, float initialSOC, int initialChange);
    /**
     * This calibrates the SOC and should be called continuously
     * @param battery   : Pointer to instant of battery
     * @param SOC       : SOC given by battery
     * @param SOH       : SOH at given time
     * @return          : Calibrated SOC
     */
    float (*thread)(BatterySOC *battery, float SOC, float SOH);
};
extern struct BatterySOCControl StaticBatterySOC;

#endif //BATTERY_BATTERY_SOC_H
