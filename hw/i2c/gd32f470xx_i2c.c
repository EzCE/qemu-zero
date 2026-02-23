/*
 * GD32F470XX I2C
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
#include "hw/i2c/gd32f470xx_i2c.h"
#include "hw/core/irq.h"
#include "qemu/log.h"
#include "qemu/module.h"
#include "trace.h"

static int gd32f470xx_i2c_raise_event_irq(GD32F470XXI2CState *s)
{
    bool raise_irq = false;

    if (GD_I2C_FIELD(s->i2c_reg[I2C_CTL1], I2C_CTL1_EVIE)) {
        if (GD_I2C_FIELD(s->i2c_reg[I2C_STAT0], EVIE_MASK)) {
            raise_irq = true;
        }
    }

    if (GD_I2C_FIELD(s->i2c_reg[I2C_CTL1], I2C_CTL1_BUFIE)) {
        if (GD_I2C_FIELD(s->i2c_reg[I2C_STAT0], BUFIE_MASK)) {
            raise_irq = true;
        }
    }

    trace_gd32f470xx_i2c_irq_event(s, s->i2c_reg[I2C_STAT0], raise_irq);

    if (raise_irq) {
        qemu_irq_raise(s->irq[0]);
    } else {
        qemu_irq_lower(s->irq[0]);
    }

    return 0;
}

static int gd32f470xx_i2c_raise_error_irq(GD32F470XXI2CState *s)
{
    bool raise_irq = false;

    if (GD_I2C_FIELD(s->i2c_reg[I2C_CTL1], I2C_CTL1_ERRIE)) {
        if (GD_I2C_FIELD(s->i2c_reg[I2C_STAT0], ERRIE_MASK)) {
            raise_irq = true;
        }
    }

    trace_gd32f470xx_i2c_irq_error(s, s->i2c_reg[I2C_STAT0], raise_irq);

    if (raise_irq) {
        qemu_irq_raise(s->irq[1]);
    } else {
        qemu_irq_lower(s->irq[1]);
    }

    return 0;
}

static void gd32f470xx_i2c_update_irqs(GD32F470XXI2CState *s)
{
    gd32f470xx_i2c_raise_event_irq(s);
    gd32f470xx_i2c_raise_error_irq(s);
}

static void gd32f470xx_i2c_reset(DeviceState *dev)
{
    GD32F470XXI2CState *s = GD32F470XX_I2C(dev);

    s->last_read = -1;
    s->last_wrote = -1;

    /* Set initial register values from GD32F4xx user manual */
    memset(s->i2c_reg, 0, sizeof(s->i2c_reg));
    s->i2c_reg[I2C_RT] = 0x00000002;
    s->address = 0xFF;

    qemu_set_irq(s->irq[0], 0);
    qemu_set_irq(s->irq[1], 0);

    i2c_end_transfer(s->bus);
}

