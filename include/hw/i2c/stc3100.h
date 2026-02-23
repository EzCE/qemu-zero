/*
 * STC3100 I2C GasGauge
 *
 * Copyright (c) 2026 TIny_Hacker
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef HW_STC3100_H
#define HW_STC3100_H

#include "hw/i2c/i2c.h"
#include "qom/object.h"

#define TYPE_STC3100 "stc3100"
OBJECT_DECLARE_SIMPLE_TYPE(STC3100State, STC3100)

#define STC3100_CHARGE  0x0CD3
#define STC3100_VOLTAGE 0x05EC
#define STC3100_CURRENT 0x0000
#define STC3100_TEMP    0x00C8

enum STC3100_Reg_Add {
    STC3100_MODE_ADD,             /* Mode register */
    STC3100_CTRL_ADD,             /* Control and status register */
    STC3100_CHARGE_LOW_ADD,       /* Gas gauge charge data, bits 0-7 */
    STC3100_CHARGE_HIGH_ADD,      /* Gas gauge charge data, bits 8-15 */
    STC3100_COUNTER_LOW_ADD,      /* Number of conversions, bits 0-7 */
    STC3100_COUNTER_HIGH_ADD,     /*  Number of conversions, bits 8-15 */
    STC3100_CURRENT_LOW_ADD,      /* Battery current value, bits 0-7 */
    STC3100_CURRENT_HIGH_ADD,     /* Battery current value, bits 8-15 */
    STC3100_VOLTAGE_LOW_ADD,      /* Battery voltage value, bits 0-7 */
    STC3100_VOLTAGE_HIGH_ADD,     /* Battery voltage value, bits 8-15 */
    STC3100_TEMPERATURE_LOW_ADD,  /* Temperature value, bits 0-7 */
    STC3100_TEMPERATURE_HIGH_ADD, /* Temperature value, bits 8-15 */
    STC3100_ID0_ADD,              /* Part type ID = 10h */
    STC3100_ID1_ADD,              /* Unique part ID, bits 0-7 */
    STC3100_ID2_ADD,              /* Unique part ID, bits 8-15 */
    STC3100_ID3_ADD,              /* Unique part ID, bits 16-23 */
    STC3100_ID4_ADD,              /* Unique part ID, bits 24-31 */
    STC3100_ID5_ADD,              /* Unique part ID, bits 32-39 */
    STC3100_ID6_ADD,              /* Unique part ID, bits 40-47 */
    STC3100_ID7_ADD,              /* Device ID CRC */
    STC3100_RAM_BASE_ADD,         /* General-purpose RAM register 0 */
    STC3100_RAM_END_ADD,          /* General-purpose RAM register 31 */
};

struct STC3100State {
    I2CSlave parent_obj;

    uint8_t reg;
    bool wait_reg;

    uint8_t mode;
    uint8_t ctrl;
    uint16_t charge;
    uint16_t counter;
    uint16_t voltage;
    uint16_t current;
    uint16_t temp;
    uint8_t ram[32];
};

#endif
