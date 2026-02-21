/*
 * GD32F470XX RCU
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

#include "qemu/osdep.h"
#include "hw/misc/gd32f470xx_rcu.h"
#include "qemu/log.h"
#include "qemu/module.h"
#include "hw/core/qdev-clock.h"
#include "trace.h"

enum GD32F470xx_RCU_Reg_Add {
    RCU_CTL_ADD         = 0x00,         /*!< control register */
    RCU_PLL_ADD         = 0x04,         /*!< PLL register */
    RCU_CFG0_ADD        = 0x08,         /*!< clock configuration register 0 */
    RCU_INT_ADD         = 0x0C,         /*!< clock interrupt register */
    RCU_AHB1RST_ADD     = 0x10,         /*!< AHB1 reset register */
    RCU_AHB2RST_ADD     = 0x14,         /*!< AHB2 reset register */
    RCU_AHB3RST_ADD     = 0x18,         /*!< AHB3 reset register */
    RCU_APB1RST_ADD     = 0x20,         /*!< APB1 reset register */
    RCU_APB2RST_ADD     = 0x24,         /*!< APB2 reset register */
    RCU_AHB1EN_ADD      = 0x30,         /*!< AHB1 enable register */
    RCU_AHB2EN_ADD      = 0x34,         /*!< AHB2 enable register */
    RCU_AHB3EN_ADD      = 0x38,         /*!< AHB3 enable register */
    RCU_APB1EN_ADD      = 0x40,         /*!< APB1 enable register */
    RCU_APB2EN_ADD      = 0x44,         /*!< APB2 enable register */
    RCU_AHB1SPEN_ADD    = 0x50,         /*!< AHB1 sleep mode enable register */
    RCU_AHB2SPEN_ADD    = 0x54,         /*!< AHB2 sleep mode enable register */
    RCU_AHB3SPEN_ADD    = 0x58,         /*!< AHB3 sleep mode enable register */ 
    RCU_APB1SPEN_ADD    = 0x60,         /*!< APB1 sleep mode enable register */
    RCU_APB2SPEN_ADD    = 0x64,         /*!< APB2 sleep mode enable register */
    RCU_BDCTL_ADD       = 0x70,         /*!< backup domain control register */
    RCU_RSTSCK_ADD      = 0x74,         /*!< reset source / clock register */
    RCU_PLLSSCTL_ADD    = 0x80,         /*!< PLL clock spread spectrum control register */
    RCU_PLLI2S_ADD      = 0x84,         /*!< PLLI2S register */ 
    RCU_PLLSAI_ADD      = 0x88,         /*!< PLLSAI register */ 
    RCU_CFG1_ADD        = 0x8C,         /*!< clock configuration register 1 */
    RCU_ADDCTL_ADD      = 0xC0,         /*!< Additional clock control register */
    RCU_ADDINT_ADD      = 0xCC,         /*!< Additional clock interrupt register */
    RCU_ADDAPB1RST_ADD  = 0xE0,         /*!< APB1 additional reset register */
    RCU_ADDAPB1EN_ADD   = 0xE4,         /*!< APB1 additional enable register */
    RCU_ADDAPB1SPEN_ADD = 0xE8,         /*!< APB1 additional sleep mode enable register */
    RCU_VKEY_ADD        = 0x100,        /*!< voltage key register */
    RCU_DSV_ADD         = 0x134,        /*!< deep-sleep mode voltage register */
};

