/*
 * GD32F470xx GPIO
 *
 * Copyright 2022 Jean-Baptiste Boric <jblbeurope@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include "qemu/osdep.h"

#include "hw/gpio/gd32f470xx_gpio.h"
#include "hw/core/irq.h"
#include "hw/core/qdev-properties.h"
#include "qapi/error.h"
#include "qemu/log.h"
#include "qemu/module.h"
#include "qemu/units.h"
#include "trace.h"

#define GD32F470XX_GPIO_REGS_SIZE (1 * KiB)

/* 32-bit register indices. */
enum GD32F470xxGPIORegister {
    GD32F470XX_GPIO_CTL,
    GD32F470XX_GPIO_OMODE,
    GD32F470XX_GPIO_OSPD,
    GD32F470XX_GPIO_PUD,
    GD32F470XX_GPIO_ISTAT,
    GD32F470XX_GPIO_OCTL,
    GD32F470XX_GPIO_BOP,
    GD32F470XX_GPIO_LOCK,
    GD32F470XX_GPIO_AFSEL0,
    GD32F470XX_GPIO_AFSEL1,
    GD32F470XX_GPIO_BC,
    GD32F470XX_GPIO_REGS_END,
};

static void gd32f470xx_gpio_update_pins(GD32F470xxGpioState *s, uint16_t diff)
{
    int i;

    for (i = 0; i < GD32F470XX_GPIO_NR_PINS; i++) {
        if (diff & (1u << i)) {
            bool value = s->octl & (1u << i);
            trace_gd32f470xx_gpio_update_pins(DEVICE(s)->canonical_path, i, value);
            qemu_set_irq(s->output[i], value);
        }
    }
}

static uint64_t gd32f470xx_gpio_regs_read(void *opaque, hwaddr addr,
                                       unsigned int size)
{
    hwaddr reg = addr / sizeof(uint32_t);
    GD32F470xxGpioState *s = opaque;
    uint64_t value = 0;

    switch (reg) {
    case GD32F470XX_GPIO_CTL:
        value = s->ctl;
        break;
    case GD32F470XX_GPIO_OMODE:
        value = s->ctl;
        break;
    case GD32F470XX_GPIO_OSPD:
        value = s->ospd;
        break;
    case GD32F470XX_GPIO_PUD:
        value = s->pud;
        break;
    case GD32F470XX_GPIO_ISTAT:
        value = s->istat;
        break;
    case GD32F470XX_GPIO_OCTL:
        value = s->octl;
        break;
    case GD32F470XX_GPIO_AFSEL0:
        value = s->afsel0;
        break;
    case GD32F470XX_GPIO_AFSEL1:
        value = s->afsel1;
        break;

    case GD32F470XX_GPIO_BOP:
        qemu_log_mask(LOG_GUEST_ERROR,
                      "%s: read from write-only register 0x%" HWADDR_PRIx "\n",
                      DEVICE(s)->canonical_path, addr);
        break;

    default:
        qemu_log_mask(LOG_GUEST_ERROR,
                      "%s: read from invalid offset 0x%" HWADDR_PRIx "\n",
                      DEVICE(s)->canonical_path, addr);
        break;
    }

    trace_gd32f470xx_gpio_read(DEVICE(s)->canonical_path, addr, value);

    return value;
}

static void gd32f470xx_gpio_regs_write(void *opaque, hwaddr addr, uint64_t v,
                                    unsigned int size)
{
    hwaddr reg = addr / sizeof(uint32_t);
    GD32F470xxGpioState *s = opaque;
    uint32_t value = v;
    uint16_t diff;

    trace_gd32f470xx_gpio_write(DEVICE(s)->canonical_path, addr, v);

    switch (reg) {
    case GD32F470XX_GPIO_OCTL:
        diff = s->octl ^ value;
        s->octl = value;
        gd32f470xx_gpio_update_pins(s, diff);
        break;
    case GD32F470XX_GPIO_BOP:
        s->octl |= value;
        gd32f470xx_gpio_update_pins(s, value);
        break;
    case GD32F470XX_GPIO_BC:
        s->octl &= ~value;
        gd32f470xx_gpio_update_pins(s, value);
        break;
    case GD32F470XX_GPIO_ISTAT:
        qemu_log_mask(LOG_GUEST_ERROR,
                      "%s: write to read-only register @ 0x%" HWADDR_PRIx "\n",
                      DEVICE(s)->canonical_path, addr);
        break;
    default:
        qemu_log_mask(LOG_GUEST_ERROR,
                      "%s: write to invalid offset 0x%" HWADDR_PRIx "\n",
                      DEVICE(s)->canonical_path, addr);
        break;
    }
}

