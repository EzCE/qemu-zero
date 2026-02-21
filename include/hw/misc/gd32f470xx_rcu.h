/*
 * GD32F470xx RCU
 *
 * Copyright (c) 2026 TIny_Hacker
 * Copyright (c) 2022 Jean-Baptiste Boric <jblbeurope@gmail.com>
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

#ifndef HW_GD32F470_RCU_H
#define HW_GD32F470_RCU_H

#include "hw/core/sysbus.h"
#include "qom/object.h"

#define TYPE_GD32F470XX_RCU "gd32f470xx-rcu"
OBJECT_DECLARE_SIMPLE_TYPE(GD32F470XXRcuState, GD32F470XX_RCU)

#define GD_RCU_FIELD(reg_val, field)    ((uint32_t)reg_val & (uint32_t)field)
#define GD_RCU_BIT(x)                   ((uint32_t)((uint32_t)0x01U<<(x)))
#define GD_RCU_BITS(start, end)         ((0xFFFFFFFFUL << (start)) & \
                                        (0xFFFFFFFFUL >> (31U - (uint32_t)(end))))

enum GD32F470xx_RCU_Reg_Add {
    RCU_CTL_ADD         = 0x00,  /* control register */
    RCU_PLL_ADD         = 0x04,  /* PLL register */
    RCU_CFG0_ADD        = 0x08,  /* clock configuration register 0 */
    RCU_INT_ADD         = 0x0C,  /* clock interrupt register */
    RCU_AHB1RST_ADD     = 0x10,  /* AHB1 reset register */
    RCU_AHB2RST_ADD     = 0x14,  /* AHB2 reset register */
    RCU_AHB3RST_ADD     = 0x18,  /* AHB3 reset register */
    RCU_APB1RST_ADD     = 0x20,  /* APB1 reset register */
    RCU_APB2RST_ADD     = 0x24,  /* APB2 reset register */
    RCU_AHB1EN_ADD      = 0x30,  /* AHB1 enable register */
    RCU_AHB2EN_ADD      = 0x34,  /* AHB2 enable register */
    RCU_AHB3EN_ADD      = 0x38,  /* AHB3 enable register */
    RCU_APB1EN_ADD      = 0x40,  /* APB1 enable register */
    RCU_APB2EN_ADD      = 0x44,  /* APB2 enable register */
    RCU_AHB1SPEN_ADD    = 0x50,  /* AHB1 sleep mode enable register */
    RCU_AHB2SPEN_ADD    = 0x54,  /* AHB2 sleep mode enable register */
    RCU_AHB3SPEN_ADD    = 0x58,  /* AHB3 sleep mode enable register */ 
    RCU_APB1SPEN_ADD    = 0x60,  /* APB1 sleep mode enable register */
    RCU_APB2SPEN_ADD    = 0x64,  /* APB2 sleep mode enable register */
    RCU_BDCTL_ADD       = 0x70,  /* backup domain control register */
    RCU_RSTSCK_ADD      = 0x74,  /* reset source / clock register */
    RCU_PLLSSCTL_ADD    = 0x80,  /* PLL clock spread spectrum control register */
    RCU_PLLI2S_ADD      = 0x84,  /* PLLI2S register */ 
    RCU_PLLSAI_ADD      = 0x88,  /* PLLSAI register */ 
    RCU_CFG1_ADD        = 0x8C,  /* clock configuration register 1 */
    RCU_ADDCTL_ADD      = 0xC0,  /* Additional clock control register */
    RCU_ADDINT_ADD      = 0xCC,  /* Additional clock interrupt register */
    RCU_ADDAPB1RST_ADD  = 0xE0,  /* APB1 additional reset register */
    RCU_ADDAPB1EN_ADD   = 0xE4,  /* APB1 additional enable register */
    RCU_ADDAPB1SPEN_ADD = 0xE8,  /* APB1 additional sleep mode enable register */
    RCU_VKEY_ADD        = 0x100, /* voltage key register */
    RCU_DSV_ADD         = 0x134, /* deep-sleep mode voltage register */
};

