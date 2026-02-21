/*
 * ZGC4 GasGauge I2C Device
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
#include "hw/i2c/gasgauge_i2c.h"
#include "trace.h"

static int gasgauge_i2c_event(I2CSlave *s, enum i2c_event event)
{
    // GasGaugeI2CState *dev = GASGAUGE_I2C(s);

    switch (event) {
    case I2C_START_RECV:
        break;
    case I2C_START_SEND:
        break;
    case I2C_FINISH:
        break;
    default:
        break;
    }

    trace_gasgauge_i2c_event(event);
    return 0;
}

static uint8_t gasgauge_i2c_recv(I2CSlave *s)
{
    GasGaugeI2CState *dev = GASGAUGE_I2C(s);

    trace_gasgauge_i2c_recv(dev->value);
    return dev->value;
}

static int gasgauge_i2c_send(I2CSlave *s, uint8_t data)
{
    GasGaugeI2CState *dev = GASGAUGE_I2C(s);

    dev->reg = data;
    trace_gasgauge_i2c_send(data);
    return 0;
}

static void gasgauge_i2c_init(Object *obj)
{
    GasGaugeI2CState *dev = GASGAUGE_I2C(obj);

    dev->reg = 0;
    dev->value = 0;
}

static void gasgauge_i2c_class_init(ObjectClass *klass, const void *data)
{
    I2CSlaveClass *k = I2C_SLAVE_CLASS(klass);

    k->event = gasgauge_i2c_event;
    k->recv  = gasgauge_i2c_recv;
    k->send  = gasgauge_i2c_send;
}

static const TypeInfo gasgauge_i2c_info = {
    .name          = TYPE_GASGAUGE_I2C,
    .parent        = TYPE_I2C_SLAVE,
    .instance_size = sizeof(GasGaugeI2CState),
    .instance_init = gasgauge_i2c_init,
    .class_init    = gasgauge_i2c_class_init,
};

static void gasgauge_i2c_register_types(void)
{
    type_register_static(&gasgauge_i2c_info);
}

type_init(gasgauge_i2c_register_types);
