/* * OSS-7 - An opensource implementation of the DASH7 Alliance Protocol for ultra
 * lowpower wireless sensor communication
 *
 * Copyright 2015 University of Antwerp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*! \file cortus_gpio.c
 *
 *  \author junghoon
 *  \author philippe.nunes@cortus.com
 */

#include "hwgpio.h"
#include <assert.h>
#include "hwatomic.h"
#include "machine/ic.h"
#include "machine/gpio.h"
#include <machine/cpu.h>
#include "cortus_gpio.h"
#include "cortus_mcu.h"

#define NUM_GPIOINT 16  // GPIO1 is only available for gpio interrupts.

#define PORT_BASE(pin)  ((GPio_edge*)(pin & ~0xFF)) // the LSB byte is used to set the pin number

// There is a limitation for using gpio interrupts. You can use gpio interrupts upto 16 pins in only gpio1 module(0 to 15).
static gpio_inthandler_t gpio_callback[NUM_GPIOINT];


__LINK_C void __gpio_init()
{
    // initialize interrupts[]
    for(int i = 0; i<NUM_GPIOINT; i++) gpio_callback[i] = 0x00;

    // enable interrupt for GPIO1
    irq[IRQ_GPIO_EDGE1].ipl = 0;
    irq[IRQ_GPIO_EDGE1].ien = 1;
    /* ic->ien = 1;// Don't forget this setting before running program */ 
}

__LINK_C error_t hw_gpio_configure_pin(pin_id_t pin_id, bool int_allowed, uint8_t mode, unsigned int out)
{
    if((int_allowed) && (gpio_callback[GPIO_PIN(pin_id)] != 0x00)) return EBUSY;

    GPio_edge *TGpio =   (GPio_edge*) PORT_BASE(pin_id);
    TGpio->dir  &=  (0x0ffffffff ^ (1 << GPIO_PIN(pin_id)));
    TGpio->dir  |=  (mode << GPIO_PIN(pin_id));    // mode => 0:input 1:output
    TGpio->out  &=  (0x0ffffffff ^ (1 << GPIO_PIN(pin_id)));
    TGpio->out  |=  (out << GPIO_PIN(pin_id));

    return SUCCESS;
}

__LINK_C error_t hw_gpio_set(pin_id_t pin_id)
{
    GPio_edge *TGpio =  (GPio_edge*) PORT_BASE(pin_id);
    TGpio->out  |= (1 << GPIO_PIN(pin_id));
    return SUCCESS;
}

__LINK_C error_t hw_gpio_clr(pin_id_t pin_id)
{
    GPio_edge *TGpio = (GPio_edge*) PORT_BASE(pin_id);
    TGpio->out &= (0x0ffffffff ^ (1 << GPIO_PIN(pin_id)));
    return SUCCESS;
}

__LINK_C error_t hw_gpio_toggle(pin_id_t pin_id)
{
    GPio_edge *TGpio = (GPio_edge*) PORT_BASE(pin_id);
    TGpio->out ^= (1<<GPIO_PIN(pin_id));
    return SUCCESS;
}

__LINK_C bool hw_gpio_get_out(pin_id_t pin_id)
{
    GPio_edge *TGpio = (GPio_edge*) PORT_BASE(pin_id);
    return ((TGpio->out >> GPIO_PIN(pin_id))&1);
}

__LINK_C bool hw_gpio_get_in(pin_id_t pin_id)
{
    GPio_edge *TGpio = (GPio_edge*) PORT_BASE(pin_id);
    return ((TGpio->in >> GPIO_PIN(pin_id))&1);
}

void interrupt_handler(IRQ_GPIO_EDGE1)
{
    uint8_t event_mask = 0; // This is a dummy var for compatible outline of call function.
    uint32_t inreg, old_inreg, mask, i;
    GPio_edge *TGpio = (GPio_edge*) SFRADR_GPIO_EDGE1;
    inreg       = TGpio->in;
    old_inreg   = TGpio->old_in;
    mask        = TGpio->mask;
    pin_id_t pin_id;

    //DPRINT ("INT in %02x old %02x", inreg, old_inreg);

    for(i=0; i<NUM_GPIOINT; i++)
    {
        if((gpio_callback[i] != 0x00) && (mask & 0x01))
        {
            if( (inreg&0x01) != (old_inreg&0x01) )
            {
                pin_id = PIN(SFRADR_GPIO_EDGE1, i);
                gpio_callback[i](pin_id, event_mask);
                break;
            }
        }

        inreg       >>= 1;
        old_inreg   >>= 1;
        mask        >>= 1;
    }
}

__LINK_C error_t hw_gpio_configure_interrupt(pin_id_t pin_id, gpio_inthandler_t callback, uint8_t event_mask)
{
    if((GPIO_PIN(pin_id) >= NUM_GPIOINT) || (gpio_callback[GPIO_PIN(pin_id)] != 0x00)) return EINVAL;
   
    start_atomic();
    GPio_edge *TGpio        = (GPio_edge *) SFRADR_GPIO_EDGE1;
    gpio_callback[GPIO_PIN(pin_id)]   = callback;
    TGpio->old_in           = TGpio->in;

    TGpio->edge = 0x1; // Clear all edges
    TGpio->level_sel |= (1<<GPIO_PIN(pin_id));// Select pin to interrupt

    if (event_mask == GPIO_RISING_EDGE)
        TGpio->rs_edge_sel = 0x1;
    else if (event_mask == GPIO_FALLING_EDGE)
        TGpio->fl_edge_sel = 0x1;
    else
    {
        end_atomic();
        return FAIL;
    }

    end_atomic();

    return SUCCESS;
}
__LINK_C error_t hw_gpio_enable_interrupt(pin_id_t pin_id)
{
    start_atomic();
    GPio_edge *TGpio = (GPio_edge*) SFRADR_GPIO_EDGE1;

    // update mask register
    TGpio->mask |= (1<<GPIO_PIN(pin_id));

    //DPRINT ("Enable: mask = %08x pin_id %04x in %02x", gpio_edge->mask, GPIO_PIN(pin_id), gpio_edge->in);

    end_atomic();

    return SUCCESS;
}

__LINK_C error_t hw_gpio_disable_interrupt(pin_id_t pin_id)
{
    start_atomic();
    GPio_edge *TGpio = (GPio_edge*) SFRADR_GPIO_EDGE1;

    // update mask register
    TGpio->mask  &= (0x0ffffffff ^ (1<<GPIO_PIN(pin_id)));

    //DPRINT ("Disable: mask = %08x pin_id %04x in %02x", gpio_edge->mask, GPIO_PIN(pin_id), gpio_edge->in);

    end_atomic();

    return SUCCESS;
}

__LINK_C error_t hw_gpio_set_edge_interrupt(pin_id_t pin_id, uint8_t edge)
{
    GPio_edge *TGpio = (GPio_edge*) PORT_BASE(pin_id);

    start_atomic();
    TGpio->mask  &= (0x0ffffffff ^ (1<<GPIO_PIN(pin_id)));

    TGpio->edge = 0x1; // Clear all edges
    TGpio->old_in = TGpio->in;
    TGpio->level_sel |= (1<<GPIO_PIN(pin_id)); // Select pin to interrupt

    if (edge == GPIO_RISING_EDGE)
        TGpio->rs_edge_sel = 0x1;
    else
        TGpio->fl_edge_sel = 0x1;

    //DPRINT ("id %04x edge %d level_sel = %02x in %02x old %02x", GPIO_PIN(pin_id), edge, gpio_edge->level_sel, gpio_edge->in, gpio_edge->old_in);

    end_atomic();

    return SUCCESS;
}
