/*
 * GD32F470xx I2C
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

#ifndef HW_GD32F470_I2C_H
#define HW_GD32F470_I2C_H

#include "hw/core/sysbus.h"
#include "hw/i2c/i2c.h"
#include "qom/object.h"

#define TYPE_GD32F470XX_I2C "gd32f470xx-i2c"
OBJECT_DECLARE_SIMPLE_TYPE(GD32F470XXI2CState, GD32F470XX_I2C)

#define GD_I2C_FIELD(reg_val, field)    ((uint32_t)reg_val & (uint32_t)field)
#define GD_I2C_BIT(x)                   ((uint32_t)((uint32_t)0x01U<<(x)))
#define GD_I2C_BITS(start, end)         ((0xFFFFFFFFUL << (start)) & \
                                        (0xFFFFFFFFUL >> (31U - (uint32_t)(end))))

enum GD32F470xx_I2C_Reg_Add {
    I2C_CTL0_ADD   = 0x00, /* I2C control register 0 */
    I2C_CTL1_ADD   = 0x04, /* I2C control register 1 */
    I2C_SADDR0_ADD = 0x08, /* I2C slave address register 0 */
    I2C_SADDR1_ADD = 0x0C, /* I2C slave address register 1 */
    I2C_DATA_ADD   = 0x10, /* I2C transfer buffer register */
    I2C_STAT0_ADD  = 0x14, /* I2C transfer status register 0 */
    I2C_STAT1_ADD  = 0x18, /* I2C transfer status register */
    I2C_CKCFG_ADD  = 0x1C, /* I2C clock configure register */
    I2C_RT_ADD     = 0x20, /* I2C rise time register */
    I2C_FCTL_ADD   = 0x24, /* I2C filter control register */
    I2C_SAMCS_ADD  = 0x80, /* I2C SAM control and status register */
};

enum GD32F470xx_I2C_Reg {
    I2C_CTL0,   /* I2C control register 0 */
    I2C_CTL1,   /* I2C control register 1 */
    I2C_SADDR0, /* I2C slave address register 0 */
    I2C_SADDR1, /* I2C slave address register 1 */
    I2C_DATA,   /* I2C transfer buffer register */
    I2C_STAT0,  /* I2C transfer status register 0 */
    I2C_STAT1,  /* I2C transfer status register */
    I2C_CKCFG,  /* I2C clock configure register */
    I2C_RT,     /* I2C rise time register */
    I2C_FCTL,   /* I2C filter control register */
    I2C_SAMCS,  /* I2C SAM control and status register */
};

/* I2Cx_CTL0 */
enum GD32F470xx_I2C_CTL0_Field {
    I2C_CTL0_I2CEN             = GD_I2C_BIT(0),
    I2C_CTL0_SMBEN             = GD_I2C_BIT(1),
    I2C_CTL0_SMBSEL            = GD_I2C_BIT(3),
    I2C_CTL0_ARPEN             = GD_I2C_BIT(4),
    I2C_CTL0_PECEN             = GD_I2C_BIT(5),
    I2C_CTL0_GCEN              = GD_I2C_BIT(6),
    I2C_CTL0_SS                = GD_I2C_BIT(7),
    I2C_CTL0_START             = GD_I2C_BIT(8),
    I2C_CTL0_STOP              = GD_I2C_BIT(9),
    I2C_CTL0_ACKEN             = GD_I2C_BIT(10),
    I2C_CTL0_POAP              = GD_I2C_BIT(11),
    I2C_CTL0_PECTRANS          = GD_I2C_BIT(12),
    I2C_CTL0_SALT              = GD_I2C_BIT(13),
    I2C_CTL0_SRESET            = GD_I2C_BIT(15),
};

/* I2Cx_CTL1 */
enum GD32F470xx_I2C_CTL1_Field {
    I2C_CTL1_I2CCLK            = GD_I2C_BITS(0,5),
    I2C_CTL1_ERRIE             = GD_I2C_BIT(8),
    I2C_CTL1_EVIE              = GD_I2C_BIT(9),
    I2C_CTL1_BUFIE             = GD_I2C_BIT(10),
    I2C_CTL1_DMAON             = GD_I2C_BIT(11),
    I2C_CTL1_DMALST            = GD_I2C_BIT(12),
};

/* I2Cx_SADDR0 */
enum GD32F470xx_I2C_SADDR0_Field {
    I2C_SADDR0_ADDRESS0        = GD_I2C_BIT(0),
    I2C_SADDR0_ADDRESS         = GD_I2C_BITS(1,7),
    I2C_SADDR0_ADDRESS_H       = GD_I2C_BITS(8,9),
    I2C_SADDR0_ADDFORMAT       = GD_I2C_BIT(15),
};

/* I2Cx_SADDR1 */
enum GD32F470xx_I2C_SADDR1_Field {
    I2C_SADDR1_DUADEN          = GD_I2C_BIT(0),
    I2C_SADDR1_ADDRESS2        = GD_I2C_BITS(1,7),
};

