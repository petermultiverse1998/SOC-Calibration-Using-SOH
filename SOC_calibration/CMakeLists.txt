cmake_minimum_required(VERSION 3.26)
project(SOC_calibration C)

set(CMAKE_C_STANDARD 11)

add_executable(SOC
        battery_SOC.h
        battery_SOC.c
        battery_SOC_test.c)

