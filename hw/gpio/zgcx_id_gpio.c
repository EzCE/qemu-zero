/*
 * ZGCx HW ID GPIO
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
#include "hw/core/irq.h"
#include "hw/core/sysbus.h"
#include "hw/gpio/zgcx_id_gpio.h"
#include "qemu/log.h"
#include "qemu/error-report.h"

static void zgcx_id_gpio_realize(DeviceState *dev, Error **errp)
{
    ZGCxIDGpioState *s = ZGCX_ID_GPIO(dev);

    qdev_init_gpio_out(dev, s->gpio, 3);
}

static void zgcx_id_gpio_reset(DeviceState *dev)
{
    ZGCxIDGpioState *s = ZGCX_ID_GPIO(dev);

    qemu_set_irq(s->gpio[0], (s->hw_version >> 0) & 1);
    qemu_set_irq(s->gpio[1], (s->hw_version >> 1) & 1);
    qemu_set_irq(s->gpio[2], (s->hw_version >> 2) & 1);
}

static const Property zgcx_id_gpio_properties[] = {
    DEFINE_PROP_UINT8("hw-version", ZGCxIDGpioState, hw_version, 0x0),
};

static void zgcx_id_gpio_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    device_class_set_props(dc, zgcx_id_gpio_properties);
    dc->realize = zgcx_id_gpio_realize;
    device_class_set_legacy_reset(dc, zgcx_id_gpio_reset);
}

static const TypeInfo zgcx_id_gpio_info = {
    .name          = TYPE_ZGCX_ID_GPIO,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(ZGCxIDGpioState),
    .class_init    = zgcx_id_gpio_class_init,
};

static void zgcx_id_gpio_register_types(void)
{
    type_register_static(&zgcx_id_gpio_info);
}

type_init(zgcx_id_gpio_register_types);