enum GD32F470xx_RCU_Reg {
    RCU_CTL,         /* control register */
    RCU_PLL,         /* PLL register */
    RCU_CFG0,        /* clock configuration register 0 */
    RCU_INT,         /* clock interrupt register */
    RCU_AHB1RST,     /* AHB1 reset register */
    RCU_AHB2RST,     /* AHB2 reset register */
    RCU_AHB3RST,     /* AHB3 reset register */
    RCU_APB1RST,     /* APB1 reset register */
    RCU_APB2RST,     /* APB2 reset register */
    RCU_AHB1EN,      /* AHB1 enable register */
    RCU_AHB2EN,      /* AHB2 enable register */
    RCU_AHB3EN,      /* AHB3 enable register */
    RCU_APB1EN,      /* APB1 enable register */
    RCU_APB2EN,      /* APB2 enable register */
    RCU_AHB1SPEN,    /* AHB1 sleep mode enable register */
    RCU_AHB2SPEN,    /* AHB2 sleep mode enable register */
    RCU_AHB3SPEN,    /* AHB3 sleep mode enable register */ 
    RCU_APB1SPEN,    /* APB1 sleep mode enable register */
    RCU_APB2SPEN,    /* APB2 sleep mode enable register */
    RCU_BDCTL,       /* backup domain control register */
    RCU_RSTSCK,      /* reset source / clock register */
    RCU_PLLSSCTL,    /* PLL clock spread spectrum control register */
    RCU_PLLI2S,      /* PLLI2S register */ 
    RCU_PLLSAI,      /* PLLSAI register */ 
    RCU_CFG1,        /* clock configuration register 1 */
    RCU_ADDCTL,      /* Additional clock control register */
    RCU_ADDINT,      /* Additional clock interrupt register */
    RCU_ADDAPB1RST,  /* APB1 additional reset register */
    RCU_ADDAPB1EN,   /* APB1 additional enable register */
    RCU_ADDAPB1SPEN, /* APB1 additional sleep mode enable register */
    RCU_VKEY,        /* voltage key register */
    RCU_DSV,         /* deep-sleep mode voltage register */
};

/* bits definitions */
/* RCU_CTL */
enum GD32F470xx_RCU_CTL_Field {
    RCU_CTL_IRC16MEN              = GD_RCU_BIT(0),
    RCU_CTL_IRC16MSTB             = GD_RCU_BIT(1),
    RCU_CTL_IRC16MADJ             = GD_RCU_BITS(3,7),
    RCU_CTL_IRC16MCALIB           = GD_RCU_BITS(8,15),
    RCU_CTL_HXTALEN               = GD_RCU_BIT(16),
    RCU_CTL_HXTALSTB              = GD_RCU_BIT(17),
    RCU_CTL_HXTALBPS              = GD_RCU_BIT(18),
    RCU_CTL_CKMEN                 = GD_RCU_BIT(19),
    RCU_CTL_PLLEN                 = GD_RCU_BIT(24),
    RCU_CTL_PLLSTB                = GD_RCU_BIT(25),
    RCU_CTL_PLLI2SEN              = GD_RCU_BIT(26),
    RCU_CTL_PLLI2SSTB             = GD_RCU_BIT(27),
    RCU_CTL_PLLSAIEN              = GD_RCU_BIT(28),
    RCU_CTL_PLLSAISTB             = GD_RCU_BIT(29),
};

/* RCU_PLL */
enum GD32F470xx_RCU_PLL_Field {
    RCU_PLL_PLLPSC                = GD_RCU_BITS(0,5),
    RCU_PLL_PLLN                  = GD_RCU_BITS(6,14),
    RCU_PLL_PLLP                  = GD_RCU_BITS(16,17),
    RCU_PLL_PLLSEL                = GD_RCU_BIT(22),
    RCU_PLL_PLLQ                  = GD_RCU_BITS(24,27),
};

/* RCU_CFG0 */
enum GD32F470xx_RCU_CFG0_Field {
    RCU_CFG0_SCS                  = GD_RCU_BITS(0,1),
    RCU_CFG0_SCSS                 = GD_RCU_BITS(2,3),
    RCU_CFG0_AHBPSC               = GD_RCU_BITS(4,7),
    RCU_CFG0_APB1PSC              = GD_RCU_BITS(10,12),
    RCU_CFG0_APB2PSC              = GD_RCU_BITS(13,15),
    RCU_CFG0_RTCDIV               = GD_RCU_BITS(16,20),
    RCU_CFG0_CKOUT0SEL            = GD_RCU_BITS(21,22),
    RCU_CFG0_I2SSEL               = GD_RCU_BIT(23),
    RCU_CFG0_CKOUT0DIV            = GD_RCU_BITS(24,26),
    RCU_CFG0_CKOUT1DIV            = GD_RCU_BITS(27,29),
    RCU_CFG0_CKOUT1SEL            = GD_RCU_BITS(30,31),
};

