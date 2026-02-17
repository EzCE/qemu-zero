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

#ifndef GD32F470XX_GPIO_H
#define GD32F470XX_GPIO_H

#include "system/memory.h"
#include "hw/core/sysbus.h"

/* Number of pins managed by each controller. */
#define GD32F470XX_GPIO_NR_PINS (16)

typedef struct GD32F470xxGpioState {
    SysBusDevice parent;

    uint32_t ctl;
    uint16_t omode;
    uint32_t ospd;
    uint32_t pud;
    uint16_t istat;
    uint16_t octl;
    uint32_t afsel0;
    uint32_t afsel1;

    uint32_t reset_ctl;
    uint32_t reset_ospd;
    uint32_t reset_pud;

    MemoryRegion mmio;
    qemu_irq output[GD32F470XX_GPIO_NR_PINS];
} GD32F470xxGpioState;

#define TYPE_GD32F470XX_GPIO "gd32f470xx-gpio"
#define GD32F470XX_GPIO(obj) \
    OBJECT_CHECK(GD32F470xxGpioState, (obj), TYPE_GD32F470XX_GPIO)

#endif
