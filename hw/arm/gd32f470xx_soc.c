/*
 * GD32F470xx SoC
 *
 * Copyright (c) 2026 TIny_Hacker
 * Copyright (c) 2014 Alistair Francis <alistair@alistair23.me>
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
#include "system/address-spaces.h"
#include "system/system.h"
#include "hw/arm/gd32f470xx_soc.h"
#include "hw/core/qdev-clock.h"
#include "hw/misc/unimp.h"

#define RCU_ADD                        0x40023800
#define CRC_ADD                        0x40023000
#define RNG_ADD                        0x50060800
#define SYSCFG_ADD                     0x40013800
#define USB_OTG_FS_ADD                 0x50000000

static const char *gpio_pass[] = {
    "gpio-a",
    "gpio-b",
    "gpio-c",
    "gpio-d",
    "gpio-e",
    "gpio-f",
    "gpio-g",
    "gpio-h",
    "gpio-i",
};
static const uint32_t gpio_addr[] = { 0x40020000, 0x40020400, 0x40020800,
                                      0x40020C00, 0x40021000, 0x40021400,
                                      0x40021800, 0x40021C00, 0x40022000 };
static const uint32_t usart_addr[] = { 0x40011000, 0x40004400, 0x40004800,
                                       0x40004C00, 0x40005000, 0x40011400,
                                       0x40007800, 0x40007C00 };
static const uint32_t timer_addr[] = { 0x40010000, 0x40000000, 0x40000400,
                                       0x40000800, 0x40000C00, 0x40001000,
                                       0x40001400, 0x40010400, 0x40014000,
                                       0x40014400, 0x40014800, 0x40001800,
                                       0x40001C00, 0x40002000 };
static const uint32_t adc_addr[] = { 0x40012000, 0x40012100, 0x40012200,
                                     0x40012300, 0x40012400, 0x40012500 };
static const uint32_t spi_addr[] =   { 0x40013000, 0x40003800, 0x40003C00,
                                       0x40013400, 0x40015000, 0x40015400 };
static const uint32_t i2c_addr[] =   { 0x40005400, 0x40005800, 0x40005C00 };
#define EXTI_ADDR                      0x40013C00

#define SYSCFG_IRQ               71
static const int usart_irq[] = { 37, 38, 39, 52, 53, 71, 82, 83 };
static const int timer_irq[] = { -1, 28, 29, 30, 50, 54, 55, -1, 24, 25, 26,
                                 43, 44, 45 };
#define ADC_IRQ 18
static const int spi_irq[] =   { 35, 36, 51, 0, 0, 0 };
static const int i2c_irq[] =   { 31, 33, 72 };
static const int exti_irq[] =  { 6, 7, 8, 9, 10, 23, 23, 23, 23, 23, 40,
                                 40, 40, 40, 40, 40} ;

typedef struct GD32F470Family {
    const char *soc_type;
    int flash_size;
    int ram_size;
} GD32F470Family;

static const GD32F470Family gd32f470_family[] = {
    { VARIANT_GD32F470Z_SOC, GD32F470Z_SOC_FLASH_SIZE, GD32F470Z_SOC_RAM_SIZE },
};

static void gd32f470xx_soc_initfn(Object *obj)
{
    GD32F470XXState *s = GD32F470XX_SOC(obj);
    int i;

    object_initialize_child(obj, "armv7m", &s->armv7m, TYPE_ARMV7M);

    object_initialize_child(obj, "rcu", &s->rcu, TYPE_GD32F470XX_RCU);

    object_initialize_child(obj, "pwr", &s->pwr, TYPE_STM32F2XX_PWR);

    object_initialize_child(obj, "crc", &s->crc, TYPE_STM32F2XX_CRC);

    object_initialize_child(obj, "rng", &s->rng, TYPE_STM32F2XX_RNG);

    object_initialize_child(obj, "syscfg", &s->syscfg, TYPE_STM32F4XX_SYSCFG);

    for (i = 0; i < GD32F470XX_NUM_GPIOS; i++) {
        object_initialize_child(obj, "gpio[*]", &s->gpio[i],
                                TYPE_GD32F470XX_GPIO);
    }

    for (i = 0; i < GD32F470XX_NUM_USARTS; i++) {
        object_initialize_child(obj, "usart[*]", &s->usart[i],
                                TYPE_STM32F2XX_USART);
    }

    for (i = 0; i < GD32F470XX_NUM_TIMERS; i++) {
        object_initialize_child(obj, "timer[*]", &s->timer[i],
                                TYPE_STM32F2XX_TIMER);
    }

    for (i = 0; i < GD32F470XX_NUM_ADCS; i++) {
        object_initialize_child(obj, "adc[*]", &s->adc[i], TYPE_STM32F2XX_ADC);
    }

    for (i = 0; i < GD32F470XX_NUM_SPIS; i++) {
        object_initialize_child(obj, "spi[*]", &s->spi[i], TYPE_STM32F2XX_SPI);
    }

    for (i = 0; i < GD32F470XX_NUM_I2CS; i++) {
        object_initialize_child(obj, "i2c[*]", &s->i2c[i], TYPE_GD32F470XX_I2C);
    }

    object_initialize_child(obj, "exti", &s->exti, TYPE_STM32F4XX_EXTI);
    object_initialize_child(obj, "usb-otg-fs", &s->usb_otg_fs, TYPE_STM32F2XX_USB_OTG_FS);

    s->sysclk = qdev_init_clock_in(DEVICE(s), "sysclk", NULL, NULL, 0);
    s->refclk = qdev_init_clock_in(DEVICE(s), "refclk", NULL, NULL, 0);
}

static void gd32f470xx_soc_realize(DeviceState *dev_soc, Error **errp)
{
    GD32F470XXState *s = GD32F470XX_SOC(dev_soc);
    MemoryRegion *system_memory = get_system_memory();
    DeviceState *dev, *armv7m;
    SysBusDevice *busdev;
    Error *err = NULL;
    int i;
    const GD32F470Family *soc_variant = NULL;

    for (i = 0; i < sizeof(gd32f470_family) / sizeof(gd32f470_family[0]); i++) {
        if (!strcmp(s->soc_type, gd32f470_family[i].soc_type)) {
            soc_variant = &gd32f470_family[i];
            break;
        }
    }

    if (!soc_variant) {
        error_setg(errp, "unknown soc-type variant for gd32f470 family");
        return;
    }

    /*
     * We use s->refclk internally and only define it with qdev_init_clock_in()
     * so it is correctly parented and not leaked on an init/deinit; it is not
     * intended as an externally exposed clock.
     */
    if (clock_has_source(s->refclk)) {
        error_setg(errp, "refclk clock must not be wired up by the board code");
        return;
    }

    if (!clock_has_source(s->sysclk)) {
        error_setg(errp, "sysclk clock must be wired up by the board code");
        return;
    }

    /*
     * TODO: ideally we should model the SoC RCC and its ability to
     * change the sysclk frequency and define different sysclk sources.
     */

    /* The refclk always runs at frequency HCLK / 8 */
    clock_set_mul_div(s->refclk, 8, 1);
    clock_set_source(s->refclk, s->sysclk);

    memory_region_init_ram(&s->flash, OBJECT(dev_soc), "GD32F470XX.flash",
                           soc_variant->flash_size, &err);
    if (err != NULL) {
        error_propagate(errp, err);
        return;
    }
    memory_region_init_alias(&s->flash_alias, OBJECT(dev_soc),
                             "GD32F470XX.flash.alias", &s->flash, 0,
                             soc_variant->flash_size);

    memory_region_add_subregion(system_memory, GD32F470XX_FLASH_BASE_ADDRESS,
                                &s->flash);
    memory_region_add_subregion(system_memory, 0, &s->flash_alias);

    memory_region_init_ram(&s->sram, NULL, "GD32F470XX.sram",
                           soc_variant->ram_size, &err);
    if (err != NULL) {
        error_propagate(errp, err);
        return;
    }
    memory_region_add_subregion(system_memory, GD32F470XX_SRAM_BASE_ADDRESS,
                                &s->sram);

    memory_region_init_ram(&s->tcmsram, NULL, "GD32F470XX.tcmsram",
                           GD32F470XX_TCMSRAM_SIZE, &err);
    if (err != NULL) {
        error_propagate(errp, err);
        return;
    }
    memory_region_add_subregion(system_memory, GD32F470XX_TCMSRAM_BASE_ADDRESS,
                                &s->tcmsram);

    memory_region_init_ram(&s->exmc_sdram, NULL, "GD32F470XX.exmc_sdram",
                           GD32F470XX_EXMC_SDRAM_SIZE, &err);
    if (err != NULL) {
        error_propagate(errp, err);
        return;
    }
    memory_region_add_subregion(system_memory, GD32F470XX_EXMC_SDRAM_BASE,
                                &s->exmc_sdram);

    armv7m = DEVICE(&s->armv7m);
    qdev_prop_set_uint32(armv7m, "init-nsvtor", GD32F470XX_FLASH_BASE_ADDRESS);
    qdev_prop_set_uint32(armv7m, "num-irq", 96);
    qdev_prop_set_string(armv7m, "cpu-type", ARM_CPU_TYPE_NAME("cortex-m4"));
    qdev_prop_set_bit(armv7m, "enable-bitband", true);
    qdev_connect_clock_in(armv7m, "cpuclk", s->sysclk);
    qdev_connect_clock_in(armv7m, "refclk", s->refclk);
    object_property_set_link(OBJECT(&s->armv7m), "memory",
                             OBJECT(system_memory), &error_abort);
    if (!sysbus_realize(SYS_BUS_DEVICE(&s->armv7m), errp)) {
        return;
    }

    /* Reset and clock controller */
    dev = DEVICE(&s->rcu);
    if (!sysbus_realize(SYS_BUS_DEVICE(&s->rcu), errp)) {
        return;
    }
    s->rcu.refclk = s->refclk;

    busdev = SYS_BUS_DEVICE(dev);
    sysbus_mmio_map(busdev, 0, RCU_ADD);

    /* PMU */
    dev = DEVICE(&s->pwr);
    if (!sysbus_realize(SYS_BUS_DEVICE(&s->pwr), errp)) {
        return;
    }

    busdev = SYS_BUS_DEVICE(dev);
    sysbus_mmio_map(busdev, 0, 0x40007000);

    /* Cyclic Redundancy Check */
    dev = DEVICE(&s->crc);
    if (!sysbus_realize(SYS_BUS_DEVICE(&s->crc), errp)) {
        return;
    }

    busdev = SYS_BUS_DEVICE(dev);
    sysbus_mmio_map(busdev, 0, CRC_ADD);

    /* Random Number Generation */
    dev = DEVICE(&s->rng);
    if (!sysbus_realize(SYS_BUS_DEVICE(&s->rng), errp)) {
        return;
    }

    busdev = SYS_BUS_DEVICE(dev);
    sysbus_mmio_map(busdev, 0, RNG_ADD);

    /* System configuration controller */
    dev = DEVICE(&s->syscfg);
    if (!sysbus_realize(SYS_BUS_DEVICE(&s->syscfg), errp)) {
        return;
    }
    busdev = SYS_BUS_DEVICE(dev);
    sysbus_mmio_map(busdev, 0, SYSCFG_ADD);
    sysbus_connect_irq(busdev, 0, qdev_get_gpio_in(armv7m, SYSCFG_IRQ));

    /* GPIOs */
    for (i = 0; i < GD32F470XX_NUM_GPIOS; i++) {
        dev = DEVICE(&s->gpio[i]);
        if (!sysbus_realize(SYS_BUS_DEVICE(&s->gpio[i]), errp)) {
            return;
        }
        busdev = SYS_BUS_DEVICE(dev);
        sysbus_mmio_map(busdev, 0, gpio_addr[i]);
        qdev_pass_aliased_gpios(dev, NULL, dev_soc, gpio_pass[i]);
    }

    /* Attach UART (uses USART registers) and USART controllers */
    for (i = 0; i < GD32F470XX_NUM_USARTS; i++) {
        dev = DEVICE(&(s->usart[i]));
        qdev_prop_set_chr(dev, "chardev", serial_hd(i));
        if (!sysbus_realize(SYS_BUS_DEVICE(&s->usart[i]), errp)) {
            return;
        }
        busdev = SYS_BUS_DEVICE(dev);
        sysbus_mmio_map(busdev, 0, usart_addr[i]);
        sysbus_connect_irq(busdev, 0, qdev_get_gpio_in(armv7m, usart_irq[i]));
    }

    /* TODO: Implement TIMER0 / 7 */
    for (i = 0; i < GD32F470XX_NUM_TIMERS; i++) {
        dev = DEVICE(&(s->timer[i]));
        qdev_prop_set_uint64(dev, "clock-frequency", 1000000000);
        if (!sysbus_realize(SYS_BUS_DEVICE(&s->timer[i]), errp)) {
            return;
        }
        busdev = SYS_BUS_DEVICE(dev);
        sysbus_mmio_map(busdev, 0, timer_addr[i]);
        if (timer_irq[i] > 0) {
            sysbus_connect_irq(busdev, 0, qdev_get_gpio_in(armv7m, timer_irq[i]));
        }
    }

    /* ADC device, the IRQs are ORed together */
    if (!object_initialize_child_with_props(OBJECT(s), "adc-orirq",
                                            &s->adc_irqs, sizeof(s->adc_irqs),
                                            TYPE_OR_IRQ, errp, NULL)) {
        return;
    }
    object_property_set_int(OBJECT(&s->adc_irqs), "num-lines", GD32F470XX_NUM_ADCS,
                            &error_abort);
    if (!qdev_realize(DEVICE(&s->adc_irqs), NULL, errp)) {
        return;
    }
    qdev_connect_gpio_out(DEVICE(&s->adc_irqs), 0,
                          qdev_get_gpio_in(armv7m, ADC_IRQ));

    for (i = 0; i < GD32F470XX_NUM_ADCS; i++) {
        dev = DEVICE(&(s->adc[i]));
        if (!sysbus_realize(SYS_BUS_DEVICE(&s->adc[i]), errp)) {
            return;
        }
        busdev = SYS_BUS_DEVICE(dev);
        sysbus_mmio_map(busdev, 0, adc_addr[i]);
        sysbus_connect_irq(busdev, 0,
                           qdev_get_gpio_in(DEVICE(&s->adc_irqs), i));
    }

    /* SPI devices */
    for (i = 0; i < GD32F470XX_NUM_SPIS; i++) {
        dev = DEVICE(&(s->spi[i]));
        if (!sysbus_realize(SYS_BUS_DEVICE(&s->spi[i]), errp)) {
            return;
        }
        busdev = SYS_BUS_DEVICE(dev);
        sysbus_mmio_map(busdev, 0, spi_addr[i]);
        sysbus_connect_irq(busdev, 0, qdev_get_gpio_in(armv7m, spi_irq[i]));
    }

    /* I2C devices */
    for (i = 0; i < GD32F470XX_NUM_I2CS; i++) {
        dev = DEVICE(&(s->i2c[i]));
        if (!sysbus_realize(SYS_BUS_DEVICE(&s->i2c[i]), errp)) {
            return;
        }
        busdev = SYS_BUS_DEVICE(dev);
        sysbus_mmio_map(busdev, 0, i2c_addr[i]);
        qdev_connect_gpio_out(dev, 0, qdev_get_gpio_in(armv7m, i2c_irq[i])); // I2Cx_EV_IRQ
        qdev_connect_gpio_out(dev, 1, qdev_get_gpio_in(armv7m, i2c_irq[i] + 1)); // I2Cx_ER_IRQ
    }

    /* EXTI device */
    dev = DEVICE(&s->exti);
    if (!sysbus_realize(SYS_BUS_DEVICE(&s->exti), errp)) {
        return;
    }
    busdev = SYS_BUS_DEVICE(dev);
    sysbus_mmio_map(busdev, 0, EXTI_ADDR);
    for (i = 0; i < 16; i++) {
        sysbus_connect_irq(busdev, i, qdev_get_gpio_in(armv7m, exti_irq[i]));
    }
    for (i = 0; i < 16; i++) {
        qdev_connect_gpio_out(DEVICE(&s->syscfg), i, qdev_get_gpio_in(dev, i));
    }

    /* USB OTG FS device */
    dev = DEVICE(&s->usb_otg_fs);
    if (!sysbus_realize(SYS_BUS_DEVICE(&s->usb_otg_fs), errp)) {
        return;
    }
    busdev = SYS_BUS_DEVICE(dev);
    sysbus_mmio_map(busdev, 0, USB_OTG_FS_ADD);

    create_unimplemented_device("RTC",         0x40002800, 0x400);
    create_unimplemented_device("WWDGT",       0x40002C00, 0x400);
    create_unimplemented_device("FWDGT",       0x40003000, 0x400);
    create_unimplemented_device("I2S1_add",    0x40003400, 0x400);
    create_unimplemented_device("I2S2_add",    0x40004000, 0x400);
    create_unimplemented_device("CAN0",        0x40006400, 0x400);
    create_unimplemented_device("CAN1",        0x40006800, 0x400);
    create_unimplemented_device("DAC",         0x40007400, 0x400);
    create_unimplemented_device("SDIO",        0x40012C00, 0x400);
    create_unimplemented_device("FMC",         0x40023C00, 0x400);
    create_unimplemented_device("BKPSRAM",     0x40024000, 0x400);
    create_unimplemented_device("DMA0",        0x40026000, 0x400);
    create_unimplemented_device("DMA1",        0x40026400, 0x400);
    create_unimplemented_device("ENET",        0x40028000, 0x1400);
    create_unimplemented_device("USBHS",       0x40040000, 0x30000);
    create_unimplemented_device("DCI",         0x50050000, 0x400);
    create_unimplemented_device("EXMCSWREG",   0xA0000000, 0x1000);
    create_unimplemented_device("DES",         0x1FFF7A10, 0x200); // Device Electronic Signature
}

static const Property gd32f470xx_soc_properties[] = {
    DEFINE_PROP_STRING("soc-type", GD32F470XXState, soc_type),
};

static void gd32f470xx_soc_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    dc->realize = gd32f470xx_soc_realize;
    device_class_set_props(dc, gd32f470xx_soc_properties);
    /* No vmstate or reset required: device has no internal state */
}

static const TypeInfo gd32f470xx_soc_info = {
    .name          = TYPE_GD32F470XX_SOC,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(GD32F470XXState),
    .instance_init = gd32f470xx_soc_initfn,
    .class_init    = gd32f470xx_soc_class_init,
};

static void gd32f470xx_soc_types(void)
{
    type_register_static(&gd32f470xx_soc_info);
}

type_init(gd32f470xx_soc_types)