enum GD32F470xx_RCU_Reg {
    RCU_CTL,            /*!< control register */
    RCU_PLL,            /*!< PLL register */
    RCU_CFG0,           /*!< clock configuration register 0 */
    RCU_INT,            /*!< clock interrupt register */
    RCU_AHB1RST,        /*!< AHB1 reset register */
    RCU_AHB2RST,        /*!< AHB2 reset register */
    RCU_AHB3RST,        /*!< AHB3 reset register */
    RCU_APB1RST,        /*!< APB1 reset register */
    RCU_APB2RST,        /*!< APB2 reset register */
    RCU_AHB1EN,         /*!< AHB1 enable register */
    RCU_AHB2EN,         /*!< AHB2 enable register */
    RCU_AHB3EN,         /*!< AHB3 enable register */
    RCU_APB1EN,         /*!< APB1 enable register */
    RCU_APB2EN,         /*!< APB2 enable register */
    RCU_AHB1SPEN,       /*!< AHB1 sleep mode enable register */
    RCU_AHB2SPEN,       /*!< AHB2 sleep mode enable register */
    RCU_AHB3SPEN,       /*!< AHB3 sleep mode enable register */ 
    RCU_APB1SPEN,       /*!< APB1 sleep mode enable register */
    RCU_APB2SPEN,       /*!< APB2 sleep mode enable register */
    RCU_BDCTL,          /*!< backup domain control register */
    RCU_RSTSCK,         /*!< reset source / clock register */
    RCU_PLLSSCTL,       /*!< PLL clock spread spectrum control register */
    RCU_PLLI2S,         /*!< PLLI2S register */ 
    RCU_PLLSAI,         /*!< PLLSAI register */ 
    RCU_CFG1,           /*!< clock configuration register 1 */
    RCU_ADDCTL,         /*!< Additional clock control register */
    RCU_ADDINT,         /*!< Additional clock interrupt register */
    RCU_ADDAPB1RST,     /*!< APB1 additional reset register */
    RCU_ADDAPB1EN,      /*!< APB1 additional enable register */
    RCU_ADDAPB1SPEN,    /*!< APB1 additional sleep mode enable register */
    RCU_VKEY,           /*!< voltage key register */
    RCU_DSV,            /*!< deep-sleep mode voltage register */
};

static void gd32f470xx_rcu_reset(DeviceState *dev)
{
    GD32F470XXRcuState *s = GD32F470XX_RCU(dev);

    // s->rcu_reg[RCU_CTL] = 0x00000083;
    // s->rcu_reg[RCU_CFG0] = 0x00000000;
    memset(s->rcu_reg, 0, sizeof(s->rcu_reg));
}

static uint64_t gd32f470xx_rcu_read(void *opaque, hwaddr addr,
                                     unsigned int size)
{
    GD32F470XXRcuState *s = opaque;
    uint64_t value = 0;

    switch (addr) {
    case RCU_CTL_ADD:
        value = s->rcu_reg[RCU_CTL];
        break;
    case RCU_PLL_ADD:
        value = s->rcu_reg[RCU_PLL];
        break;
    case RCU_CFG0_ADD:
        value = s->rcu_reg[RCU_CFG0];
        break;
    case RCU_INT_ADD:
        value = s->rcu_reg[RCU_INT];
        break;
    case RCU_AHB1RST_ADD:
        value = s->rcu_reg[RCU_AHB1RST];
        break;
    case RCU_AHB2RST_ADD:
        value = s->rcu_reg[RCU_AHB2RST];
        break;
    case RCU_AHB3RST_ADD:
        value = s->rcu_reg[RCU_AHB3RST];
        break;
    case RCU_APB1RST_ADD:
        value = s->rcu_reg[RCU_APB1RST];
        break;
    case RCU_APB2RST_ADD:
        value = s->rcu_reg[RCU_APB2RST];
        break;
    case RCU_AHB1EN_ADD:
        value = s->rcu_reg[RCU_AHB1EN];
        break;
    case RCU_AHB2EN_ADD:
        value = s->rcu_reg[RCU_AHB2EN];
        break;
    case RCU_AHB3EN_ADD:
        value = s->rcu_reg[RCU_AHB3EN];
        break;
    case RCU_APB1EN_ADD:
        value = s->rcu_reg[RCU_APB1EN];
        break;
    case RCU_APB2EN_ADD:
        value = s->rcu_reg[RCU_APB2EN];
        break;
    case RCU_AHB1SPEN_ADD:
        value = s->rcu_reg[RCU_AHB1SPEN];
        break;
    case RCU_AHB2SPEN_ADD:
        value = s->rcu_reg[RCU_AHB2SPEN];
        break;
    case RCU_AHB3SPEN_ADD:
        value = s->rcu_reg[RCU_AHB3SPEN];
        break;
    case RCU_APB1SPEN_ADD:
        value = s->rcu_reg[RCU_APB1SPEN];
        break;
    case RCU_APB2SPEN_ADD:
        value = s->rcu_reg[RCU_APB2SPEN];
        break;
    case RCU_BDCTL_ADD:
        value = s->rcu_reg[RCU_BDCTL];
        break;
    case RCU_RSTSCK_ADD:
        value = s->rcu_reg[RCU_RSTSCK];
        break;
    case RCU_PLLSSCTL_ADD:
        value = s->rcu_reg[RCU_PLLSSCTL];
        break;
    case RCU_PLLI2S_ADD:
        value = s->rcu_reg[RCU_PLLI2S];
        break;
    case RCU_PLLSAI_ADD:
        value = s->rcu_reg[RCU_PLLSAI];
        break;
    case RCU_CFG1_ADD:
        value = s->rcu_reg[RCU_CFG1];
        break;
    case RCU_ADDCTL_ADD:
        value = s->rcu_reg[RCU_ADDCTL];
        break;
    case RCU_ADDINT_ADD:
        value = s->rcu_reg[RCU_ADDINT];
        break;
    case RCU_ADDAPB1RST_ADD:
        value = s->rcu_reg[RCU_ADDAPB1RST];
        break;
    case RCU_ADDAPB1EN_ADD:
        value = s->rcu_reg[RCU_ADDAPB1EN];
        break;
    case RCU_ADDAPB1SPEN_ADD:
        value = s->rcu_reg[RCU_ADDAPB1SPEN];
        break;
    case RCU_VKEY_ADD:
        value = s->rcu_reg[RCU_VKEY];
        break;
    case RCU_DSV_ADD:
        value = s->rcu_reg[RCU_DSV];
        break;
    default:
        qemu_log_mask(LOG_UNIMP,
                      "%s: Unimplemented RCU read 0x%"HWADDR_PRIx"\n", __func__,
                      addr);
        break;
    }

    trace_gd32f470xx_rcu_read(s, addr, size, value);
    return value;
}