static const MemoryRegionOps gd32f470xx_gpio_regs_ops = {
    .read = gd32f470xx_gpio_regs_read,
    .write = gd32f470xx_gpio_regs_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
    .valid = {
        .min_access_size = 2,
        .max_access_size = 4,
        .unaligned = false,
    },
};

static void gd32f470xx_gpio_set_input(void *opaque, int n, int level)
{
    GD32F470xxGpioState *s = opaque;

    s->istat &= ~(1u << n);
    if (level) {
        s->istat |= 1u << n;
    }
}

static void gd32f470xx_gpio_enter_reset(Object *obj, ResetType type)
{
    GD32F470xxGpioState *s = GD32F470XX_GPIO(obj);

    s->ctl = s->reset_ctl;
    s->omode = 0x0000;
    s->ospd = s->reset_ospd;
    s->pud = s->reset_pud;
    s->istat = 0x0000;
    s->octl = 0x0000;
    s->afsel0 = 0x00000000;
    s->afsel1 = 0x00000000;
}

static void gd32f470xx_gpio_hold_reset(Object *obj, ResetType type)
{
    GD32F470xxGpioState *s = GD32F470XX_GPIO(obj);

    gd32f470xx_gpio_update_pins(s, 0xFFFF);
}

static void gd32f470xx_gpio_init(Object *obj)
{
    GD32F470xxGpioState *s = GD32F470XX_GPIO(obj);
    DeviceState *dev = DEVICE(obj);

    memory_region_init_io(&s->mmio, obj, &gd32f470xx_gpio_regs_ops, s,
                          "regs", GD32F470XX_GPIO_REGS_SIZE);
    sysbus_init_mmio(SYS_BUS_DEVICE(obj), &s->mmio);

    qdev_init_gpio_in(dev, gd32f470xx_gpio_set_input, GD32F470XX_GPIO_NR_PINS);
    qdev_init_gpio_out(dev, s->output, GD32F470XX_GPIO_NR_PINS);
}

static const Property gd32f470xx_gpio_properties[] = {
    DEFINE_PROP_UINT32("reset-ctl", GD32F470xxGpioState, reset_ctl, 0),
    DEFINE_PROP_UINT32("reset-ospd", GD32F470xxGpioState, reset_ospd, 0),
    DEFINE_PROP_UINT32("reset-pud", GD32F470xxGpioState, reset_pud, 0),
};

static void gd32f470xx_gpio_class_init(ObjectClass *klass, const void *data)
{
    ResettableClass *reset = RESETTABLE_CLASS(klass);
    DeviceClass *dc = DEVICE_CLASS(klass);

    dc->desc = "GD32F470xx GPIO Controller";
    reset->phases.enter = gd32f470xx_gpio_enter_reset;
    reset->phases.hold = gd32f470xx_gpio_hold_reset;
    device_class_set_props(dc, gd32f470xx_gpio_properties);
}

static const TypeInfo gd32f470xx_gpio_types[] = {
    {
        .name = TYPE_GD32F470XX_GPIO,
        .parent = TYPE_SYS_BUS_DEVICE,
        .instance_size = sizeof(GD32F470xxGpioState),
        .class_init = gd32f470xx_gpio_class_init,
        .instance_init = gd32f470xx_gpio_init,
    },
};

DEFINE_TYPES(gd32f470xx_gpio_types);
