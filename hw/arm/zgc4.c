/*
 * ZGC4 calculator
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

#include "qemu/osdep.h"
#include "qapi/error.h"
#include "qapi/qapi-types-ui.h"
#include "hw/core/boards.h"
#include "hw/core/qdev-properties.h"
#include "hw/core/qdev-clock.h"
#include "qemu/error-report.h"
#include "hw/arm/gd32f470xx_soc.h"
#include "hw/arm/boot.h"
#include "hw/input/gpio-keypad.h"
#include "hw/display/st7789v.h"
#include "hw/arm/zgc4.h"
#include "system/block-backend.h"
#include "system/address-spaces.h"
#include "hw/arm/machines-qom.h"

/* Main SYSCLK frequency in Hz (240MHz) */
#define SYSCLK_FRQ 240000000ULL

#define ST7789V_ADD 0x60000000

static const GpioKeypadKey keys[] = {
    { 0, 0, Q_KEY_CODE_F1 }, // Key::y=
    { 1, 0, Q_KEY_CODE_F2 }, // Key::window
    { 2, 0, Q_KEY_CODE_F3 }, // Key::zoom
    { 3, 0, Q_KEY_CODE_F4 }, // Key::trace
    { 4, 0, Q_KEY_CODE_F5 }, // Key::graph

    { 0, 1, Q_KEY_CODE_ESC }, // Key::mode
    { 1, 1, Q_KEY_CODE_LEFT }, // Key::left
    { 2, 1, Q_KEY_CODE_UP }, // Key::up
    { 3, 1, Q_KEY_CODE_RIGHT }, // Key::right

    { 0, 2, Q_KEY_CODE_EQUAL }, // Key::x,t,theta,n
    { 1, 2, Q_KEY_CODE_END }, // Key::stat
    { 2, 2, Q_KEY_CODE_DOWN }, // Key::down
    { 3, 2, Q_KEY_CODE_DELETE }, // Key::del
    { 4, 2, Q_KEY_CODE_SLASH }, // Key::clear

    { 0, 3, Q_KEY_CODE_A }, // Key::math
    { 1, 3, Q_KEY_CODE_B }, // Key::apps
    { 2, 3, Q_KEY_CODE_C }, // Key::prgm
    { 3, 3, Q_KEY_CODE_INSERT }, // Key::vars
    { 4, 3, Q_KEY_CODE_CTRL }, // Key::alpha

    { 0, 4, Q_KEY_CODE_D }, // Key::x^-1
    { 1, 4, Q_KEY_CODE_E }, // Key::sin
    { 2, 4, Q_KEY_CODE_F }, // Key::cos
    { 3, 4, Q_KEY_CODE_G }, // Key::tan
    { 4, 4, Q_KEY_CODE_H }, // Key::^

    { 0, 5, Q_KEY_CODE_I }, // Key::x^2
    { 1, 5, Q_KEY_CODE_J }, // Key::,
    { 2, 5, Q_KEY_CODE_K }, // Key::(
    { 3, 5, Q_KEY_CODE_L }, // Key::)
    { 4, 5, Q_KEY_CODE_M }, { 4, 5, Q_KEY_CODE_KP_DIVIDE }, // Key::/

    { 0, 6, Q_KEY_CODE_N }, // Key::log
    { 1, 6, Q_KEY_CODE_O }, { 1, 6, Q_KEY_CODE_KP_7 }, // Key::7
    { 2, 6, Q_KEY_CODE_P }, { 2, 6, Q_KEY_CODE_KP_8 }, // Key::8
    { 3, 6, Q_KEY_CODE_Q }, { 3, 6, Q_KEY_CODE_KP_9 }, // Key::9
    { 4, 6, Q_KEY_CODE_R }, { 4, 6, Q_KEY_CODE_KP_MULTIPLY }, // Key::*

    { 0, 7, Q_KEY_CODE_S }, // Key::ln
    { 1, 7, Q_KEY_CODE_T }, { 1, 7, Q_KEY_CODE_KP_4 }, // Key::4
    { 2, 7, Q_KEY_CODE_U }, { 2, 7, Q_KEY_CODE_KP_5 }, // Key::5
    { 3, 7, Q_KEY_CODE_V }, { 3, 7, Q_KEY_CODE_KP_6 }, // Key::6
    { 4, 7, Q_KEY_CODE_W }, { 4, 7, Q_KEY_CODE_KP_SUBTRACT }, // Key::-

    { 0, 8, Q_KEY_CODE_X }, // Key::sto->
    { 1, 8, Q_KEY_CODE_Y }, { 1, 8, Q_KEY_CODE_1 }, // Key::1
    { 2, 8, Q_KEY_CODE_Z }, { 2, 8, Q_KEY_CODE_2 }, // Key::2
    { 3, 8, Q_KEY_CODE_3 }, // Key::3
    { 4, 8, Q_KEY_CODE_KP_ADD }, // Key::+

    { 1, 8, Q_KEY_CODE_SPC }, { 1, 8, Q_KEY_CODE_0 }, // Key::0
    { 2, 8, Q_KEY_CODE_KP_DECIMAL }, // Key::.
    { 3, 8, Q_KEY_CODE_ALT }, // Key::(-)
    { 4, 8, Q_KEY_CODE_KP_ENTER }, // Key::enter

    { 0, 0, Q_KEY_CODE_UNMAPPED },
};