/* RCU_INT */
enum GD32F470xx_RCU_INT_Field {
    RCU_INT_IRC32KSTBIF           = GD_RCU_BIT(0),
    RCU_INT_LXTALSTBIF            = GD_RCU_BIT(1),
    RCU_INT_IRC16MSTBIF           = GD_RCU_BIT(2),
    RCU_INT_HXTALSTBIF            = GD_RCU_BIT(3),
    RCU_INT_PLLSTBIF              = GD_RCU_BIT(4),
    RCU_INT_PLLI2SSTBIF           = GD_RCU_BIT(5),
    RCU_INT_PLLSAISTBIF           = GD_RCU_BIT(6),
    RCU_INT_CKMIF                 = GD_RCU_BIT(7),
    RCU_INT_IRC32KSTBIE           = GD_RCU_BIT(8),
    RCU_INT_LXTALSTBIE            = GD_RCU_BIT(9),
    RCU_INT_IRC16MSTBIE           = GD_RCU_BIT(10),
    RCU_INT_HXTALSTBIE            = GD_RCU_BIT(11),
    RCU_INT_PLLSTBIE              = GD_RCU_BIT(12),
    RCU_INT_PLLI2SSTBIE           = GD_RCU_BIT(13),
    RCU_INT_PLLSAISTBIE           = GD_RCU_BIT(14),
    RCU_INT_IRC32KSTBIC           = GD_RCU_BIT(16),
    RCU_INT_LXTALSTBIC            = GD_RCU_BIT(17),
    RCU_INT_IRC16MSTBIC           = GD_RCU_BIT(18),
    RCU_INT_HXTALSTBIC            = GD_RCU_BIT(19),
    RCU_INT_PLLSTBIC              = GD_RCU_BIT(20),
    RCU_INT_PLLI2SSTBIC           = GD_RCU_BIT(21),
    RCU_INT_PLLSAISTBIC           = GD_RCU_BIT(22),
    RCU_INT_CKMIC                 = GD_RCU_BIT(23),
};

/* RCU_AHB1RST */
enum GD32F470xx_RCU_AHB1RST_Field {
    RCU_AHB1RST_PARST             = GD_RCU_BIT(0),
    RCU_AHB1RST_PBRST             = GD_RCU_BIT(1),
    RCU_AHB1RST_PCRST             = GD_RCU_BIT(2),
    RCU_AHB1RST_PDRST             = GD_RCU_BIT(3),
    RCU_AHB1RST_PERST             = GD_RCU_BIT(4),
    RCU_AHB1RST_PFRST             = GD_RCU_BIT(5),
    RCU_AHB1RST_PGRST             = GD_RCU_BIT(6),
    RCU_AHB1RST_PHRST             = GD_RCU_BIT(7),
    RCU_AHB1RST_PIRST             = GD_RCU_BIT(8),
    RCU_AHB1RST_CRCRST            = GD_RCU_BIT(12),
    RCU_AHB1RST_DMA0RST           = GD_RCU_BIT(21),
    RCU_AHB1RST_DMA1RST           = GD_RCU_BIT(22),
    RCU_AHB1RST_IPARST            = GD_RCU_BIT(23),
    RCU_AHB1RST_ENETRST           = GD_RCU_BIT(25),
    RCU_AHB1RST_USBHSRST          = GD_RCU_BIT(29),
};

/* RCU_AHB2RST */
enum GD32F470xx_RCU_AHB2RST_Field {
    RCU_AHB2RST_DCIRST            = GD_RCU_BIT(0),
    RCU_AHB2RST_TRNGRST           = GD_RCU_BIT(6),
    RCU_AHB2RST_USBFSRST          = GD_RCU_BIT(7),
};

/* RCU_AHB3RST */
enum GD32F470xx_RCU_AHB3RST_Field {
    RCU_AHB3RST_EXMCRST           = GD_RCU_BIT(0),
};

