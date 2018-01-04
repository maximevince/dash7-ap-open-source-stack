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

/*! \file cortus_timer.c
 *
 *  \author soomee
 *
 */


#include <stdbool.h>
#include <stdint.h>

#include "hwtimer.h"
#include "hwatomic.h"

#include "machine/sfradr.h"
#include "machine/ic.h"
#include "machine/timer.h"
#include "machine/timer_cap.h"
#include "machine/timer_cmp.h"

#if defined(FRAMEWORK_LOG_ENABLED)
#include <stdio.h>
#endif

#define HWTIMER_NUM 1

static timer_callback_t compare_f = 0x0;
static timer_callback_t overflow_f = 0x0;
static bool timer_inited = false;
static uint8_t FREQ;



error_t hw_timer_init(hwtimer_id_t timer_id, uint8_t frequency, timer_callback_t compare_callback, timer_callback_t overflow_callback)
{
   if(timer_id >= HWTIMER_NUM)
      return ESIZE;
   if(timer_inited)
      return EALREADY;
   if(frequency != HWTIMER_FREQ_1MS && frequency != HWTIMER_FREQ_32K)
      return EINVAL;

   start_atomic();
         compare_f = compare_callback;
         overflow_f = overflow_callback;
         timer_inited = true;

         // clear & disable
         timer1->enable = 0;

         timer1_cmpa->status = 0;
         timer1_cmpa->enable = 0;

         timer1_cmpb->status = 0;
         timer1_cmpb->enable = 0;

         timer1_capa->status = 0;
         timer1_capa->enable = 0;

         // init
         if(frequency == HWTIMER_FREQ_32K)
            timer1->period = 0x0061A800;
         else
            timer1->period = 0x0C350000;
         timer1->prescaler = 0;
         timer1->tclk_sel = 3; // 3.125MHz

         if(frequency == HWTIMER_FREQ_32K)
            timer1_cmpa->compare = 0x0061A7FF;
         else
            timer1_cmpa->compare = 0x0C34FFFF;
         irq[IRQ_TIMER1_CMPA].ipl = 0;
         irq[IRQ_TIMER1_CMPA].ien = 1;

         // enable
         timer1->enable = 1;

         timer1_cmpa->enable = 1;
         timer1_cmpa->mask = 1;

         timer1_capa->in_cap_sel = 0x4; // software capture
         timer1_capa->enable = 1; // 1: enable timer to interrupt

         FREQ = frequency;
   end_atomic();
   return SUCCESS;
}



hwtimer_tick_t hw_timer_getvalue(hwtimer_id_t timer_id)
{
   if(timer_id >= HWTIMER_NUM || (!timer_inited))
      return 0;
   else
   {
      timer1_capa->capture = 1;
      while(!timer1_capa->status)
         ;
      uint32_t value;
      if(FREQ == HWTIMER_FREQ_32K)
         value = timer1_capa->value / 98;
      else
         value = timer1_capa->value / 3125;
      timer1_capa->capture = 0;
      timer1_capa->status = 0;
#if defined(FRAMEWORK_LOG_ENABLED)
  printf("cap = %d\n", value);
#endif

      return value;
   }
}



error_t hw_timer_schedule(hwtimer_id_t timer_id, hwtimer_tick_t tick )
{
   if(timer_id >= HWTIMER_NUM)
      return ESIZE;
   if(!timer_inited)
      return EOFF;

   start_atomic();
      timer1_cmpb->status = 0;
      timer1_cmpb->enable = 0;

      if(FREQ == HWTIMER_FREQ_32K)
         timer1_cmpb->compare = tick * 98;
      else
         timer1_cmpb->compare = tick * 3125;
      irq[IRQ_TIMER1_CMPB].ipl = 0;
      irq[IRQ_TIMER1_CMPB].ien = 1;

      timer1_cmpb->enable = 1;
      timer1_cmpb->mask = 1;
   end_atomic();
}



error_t hw_timer_cancel(hwtimer_id_t timer_id)
{
   if(timer_id >= HWTIMER_NUM)
      return ESIZE;
   if(!timer_inited)
      return EOFF;

   start_atomic();
      timer1_cmpb->status = 0;
      timer1_cmpb->enable = 0;
   end_atomic();
}



error_t hw_timer_counter_reset(hwtimer_id_t timer_id)
{
   if(timer_id >= HWTIMER_NUM)
      return ESIZE;
   if(!timer_inited)
      return EOFF;

   start_atomic();
      timer1->enable = 0;
      timer1_cmpa->status = 0;
      timer1_cmpa->enable = 0;
      timer1_cmpb->status = 0;
      timer1_cmpb->enable = 0;

      timer1->enable = 1;
      timer1_cmpa->enable = 1;
      timer1_cmpa->mask = 1;
   end_atomic();
}



bool hw_timer_is_overflow_pending(hwtimer_id_t timer_id)
{
   if(timer_id >= HWTIMER_NUM)
      return false;

   start_atomic();
      bool is_pending = timer1_cmpa->status;
   end_atomic();
   return is_pending;	
}



bool hw_timer_is_interrupt_pending(hwtimer_id_t timer_id)
{
   if(timer_id >= HWTIMER_NUM)
      return false;

   start_atomic();
      bool is_pending = timer1_cmpb->status;
   end_atomic();
   return is_pending;	
}



void interrupt_handler(IRQ_TIMER1_CMPA)
{
#if defined(FRAMEWORK_LOG_ENABLED)
  printf("hit! CMPA = %d\n", hw_timer_getvalue(0));
#endif
  timer1_cmpa->status = 0;
  timer1_cmpb->status = 0;

  if (overflow_f != 0x0)
    overflow_f();
}



void interrupt_handler(IRQ_TIMER1_CMPB)
{
#if defined(FRAMEWORK_LOG_ENABLED)
  printf("hit! CMPB = %d\n", hw_timer_getvalue(0));
#endif
  timer1_cmpa->status = 0;
  timer1_cmpb->status = 0;
  timer1_cmpb->enable = 0;

  if (compare_f != 0x0)
    compare_f();
}