static uint64_t gd32f470xx_i2c_read(void *opaque, hwaddr addr,
                                     unsigned int size)
{
    GD32F470XXI2CState *s = opaque;
    uint64_t value = 0;

    switch (addr) {
    case I2C_CTL0_ADD:
        value = s->i2c_reg[I2C_CTL0];
        s->last_read = I2C_CTL0;
        break;
    case I2C_CTL1_ADD:
        value = s->i2c_reg[I2C_CTL1];
        s->last_read = I2C_CTL1;
        break;
    case I2C_SADDR0_ADD:
        value = s->i2c_reg[I2C_SADDR0];
        s->last_read = I2C_SADDR0;
        break;
    case I2C_SADDR1_ADD:
        value = s->i2c_reg[I2C_SADDR1];
        s->last_read = I2C_SADDR1;
        break;
    case I2C_DATA_ADD:
        value = s->i2c_reg[I2C_DATA];
        s->i2c_reg[I2C_DATA] = 0;
        s->i2c_reg[I2C_STAT0] &= ~I2C_STAT0_RBNE;
        if (s->read_mode && GD_I2C_FIELD(s->i2c_reg[I2C_CTL0], I2C_CTL0_ACKEN)) {
            int byte = i2c_recv(s->bus); // Receive next byte ahead if ACKEN
            if (byte >= 0) {
                s->i2c_reg[I2C_DATA] = byte;
                s->i2c_reg[I2C_STAT0] |= I2C_STAT0_RBNE;
            }
        }
        s->last_read = I2C_DATA;
        break;
    case I2C_STAT0_ADD:
        value = s->i2c_reg[I2C_STAT0];
        s->last_read = I2C_STAT0;
        break;
    case I2C_STAT1_ADD:
        value = s->i2c_reg[I2C_STAT1];
        if (s->last_read == I2C_STAT0) {
            s->i2c_reg[I2C_STAT0] &= ~I2C_STAT0_ADDSEND;
            if (s->read_mode) {
                int byte = i2c_recv(s->bus);
                if (byte >= 0) {
                    s->i2c_reg[I2C_DATA] = byte;
                    s->i2c_reg[I2C_STAT0] |= I2C_STAT0_RBNE;
                }
            }
        }
        s->last_read = I2C_STAT1;
        break;
    case I2C_CKCFG_ADD:
        value = s->i2c_reg[I2C_CKCFG];
        s->last_read = I2C_CKCFG;
        break;
    case I2C_RT_ADD:
        value = s->i2c_reg[I2C_RT];
        s->last_read = I2C_RT;
        break;
    case I2C_FCTL_ADD:
        value = s->i2c_reg[I2C_FCTL];
        s->last_read = I2C_FCTL;
        break;
    case I2C_SAMCS_ADD:
        value = s->i2c_reg[I2C_SAMCS];
        s->last_read = I2C_SAMCS;
        break;
    default:
        qemu_log_mask(LOG_UNIMP,
                      "%s: Unimplemented I2C read 0x%"HWADDR_PRIx"\n", __func__,
                      addr);
        break;
    }

    trace_gd32f470xx_i2c_read(s, addr, size, value);
    gd32f470xx_i2c_update_irqs(s);
    return value;
}