/* RCU_APB1RST */
enum GD32F470xx_RCU_APB1RST_Field {
    RCU_APB1RST_TIMER1RST         = GD_RCU_BIT(0),
    RCU_APB1RST_TIMER2RST         = GD_RCU_BIT(1),
    RCU_APB1RST_TIMER3RST         = GD_RCU_BIT(2),
    RCU_APB1RST_TIMER4RST         = GD_RCU_BIT(3),
    RCU_APB1RST_TIMER5RST         = GD_RCU_BIT(4),
    RCU_APB1RST_TIMER6RST         = GD_RCU_BIT(5),
    RCU_APB1RST_TIMER11RST        = GD_RCU_BIT(6),
    RCU_APB1RST_TIMER12RST        = GD_RCU_BIT(7),
    RCU_APB1RST_TIMER13RST        = GD_RCU_BIT(8),
    RCU_APB1RST_WWDGTRST          = GD_RCU_BIT(11),
    RCU_APB1RST_SPI1RST           = GD_RCU_BIT(14),
    RCU_APB1RST_SPI2RST           = GD_RCU_BIT(15),
    RCU_APB1RST_USART1RST         = GD_RCU_BIT(17),
    RCU_APB1RST_USART2RST         = GD_RCU_BIT(18),
    RCU_APB1RST_UART3RST          = GD_RCU_BIT(19),
    RCU_APB1RST_UART4RST          = GD_RCU_BIT(20),
    RCU_APB1RST_I2C0RST           = GD_RCU_BIT(21),
    RCU_APB1RST_I2C1RST           = GD_RCU_BIT(22),
    RCU_APB1RST_I2C2RST           = GD_RCU_BIT(23),
    RCU_APB1RST_CAN0RST           = GD_RCU_BIT(25),
    RCU_APB1RST_CAN1RST           = GD_RCU_BIT(26),
    RCU_APB1RST_PMURST            = GD_RCU_BIT(28),
    RCU_APB1RST_DACRST            = GD_RCU_BIT(29),
    RCU_APB1RST_UART6RST          = GD_RCU_BIT(30),
    RCU_APB1RST_UART7RST          = GD_RCU_BIT(31),
};

/* RCU_APB2RST */
enum GD32F470xx_RCU_APB2RST_Field {
    RCU_APB2RST_TIMER0RST         = GD_RCU_BIT(0),
    RCU_APB2RST_TIMER7RST         = GD_RCU_BIT(1),
    RCU_APB2RST_USART0RST         = GD_RCU_BIT(4),
    RCU_APB2RST_USART5RST         = GD_RCU_BIT(5),
    RCU_APB2RST_ADCRST            = GD_RCU_BIT(8),
    RCU_APB2RST_SDIORST           = GD_RCU_BIT(11),
    RCU_APB2RST_SPI0RST           = GD_RCU_BIT(12),
    RCU_APB2RST_SPI3RST           = GD_RCU_BIT(13),
    RCU_APB2RST_SYSCFGRST         = GD_RCU_BIT(14),
    RCU_APB2RST_TIMER8RST         = GD_RCU_BIT(16),
    RCU_APB2RST_TIMER9RST         = GD_RCU_BIT(17),
    RCU_APB2RST_TIMER10RST        = GD_RCU_BIT(18),
    RCU_APB2RST_SPI4RST           = GD_RCU_BIT(20),
    RCU_APB2RST_SPI5RST           = GD_RCU_BIT(21),
    RCU_APB2RST_TLIRST            = GD_RCU_BIT(26),
};

/* RCU_AHB1EN */
enum GD32F470xx_RCU_AHB1EN_Field {
    RCU_AHB1EN_PAEN               = GD_RCU_BIT(0),
    RCU_AHB1EN_PBEN               = GD_RCU_BIT(1),
    RCU_AHB1EN_PCEN               = GD_RCU_BIT(2),
    RCU_AHB1EN_PDEN               = GD_RCU_BIT(3),
    RCU_AHB1EN_PEEN               = GD_RCU_BIT(4),
    RCU_AHB1EN_PFEN               = GD_RCU_BIT(5),
    RCU_AHB1EN_PGEN               = GD_RCU_BIT(6),
    RCU_AHB1EN_PHEN               = GD_RCU_BIT(7),
    RCU_AHB1EN_PIEN               = GD_RCU_BIT(8),
    RCU_AHB1EN_CRCEN              = GD_RCU_BIT(12),
    RCU_AHB1EN_BKPSRAMEN          = GD_RCU_BIT(18),
    RCU_AHB1EN_TCMSRAMEN          = GD_RCU_BIT(20),
    RCU_AHB1EN_DMA0EN             = GD_RCU_BIT(21),
    RCU_AHB1EN_DMA1EN             = GD_RCU_BIT(22),
    RCU_AHB1EN_IPAEN              = GD_RCU_BIT(23),
    RCU_AHB1EN_ENETEN             = GD_RCU_BIT(25),
    RCU_AHB1EN_ENETTXEN           = GD_RCU_BIT(26),
    RCU_AHB1EN_ENETRXEN           = GD_RCU_BIT(27),
    RCU_AHB1EN_ENETPTPEN          = GD_RCU_BIT(28),
    RCU_AHB1EN_USBHSEN            = GD_RCU_BIT(29),
    RCU_AHB1EN_USBHSULPIEN        = GD_RCU_BIT(30),
};

