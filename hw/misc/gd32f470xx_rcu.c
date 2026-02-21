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

static void gd32f470xx_rcu_reset(DeviceState *dev)
{
    GD32F470XXRcuState *s = GD32F470XX_RCU(dev);

    /* Set initial register values from GD32F4xx user manual */
    memset(s->rcu_reg, 0, sizeof(s->rcu_reg));
    s->rcu_reg[RCU_CTL] = 0x00000083;
    s->rcu_reg[RCU_PLL] = 0x24003010;
    s->rcu_reg[RCU_AHB1EN] = 0x00100000;
    s->rcu_reg[RCU_AHB1SPEN] = 0x7EEF91FF;
    s->rcu_reg[RCU_AHB2SPEN] = 0x000000C1;
    s->rcu_reg[RCU_AHB3SPEN] = 0x00000001;
    s->rcu_reg[RCU_APB1SPEN] = 0xF6FEC9FF;
    s->rcu_reg[RCU_APB2SPEN] = 0x04777F33;
    s->rcu_reg[RCU_RSTSCK] = 0x0E000000;
    s->rcu_reg[RCU_PLLI2S] = 0x24003000;
    s->rcu_reg[RCU_PLLSAI] = 0x24003000;
    s->rcu_reg[RCU_ADDAPB1SPEN] = 0x88000000;
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
    case RCU_BDCTL_ADD:
        s->rcu_reg[RCU_BDCTL] = value;

        if (GD_RCU_FIELD(value, RCU_BDCTL_LXTALEN)) {
            s->rcu_reg[RCU_BDCTL] |= RCU_BDCTL_LXTALSTB;
        }
        break;
    case RCU_ADDCTL_ADD:
        s->rcu_reg[RCU_ADDCTL] = value;

        if (GD_RCU_FIELD(value, RCU_ADDCTL_IRC48MEN)) {
            s->rcu_reg[RCU_ADDCTL] |= RCU_ADDCTL_IRC48MSTB;
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