/* I2Cx_DATA */
enum GD32F470xx_I2C_DATA_Field {
    I2C_DATA_TRB               = GD_I2C_BITS(0,7),
};

/* I2Cx_STAT0 */
enum GD32F470xx_I2C_STAT0_Field {
    I2C_STAT0_SBSEND           = GD_I2C_BIT(0),
    I2C_STAT0_ADDSEND          = GD_I2C_BIT(1),
    I2C_STAT0_BTC              = GD_I2C_BIT(2),
    I2C_STAT0_ADD10SEND        = GD_I2C_BIT(3),
    I2C_STAT0_STPDET           = GD_I2C_BIT(4),
    I2C_STAT0_RBNE             = GD_I2C_BIT(6),
    I2C_STAT0_TBE              = GD_I2C_BIT(7),
    I2C_STAT0_BERR             = GD_I2C_BIT(8),
    I2C_STAT0_LOSTARB          = GD_I2C_BIT(9),
    I2C_STAT0_AERR             = GD_I2C_BIT(10),
    I2C_STAT0_OUERR            = GD_I2C_BIT(11),
    I2C_STAT0_PECERR           = GD_I2C_BIT(12),
    I2C_STAT0_SMBTO            = GD_I2C_BIT(14),
    I2C_STAT0_SMBALT           = GD_I2C_BIT(15),
};

#define BUFIE_MASK (I2C_STAT0_TBE | I2C_STAT0_RBNE)

#define EVIE_MASK  (I2C_STAT0_ADDSEND | \
                    I2C_STAT0_ADD10SEND | \
                    I2C_STAT0_BTC | \
                    I2C_STAT0_SBSEND | \
                    I2C_STAT0_STPDET)

#define ERRIE_MASK (I2C_STAT0_BERR | \
                    I2C_STAT0_LOSTARB | \
                    I2C_STAT0_AERR | \
                    I2C_STAT0_OUERR | \
                    I2C_STAT0_PECERR | \
                    I2C_STAT0_SMBTO | \
                    I2C_STAT0_SMBALT)

/* I2Cx_STAT1 */
enum GD32F470xx_I2C_STAT1_Field {
    I2C_STAT1_MASTER           = GD_I2C_BIT(0),
    I2C_STAT1_I2CBSY           = GD_I2C_BIT(1),
    I2C_STAT1_TR               = GD_I2C_BIT(2),
    I2C_STAT1_RXGC             = GD_I2C_BIT(4),
    I2C_STAT1_DEFSMB           = GD_I2C_BIT(5),
    I2C_STAT1_HSTSMB           = GD_I2C_BIT(6),
    I2C_STAT1_DUMODF           = GD_I2C_BIT(7),
    I2C_STAT1_PECV             = GD_I2C_BITS(8,15),
};

/* I2Cx_CKCFG */
enum GD32F470xx_I2C_CKCFG_Field {
    I2C_CKCFG_CLKC             = GD_I2C_BITS(0,11),
    I2C_CKCFG_DTCY             = GD_I2C_BIT(14),
    I2C_CKCFG_FAST             = GD_I2C_BIT(15),
};

/* I2Cx_RT */
enum GD32F470xx_I2C_RT_Field {
    I2C_RT_RISETIME            = GD_I2C_BITS(0,5),
};

/* I2Cx_FCTL */
enum GD32F470xx_I2C_FCTL_Field {
    I2C_FCTL_DF                = GD_I2C_BITS(0,3),
    I2C_FCTL_AFD               = GD_I2C_BIT(4),
};

/* I2Cx_SAMCS */
enum GD32F470xx_I2C_SAMCS_Field {
    I2C_SAMCS_SAMEN            = GD_I2C_BIT(0),
    I2C_SAMCS_STOEN            = GD_I2C_BIT(1),
    I2C_SAMCS_TFFIE            = GD_I2C_BIT(4),
    I2C_SAMCS_TFRIE            = GD_I2C_BIT(5),
    I2C_SAMCS_RFFIE            = GD_I2C_BIT(6),
    I2C_SAMCS_RFRIE            = GD_I2C_BIT(7),
    I2C_SAMCS_TXF              = GD_I2C_BIT(8),
    I2C_SAMCS_RXF              = GD_I2C_BIT(9),
    I2C_SAMCS_TFF              = GD_I2C_BIT(12),
    I2C_SAMCS_TFR              = GD_I2C_BIT(13),
    I2C_SAMCS_RFF              = GD_I2C_BIT(14),
    I2C_SAMCS_RFR              = GD_I2C_BIT(15),
};

struct GD32F470XXI2CState {
    /* <private> */
    SysBusDevice parent_obj;

    /* <public> */
    MemoryRegion mmio;
    I2CBus *bus;
    qemu_irq irq[2];

    uint8_t address;
    uint8_t read_mode;

    uint32_t i2c_reg[11];
    int8_t last_read;
    int8_t last_wrote;
};

#endif