/* RCU_AHB2EN */
enum GD32F470xx_RCU_AHB2EN_Field {
    RCU_AHB2EN_DCIEN              = GD_RCU_BIT(0),
    RCU_AHB2EN_TRNGEN             = GD_RCU_BIT(6),
    RCU_AHB2EN_USBFSEN            = GD_RCU_BIT(7),
};

/* RCU_AHB3EN */
enum GD32F470xx_RCU_AHB3EN_Field {
    RCU_AHB3EN_EXMCEN             = GD_RCU_BIT(0),
};

/* RCU_APB1EN */
enum GD32F470xx_RCU_APB1EN_Field {
    RCU_APB1EN_TIMER1EN           = GD_RCU_BIT(0),
    RCU_APB1EN_TIMER2EN           = GD_RCU_BIT(1),
    RCU_APB1EN_TIMER3EN           = GD_RCU_BIT(2),
    RCU_APB1EN_TIMER4EN           = GD_RCU_BIT(3),
    RCU_APB1EN_TIMER5EN           = GD_RCU_BIT(4),
    RCU_APB1EN_TIMER6EN           = GD_RCU_BIT(5),
    RCU_APB1EN_TIMER11EN          = GD_RCU_BIT(6),
    RCU_APB1EN_TIMER12EN          = GD_RCU_BIT(7),
    RCU_APB1EN_TIMER13EN          = GD_RCU_BIT(8),
    RCU_APB1EN_WWDGTEN            = GD_RCU_BIT(11),
    RCU_APB1EN_SPI1EN             = GD_RCU_BIT(14),
    RCU_APB1EN_SPI2EN             = GD_RCU_BIT(15),
    RCU_APB1EN_USART1EN           = GD_RCU_BIT(17),
    RCU_APB1EN_USART2EN           = GD_RCU_BIT(18),
    RCU_APB1EN_UART3EN            = GD_RCU_BIT(19),
    RCU_APB1EN_UART4EN            = GD_RCU_BIT(20),
    RCU_APB1EN_I2C0EN             = GD_RCU_BIT(21),
    RCU_APB1EN_I2C1EN             = GD_RCU_BIT(22),
    RCU_APB1EN_I2C2EN             = GD_RCU_BIT(23),
    RCU_APB1EN_CAN0EN             = GD_RCU_BIT(25),
    RCU_APB1EN_CAN1EN             = GD_RCU_BIT(26),
    RCU_APB1EN_PMUEN              = GD_RCU_BIT(28),
    RCU_APB1EN_DACEN              = GD_RCU_BIT(29),
    RCU_APB1EN_UART6EN            = GD_RCU_BIT(30),
    RCU_APB1EN_UART7EN            = GD_RCU_BIT(31),
};

/* RCU_APB2EN */
enum GD32F470xx_RCU_APB2EN_Field {
    RCU_APB2EN_TIMER0EN           = GD_RCU_BIT(0),
    RCU_APB2EN_TIMER7EN           = GD_RCU_BIT(1),
    RCU_APB2EN_USART0EN           = GD_RCU_BIT(4),
    RCU_APB2EN_USART5EN           = GD_RCU_BIT(5),
    RCU_APB2EN_ADC0EN             = GD_RCU_BIT(8),
    RCU_APB2EN_ADC1EN             = GD_RCU_BIT(9),
    RCU_APB2EN_ADC2EN             = GD_RCU_BIT(10),
    RCU_APB2EN_SDIOEN             = GD_RCU_BIT(11),
    RCU_APB2EN_SPI0EN             = GD_RCU_BIT(12),
    RCU_APB2EN_SPI3EN             = GD_RCU_BIT(13),
    RCU_APB2EN_SYSCFGEN           = GD_RCU_BIT(14),
    RCU_APB2EN_TIMER8EN           = GD_RCU_BIT(16),
    RCU_APB2EN_TIMER9EN           = GD_RCU_BIT(17),
    RCU_APB2EN_TIMER10EN          = GD_RCU_BIT(18),
    RCU_APB2EN_SPI4EN             = GD_RCU_BIT(20),
    RCU_APB2EN_SPI5EN             = GD_RCU_BIT(21),
    RCU_APB2EN_TLIEN              = GD_RCU_BIT(26),
};