static void zgc4_init(MachineState *machine)
{
    DeviceState *soc;
    DeviceState *flash;
    DeviceState *gpio;
    DeviceState *dev;
    DriveInfo *dinfo;
    Clock *sysclk;

    /* This clock doesn't need migration because it is fixed-frequency */
    sysclk = clock_new(OBJECT(machine), "SYSCLK");
    clock_set_hz(sysclk, SYSCLK_FRQ);

    soc = qdev_new(TYPE_GD32F470XX_SOC);
    qdev_prop_set_string(soc, "soc-type", VARIANT_GD32F470Z_SOC);
    qdev_connect_clock_in(soc, "sysclk", sysclk);
    sysbus_realize(SYS_BUS_DEVICE(soc), &error_fatal);

    dinfo = drive_get(IF_MTD, 0, 0);
    flash = qdev_new("w25q64");
    if (dinfo) {
        qdev_prop_set_drive(flash, "drive",
                            blk_by_legacy_dinfo(dinfo));
    }
    GD32F470XXState *s = GD32F470XX_SOC(soc);
    qdev_realize(flash, BUS(s->spi[5].ssi), &error_fatal);
    qemu_irq cs_line = qdev_get_gpio_in_named(flash, SSI_GPIO_CS, 0);
    qdev_connect_gpio_out(DEVICE(&s->gpio[1]),
                          5,
                          cs_line);
    object_unref(OBJECT(flash));

    dev = qdev_new(TYPE_ST7789V);
    qdev_prop_set_bit(dev, "rotate-right", true);
    sysbus_mmio_map(SYS_BUS_DEVICE(dev), 0, ST7789V_ADD);
    sysbus_realize_and_unref(SYS_BUS_DEVICE(dev), &error_fatal);

    gpio = qdev_new(TYPE_GPIO_KEYPAD);
    qdev_prop_set_bit(gpio, "active-low", true);
    qdev_prop_set_uint32(gpio, "num-columns", 5);
    qdev_prop_set_uint32(gpio, "num-rows", 10);
    gpio_keypad_set_keys(gpio, keys);
    sysbus_realize(SYS_BUS_DEVICE(gpio), &error_fatal);
    qdev_connect_gpio_out(DEVICE(gpio), 0,
                          qdev_get_gpio_in_named(soc, "gpio-a", 4));

    qdev_connect_gpio_out(DEVICE(gpio), 1,
                          qdev_get_gpio_in_named(soc, "gpio-a", 5));

    qdev_connect_gpio_out(DEVICE(gpio), 2,
                          qdev_get_gpio_in_named(soc, "gpio-d", 2));

    qdev_connect_gpio_out(DEVICE(gpio), 3,
                          qdev_get_gpio_in_named(soc, "gpio-d", 3));

    qdev_connect_gpio_out(DEVICE(gpio), 4,
                          qdev_get_gpio_in_named(soc, "gpio-b", 14));

    qdev_connect_gpio_out(DEVICE(gpio), 5,
                          qdev_get_gpio_in_named(soc, "gpio-b", 15));

    qdev_connect_gpio_out(DEVICE(gpio), 6,
                          qdev_get_gpio_in_named(soc, "gpio-d", 6));

    qdev_connect_gpio_out(DEVICE(gpio), 7,
                          qdev_get_gpio_in_named(soc, "gpio-c", 7));

    qdev_connect_gpio_out(DEVICE(gpio), 8,
                          qdev_get_gpio_in_named(soc, "gpio-c", 8));

    qdev_connect_gpio_out(DEVICE(gpio), 9,
                          qdev_get_gpio_in_named(soc, "gpio-c", 9));
    for (int i = 0; i < 5; i++) {
        qdev_connect_gpio_out_named(DEVICE(soc), "gpio-b-out", i,
                                    qdev_get_gpio_in(gpio, i));
    }
    object_unref(OBJECT(gpio));

    object_unref(OBJECT(soc));

    armv7m_load_kernel(ARM_CPU(first_cpu),
                       machine->kernel_filename,
                       GD32F470XX_FLASH_BASE_ADDRESS,
                       GD32F470Z_SOC_FLASH_SIZE);
}

static void zgc4_machine_init(MachineClass *mc)
{
    mc->desc = "ZGC4 calculator (Cortex-M4)";
    mc->init = zgc4_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("cortex-m4");
}

DEFINE_MACHINE_ARM("zgc4", zgc4_machine_init)