static void gd32f470xx_i2c_write(void *opaque, hwaddr addr,
                       uint64_t val64, unsigned int size)
{
    GD32F470XXI2CState *s = opaque;
    uint32_t value = val64;
    trace_gd32f470xx_i2c_write(s, addr, size, val64);

    switch (addr) {
    case I2C_CTL0_ADD:
        s->i2c_reg[I2C_CTL0] = value;
        if (GD_I2C_FIELD(s->i2c_reg[I2C_CTL0], I2C_CTL0_START)) {
            s->i2c_reg[I2C_STAT0] |= I2C_STAT0_SBSEND;
            s->i2c_reg[I2C_STAT0] &= ~I2C_STAT0_BTC;
            s->i2c_reg[I2C_STAT1] |= I2C_STAT1_MASTER;
            s->i2c_reg[I2C_STAT1] |= I2C_STAT1_I2CBSY;
            s->i2c_reg[I2C_CTL0] &= ~I2C_CTL0_START;
            s->address = 0xFF;
        }
        if (GD_I2C_FIELD(s->i2c_reg[I2C_CTL0], I2C_CTL0_STOP)) {
            s->i2c_reg[I2C_STAT0] &= ~I2C_STAT0_BTC;
            s->i2c_reg[I2C_STAT0] |= I2C_STAT0_STPDET;
            s->i2c_reg[I2C_STAT1] &= ~I2C_STAT1_MASTER;
            s->i2c_reg[I2C_STAT1] &= ~I2C_STAT1_I2CBSY;
            s->i2c_reg[I2C_CTL0] &= ~I2C_CTL0_STOP;
            i2c_end_transfer(s->bus);
            s->address = 0xFF;
        }
        if (!GD_I2C_FIELD(value, I2C_CTL0_I2CEN)) {
            s->i2c_reg[I2C_STAT1] &= ~I2C_STAT1_MASTER;
            s->i2c_reg[I2C_STAT1] &= ~I2C_STAT1_I2CBSY;
            s->i2c_reg[I2C_CTL0] &= ~I2C_CTL0_START;
        }
        if (s->last_read == I2C_STAT0) {
            s->i2c_reg[I2C_STAT0] &= ~I2C_STAT0_STPDET;
        }
        s->last_wrote = I2C_CTL0;
        break;
    case I2C_DATA_ADD:
        s->i2c_reg[I2C_DATA] = value;
        if ((s->i2c_reg[I2C_STAT0] & I2C_STAT0_SBSEND) && s->address == 0xFF) {
            if (i2c_start_transfer(s->bus, value >> 1, value & 1)) {
                qemu_log_mask(LOG_GUEST_ERROR,
                              "%s: I2C start transfer to slave 0x%2x failed\n",
                              __func__, value >> 1);
            } else {
                s->address = GD_I2C_FIELD(s->i2c_reg[I2C_DATA], I2C_DATA_TRB);
                s->read_mode = value & 1;
                s->i2c_reg[I2C_STAT0] &= ~I2C_STAT0_SBSEND;
                s->i2c_reg[I2C_STAT0] |= I2C_STAT0_ADDSEND;
            }
        } else {
            if (i2c_send(s->bus, GD_I2C_FIELD(s->i2c_reg[I2C_DATA], I2C_DATA_TRB))) {
                qemu_log_mask(LOG_GUEST_ERROR,
                              "%s: I2C send failed\n", __func__);
            } else {
                s->i2c_reg[I2C_STAT0] |= I2C_STAT0_BTC;
                s->i2c_reg[I2C_STAT0] |= I2C_STAT0_TBE;
            }
        }
        s->i2c_reg[I2C_DATA] = 0;
        s->i2c_reg[I2C_STAT0] |= I2C_STAT0_TBE;
        s->last_wrote = I2C_DATA;
        break;
    case I2C_STAT0_ADD:
        s->i2c_reg[I2C_STAT0] = value;
        s->last_wrote = I2C_STAT0;
        break;
    case I2C_STAT1_ADD:
        s->i2c_reg[I2C_STAT1] = value;
        s->last_wrote = I2C_STAT1;
        break;
    default:
        qemu_log_mask(LOG_UNIMP,
                      "%s: Unimplemented I2C write 0x%"HWADDR_PRIx"\n",
                      __func__, addr);
        break;
    }

    gd32f470xx_i2c_update_irqs(s);
}

static const MemoryRegionOps gd32f470xx_i2c_ops = {
    .read = gd32f470xx_i2c_read,
    .write = gd32f470xx_i2c_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

static void gd32f470xx_i2c_realize(DeviceState *dev, Error **errp)
{
    GD32F470XXI2CState *s = GD32F470XX_I2C(dev);
    s->bus = i2c_init_bus(dev, NULL);

    memory_region_init_io(&s->mmio, OBJECT(dev), &gd32f470xx_i2c_ops, s,
                          TYPE_GD32F470XX_I2C, 0x400);
    sysbus_init_mmio(SYS_BUS_DEVICE(dev), &s->mmio);
    qdev_init_gpio_out(DEVICE(s), s->irq, 2);
}

static void gd32f470xx_i2c_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    device_class_set_legacy_reset(dc, gd32f470xx_i2c_reset);
    dc->realize = gd32f470xx_i2c_realize;
}

static const TypeInfo gd32f470xx_i2c_info = {
    .name          = TYPE_GD32F470XX_I2C,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(GD32F470XXI2CState),
    // .instance_init = gd32f470xx_i2c_init,
    .class_init    = gd32f470xx_i2c_class_init,
};

static void gd32f470xx_i2c_register_types(void)
{
    type_register_static(&gd32f470xx_i2c_info);
}

type_init(gd32f470xx_i2c_register_types)