/* RCU_AHB1SPEN */
enum GD32F470xx_RCU_AHB1SPEN_Field {
    RCU_AHB1SPEN_PASPEN           = GD_RCU_BIT(0),
    RCU_AHB1SPEN_PBSPEN           = GD_RCU_BIT(1),
    RCU_AHB1SPEN_PCSPEN           = GD_RCU_BIT(2),
    RCU_AHB1SPEN_PDSPEN           = GD_RCU_BIT(3),
    RCU_AHB1SPEN_PESPEN           = GD_RCU_BIT(4),
    RCU_AHB1SPEN_PFSPEN           = GD_RCU_BIT(5),
    RCU_AHB1SPEN_PGSPEN           = GD_RCU_BIT(6),
    RCU_AHB1SPEN_PHSPEN           = GD_RCU_BIT(7),
    RCU_AHB1SPEN_PISPEN           = GD_RCU_BIT(8),
    RCU_AHB1SPEN_CRCSPEN          = GD_RCU_BIT(12),
    RCU_AHB1SPEN_FMCSPEN          = GD_RCU_BIT(15),
    RCU_AHB1SPEN_SRAM0SPEN        = GD_RCU_BIT(16),
    RCU_AHB1SPEN_SRAM1SPEN        = GD_RCU_BIT(17),
    RCU_AHB1SPEN_BKPSRAMSPEN      = GD_RCU_BIT(18),
    RCU_AHB1SPEN_SRAM2SPEN        = GD_RCU_BIT(19),
    RCU_AHB1SPEN_DMA0SPEN         = GD_RCU_BIT(21),
    RCU_AHB1SPEN_DMA1SPEN         = GD_RCU_BIT(22),
    RCU_AHB1SPEN_IPASPEN          = GD_RCU_BIT(23),
    RCU_AHB1SPEN_ENETSPEN         = GD_RCU_BIT(25),
    RCU_AHB1SPEN_ENETTXSPEN       = GD_RCU_BIT(26),
    RCU_AHB1SPEN_ENETRXSPEN       = GD_RCU_BIT(27),
    RCU_AHB1SPEN_ENETPTPSPEN      = GD_RCU_BIT(28),
    RCU_AHB1SPEN_USBHSSPEN        = GD_RCU_BIT(29),
    RCU_AHB1SPEN_USBHSULPISPEN    = GD_RCU_BIT(30),
};

/* RCU_AHB2SPEN */
enum GD32F470xx_RCU_AHB2SPEN_Field {
    RCU_AHB2SPEN_DCISPEN          = GD_RCU_BIT(0),
    RCU_AHB2SPEN_TRNGSPEN         = GD_RCU_BIT(6),
    RCU_AHB2SPEN_USBFSSPEN        = GD_RCU_BIT(7),
};

/* RCU_AHB3SPEN */
enum GD32F470xx_RCU_EXMCSPEN_Field {
    RCU_AHB3SPEN_EXMCSPEN         = GD_RCU_BIT(0),
};