static void gd32f470xx_rcu_write(void *opaque, hwaddr addr,
                       uint64_t val64, unsigned int size)
{
    const uint32_t clocks_on_mask = 0x15010001;
    GD32F470XXRcuState *s = opaque;
    uint32_t value = val64;

    trace_gd32f470xx_rcu_write(s, addr, size, val64);

    switch (addr) {
    case RCU_CTL_ADD:
        /* Crudely simulate clock readiness. */
        value = (value & ~(clocks_on_mask << 1));
        value |= (value & clocks_on_mask) << 1;
        s->rcu_reg[RCU_CTL] = value;
        break;
    case RCU_CFG0_ADD:
        uint8_t AHBPrescalar = (value & 240) >> 4;

        value &= ~0xC;
        value |= (value & 0x3) << 2;
        s->rcu_reg[RCU_CFG0] = value;

        if (AHBPrescalar == 0) {
            clock_set_mul_div(s->refclk, 8, 1);
        } else if (AHBPrescalar == 9) {
            clock_set_mul_div(s->refclk, 32, 1);
        } else {
            qemu_log_mask(LOG_UNIMP, "%s : Unimplemented AHBPrescalar\n", __func__);
        }
        clock_propagate(s->refclk->source->source);
        break;
    case RCU_ADDCTL_ADD:
        s->rcu_reg[RCU_ADDCTL] = value;

        if (value & (1 << 16)) { // IRC48MEN
            s->rcu_reg[RCU_ADDCTL] |= (1 << 17); // IRC48MSTB
        }
        break;
    default:
        qemu_log_mask(LOG_UNIMP,
                      "%s: Unimplemented RCU write 0x%"HWADDR_PRIx"\n",
                      __func__, addr);
    }
}

static const MemoryRegionOps gd32f470xx_rcu_ops = {
    .read = gd32f470xx_rcu_read,
    .write = gd32f470xx_rcu_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

static void gd32f470xx_rcu_init(Object *obj)
{
    GD32F470XXRcuState *s = GD32F470XX_RCU(obj);

    memory_region_init_io(&s->mmio, obj, &gd32f470xx_rcu_ops, s,
                          TYPE_GD32F470XX_RCU, 0x400);
    sysbus_init_mmio(SYS_BUS_DEVICE(obj), &s->mmio);
}

static void gd32f470xx_rcu_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    device_class_set_legacy_reset(dc, gd32f470xx_rcu_reset);
}

static const TypeInfo gd32f470xx_rcu_info = {
    .name          = TYPE_GD32F470XX_RCU,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(GD32F470XXRcuState),
    .instance_init = gd32f470xx_rcu_init,
    .class_init    = gd32f470xx_rcu_class_init,
};

static void gd32f470xx_rcu_register_types(void)
{
    type_register_static(&gd32f470xx_rcu_info);
}

type_init(gd32f470xx_rcu_register_types)
