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

#include "qemu/osdep.h"
#include "hw/i2c/i2c.h"
#include "hw/core/qdev-properties.h"
#include "qapi/error.h"
#include "hw/i2c/stc3100.h"
#include "qemu/log.h"
#include "trace.h"

static const uint8_t stc3100_id[] = { 0x10, 0x00, 0x00, 0x00,
                                      0x00, 0x00, 0x00, 0x00 };

static int stc3100_event(I2CSlave *s, enum i2c_event event)
{
    STC3100State *dev = STC3100(s);

    switch (event) {
    case I2C_START_RECV:
        dev->wait_reg = false;
        break;
    case I2C_START_SEND:
        dev->wait_reg = true;
        break;
    case I2C_FINISH:
        break;
    default:
        break;
    }

    trace_stc3100_event(event);
    return 0;
}

static uint8_t stc3100_recv(I2CSlave *s)
{
    STC3100State *dev = STC3100(s);

    uint8_t value = 0;

    switch (dev->reg) {
    case STC3100_MODE_ADD:
        value = dev->mode;
        break;
    case STC3100_CTRL_ADD:
        value = dev->ctrl;
        break;
    case STC3100_CHARGE_LOW_ADD:
        value = (dev->charge & 0xFF);
        break;
    case STC3100_CHARGE_HIGH_ADD:
        value = (dev->charge >> 8);
        break;
    case STC3100_COUNTER_LOW_ADD:
        value = (dev->counter & 0xFF);
        break;
    case STC3100_COUNTER_HIGH_ADD:
        value = (dev->counter >> 8);
        break;
    case STC3100_CURRENT_LOW_ADD:
        value = (dev->current & 0xFF);
        break;
    case STC3100_CURRENT_HIGH_ADD:
        value = (dev->current >> 8);
        break;
    case STC3100_VOLTAGE_LOW_ADD:
        value = (dev->voltage & 0xFF);
        break;
    case STC3100_VOLTAGE_HIGH_ADD:
        value = (dev->voltage >> 8);
        break;
    case STC3100_TEMPERATURE_LOW_ADD:
        value = (dev->temp & 0xFF);
        break;
    case STC3100_TEMPERATURE_HIGH_ADD:
        value = (dev->temp >> 8);
        break;
    default:
        if (dev->reg >= STC3100_ID0_ADD &&
            dev->reg <= STC3100_ID7_ADD) {

            value = stc3100_id[dev->reg - STC3100_ID0_ADD];
        } else if (dev->reg >= STC3100_RAM_BASE_ADD && 
                   dev->reg <= STC3100_RAM_END_ADD) {

            value = dev->ram[dev->reg - STC3100_RAM_BASE_ADD];
        }
        break;
    }

    trace_stc3100_recv(value, dev->reg);
    dev->reg += 1; /* Increment register for next read */

    return value;
}

static int stc3100_send(I2CSlave *s, uint8_t data)
{
    STC3100State *dev = STC3100(s);

    if (dev->wait_reg) {
        dev->reg = data;
        dev->wait_reg = false;
        trace_stc3100_select_reg(dev->reg);
        return 0;
    } else if (dev->reg == STC3100_MODE_ADD) {
        dev->mode = data;
    } else if (dev->reg == STC3100_CTRL_ADD) {
        dev->ctrl = data;
    } else if (dev->reg >= STC3100_RAM_BASE_ADD &&
               dev->reg <= STC3100_RAM_END_ADD) {
        dev->ram[dev->reg - STC3100_RAM_BASE_ADD] = data;
    } else {
        qemu_log_mask(LOG_GUEST_ERROR,
              "%s: Master sent to invalid reg 0x%02x\n",
              __func__, dev->reg);
        return 0;
    }

    trace_stc3100_send(data, dev->reg);
    dev->reg += 1; /* Increment register for next write */

    return 0;
}

static void stc3100_init(Object *obj)
{
    STC3100State *dev = STC3100(obj);

    dev->reg = 0;
    dev->mode = 0;
    dev->ctrl = 0;
    dev->charge = STC3100_CHARGE;
    dev->counter = 1;
    dev->voltage = STC3100_VOLTAGE;
    dev->current = STC3100_CURRENT;
    dev->temp = STC3100_TEMP;
}

static void stc3100_class_init(ObjectClass *klass, const void *data)
{
    I2CSlaveClass *k = I2C_SLAVE_CLASS(klass);

    k->event = stc3100_event;
    k->recv  = stc3100_recv;
    k->send  = stc3100_send;
}

static const TypeInfo stc3100_info = {
    .name          = TYPE_STC3100,
    .parent        = TYPE_I2C_SLAVE,
    .instance_size = sizeof(STC3100State),
    .instance_init = stc3100_init,
    .class_init    = stc3100_class_init,
};

static void stc3100_register_types(void)
{
    type_register_static(&stc3100_info);
}

type_init(stc3100_register_types);