/* RCU_APB1SPEN */
enum GD32F470xx_RCU_APB1SPEN_Field {
    RCU_APB1SPEN_TIMER1SPEN       = GD_RCU_BIT(0),
    RCU_APB1SPEN_TIMER2SPEN       = GD_RCU_BIT(1),
    RCU_APB1SPEN_TIMER3SPEN       = GD_RCU_BIT(2),
    RCU_APB1SPEN_TIMER4SPEN       = GD_RCU_BIT(3),
    RCU_APB1SPEN_TIMER5SPEN       = GD_RCU_BIT(4),
    RCU_APB1SPEN_TIMER6SPEN       = GD_RCU_BIT(5),
    RCU_APB1SPEN_TIMER11SPEN      = GD_RCU_BIT(6),
    RCU_APB1SPEN_TIMER12SPEN      = GD_RCU_BIT(7),
    RCU_APB1SPEN_TIMER13SPEN      = GD_RCU_BIT(8),
    RCU_APB1SPEN_WWDGTSPEN        = GD_RCU_BIT(11),
    RCU_APB1SPEN_SPI1SPEN         = GD_RCU_BIT(14),
    RCU_APB1SPEN_SPI2SPEN         = GD_RCU_BIT(15),
    RCU_APB1SPEN_USART1SPEN       = GD_RCU_BIT(17),
    RCU_APB1SPEN_USART2SPEN       = GD_RCU_BIT(18),
    RCU_APB1SPEN_UART3SPEN        = GD_RCU_BIT(19),
    RCU_APB1SPEN_UART4SPEN        = GD_RCU_BIT(20),
    RCU_APB1SPEN_I2C0SPEN         = GD_RCU_BIT(21),
    RCU_APB1SPEN_I2C1SPEN         = GD_RCU_BIT(22),
    RCU_APB1SPEN_I2C2SPEN         = GD_RCU_BIT(23),
    RCU_APB1SPEN_CAN0SPEN         = GD_RCU_BIT(25),
    RCU_APB1SPEN_CAN1SPEN         = GD_RCU_BIT(26),
    RCU_APB1SPEN_PMUSPEN          = GD_RCU_BIT(28),
    RCU_APB1SPEN_DACSPEN          = GD_RCU_BIT(29),
    RCU_APB1SPEN_UART6SPEN        = GD_RCU_BIT(30),
    RCU_APB1SPEN_UART7SPEN        = GD_RCU_BIT(31),
};

/* RCU_APB2SPEN */
enum GD32F470xx_RCU_APB2SPEN_Field {
    RCU_APB2SPEN_TIMER0SPEN       = GD_RCU_BIT(0),
    RCU_APB2SPEN_TIMER7SPEN       = GD_RCU_BIT(1),
    RCU_APB2SPEN_USART0SPEN       = GD_RCU_BIT(4),
    RCU_APB2SPEN_USART5SPEN       = GD_RCU_BIT(5),
    RCU_APB2SPEN_ADC0SPEN         = GD_RCU_BIT(8),
    RCU_APB2SPEN_ADC1SPEN         = GD_RCU_BIT(9),
    RCU_APB2SPEN_ADC2SPEN         = GD_RCU_BIT(10),
    RCU_APB2SPEN_SDIOSPEN         = GD_RCU_BIT(11),
    RCU_APB2SPEN_SPI0SPEN         = GD_RCU_BIT(12),
    RCU_APB2SPEN_SPI3SPEN         = GD_RCU_BIT(13),
    RCU_APB2SPEN_SYSCFGSPEN       = GD_RCU_BIT(14),
    RCU_APB2SPEN_TIMER8SPEN       = GD_RCU_BIT(16),
    RCU_APB2SPEN_TIMER9SPEN       = GD_RCU_BIT(17),
    RCU_APB2SPEN_TIMER10SPEN      = GD_RCU_BIT(18),
    RCU_APB2SPEN_SPI4SPEN         = GD_RCU_BIT(20),
    RCU_APB2SPEN_SPI5SPEN         = GD_RCU_BIT(21),
    RCU_APB2SPEN_TLISPEN          = GD_RCU_BIT(26),
};

/* RCU_BDCTL */
enum GD32F470xx_RCU_BDCTL_Field {
    RCU_BDCTL_LXTALEN             = GD_RCU_BIT(0),
    RCU_BDCTL_LXTALSTB            = GD_RCU_BIT(1),
    RCU_BDCTL_LXTALBPS            = GD_RCU_BIT(2),
    RCU_BDCTL_LXTALDRI            = GD_RCU_BIT(3),
    RCU_BDCTL_RTCSRC              = GD_RCU_BITS(8,9),
    RCU_BDCTL_RTCEN               = GD_RCU_BIT(15),
    RCU_BDCTL_BKPRST              = GD_RCU_BIT(16),
};

/* RCU_RSTSCK */
enum GD32F470xx_RCU_RSTSCK_Field {
    RCU_RSTSCK_IRC32KEN           = GD_RCU_BIT(0),
    RCU_RSTSCK_IRC32KSTB          = GD_RCU_BIT(1),
    RCU_RSTSCK_RSTFC              = GD_RCU_BIT(24),
    RCU_RSTSCK_BORRSTF            = GD_RCU_BIT(25),
    RCU_RSTSCK_EPRSTF             = GD_RCU_BIT(26),
    RCU_RSTSCK_PORRSTF            = GD_RCU_BIT(27),
    RCU_RSTSCK_SWRSTF             = GD_RCU_BIT(28),
    RCU_RSTSCK_FWDGTRSTF          = GD_RCU_BIT(29),
    RCU_RSTSCK_WWDGTRSTF          = GD_RCU_BIT(30),
    RCU_RSTSCK_LPRSTF             = GD_RCU_BIT(31),
};

/* RCU_PLLSSCTL */
enum GD32F470xx_RCU_PLLSSCTL_Field {
    RCU_PLLSSCTL_MODCNT           = GD_RCU_BITS(0,12),
    RCU_PLLSSCTL_MODSTEP          = GD_RCU_BITS(13,27),
    RCU_PLLSSCTL_SS_TYPE          = GD_RCU_BIT(30),
    RCU_PLLSSCTL_SSCGON           = GD_RCU_BIT(31),
};

/* RCU_PLLI2S */
enum GD32F470xx_RCU_PLLI2S_Field {
    RCU_PLLI2S_PLLI2SN            = GD_RCU_BITS(6,14),
    RCU_PLLI2S_PLLI2SR            = GD_RCU_BITS(28,30),
};

/* RCU_PLLSAI */
enum GD32F470xx_RCU_PLLSAI_Field {
    RCU_PLLSAI_PLLSAIN            = GD_RCU_BITS(6,14),
    RCU_PLLSAI_PLLSAIP            = GD_RCU_BITS(16,17),
    RCU_PLLSAI_PLLSAIR            = GD_RCU_BITS(28,30),
};

/* RCU_CFG1 */
enum GD32F470xx_RCU_CFG1_Field {
    RCU_CFG1_PLLSAIRDIV           = GD_RCU_BITS(16,17),
    RCU_CFG1_TIMERSEL             = GD_RCU_BIT(24),
};

/* RCU_ADDCTL */
enum GD32F470xx_RCU_ADDCTL_Field {
    RCU_ADDCTL_CK48MSEL           = GD_RCU_BIT(0),
    RCU_ADDCTL_PLL48MSEL          = GD_RCU_BIT(1),
    RCU_ADDCTL_IRC48MEN           = GD_RCU_BIT(16),
    RCU_ADDCTL_IRC48MSTB          = GD_RCU_BIT(17),
    RCU_ADDCTL_IRC48MCAL          = GD_RCU_BITS(24,31),
};

/* RCU_ADDINT */
enum GD32F470xx_RCU_ADDINT_Field {
    RCU_ADDINT_IRC48MSTBIF        = GD_RCU_BIT(6),
    RCU_ADDINT_IRC48MSTBIE        = GD_RCU_BIT(14),
    RCU_ADDINT_IRC48MSTBIC        = GD_RCU_BIT(22),
};

/* RCU_ADDAPB1RST */
enum GD32F470xx_RCU_ADDAPB1RST_Field {
    RCU_ADDAPB1RST_CTCRST         = GD_RCU_BIT(27),
    RCU_ADDAPB1RST_IREFRST        = GD_RCU_BIT(31),
};

/* RCU_ADDAPB1EN */
enum GD32F470xx_RCU_ADDAPB1EN_Field {
    RCU_ADDAPB1EN_CTCEN           = GD_RCU_BIT(27),
    RCU_ADDAPB1EN_IREFEN          = GD_RCU_BIT(31),
};

/* RCU_ADDAPB1SPEN */
enum GD32F470xx_RCU_ADDAPB1SPEN_Field {
    RCU_ADDAPB1SPEN_CTCSPEN       = GD_RCU_BIT(27),
    RCU_ADDAPB1SPEN_IREFSPEN      = GD_RCU_BIT(31),
};

/* RCU_VKEY */
enum GD32F470xx_RCU_VKEY_Field {
    RCU_VKEY_KEY                  = GD_RCU_BITS(0,31),
};

/* RCU_DSV */
enum GD32F470xx_RCU_DSV_Field {
    RCU_DSV_DSLPVS                = GD_RCU_BITS(0,2),
};

struct GD32F470XXRcuState {
    /* <private> */
    SysBusDevice parent_obj;

    /* <public> */
    MemoryRegion mmio;

    uint32_t rcu_reg[32];

    Clock * refclk;
};

#endif
