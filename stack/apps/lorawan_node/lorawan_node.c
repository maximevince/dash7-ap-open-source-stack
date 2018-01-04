/* * OSS-7 - An opensource implementation of the DASH7 Alliance Protocol for ultra
 * lowpower wireless sensor communication
 *
 * Copyright 2017 University of Antwerp
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

#include "log.h"

#include "lorawan_stack.h"
#include "scheduler.h"
#include "timeServer.h"
#include "timer.h"

// TODO
static uint8_t devEui[] = { 0xBE, 0x7A, 0x00, 0x00, 0x00, 0x00, 0x1B, 0x81 };
static uint8_t appEui[] = { 0xBE, 0x7A, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x9F };
static uint8_t appKey[] = { 0x7E, 0xEF, 0x56, 0xEC, 0xDA, 0x1D, 0xD5, 0xA4, 0x70, 0x59, 0xFD, 0x35, 0x9C, 0xE6, 0x80, 0xCD };


void read_sensor_task() {
  static uint8_t msg_counter = 0;
  log_print_string("read sensor");
  lorawan_stack_error_t e = lorawan_stack_send(&msg_counter, 1, 2, false);
  if(e == LORAWAN_STACK_ERROR_OK) {
    log_print_string("TX ok (%i)", msg_counter);
  } else {
    log_print_string("TX failed");
  }

  msg_counter++;
  timer_post_task_delay(&read_sensor_task, 60 * TIMER_TICKS_PER_SEC);
}

void on_join_completed(bool success) {
  if(!success) {
    log_print_string("join failed");
    // ...
  } else {
    log_print_string("join succeeded");
  }
}

void bootstrap()
{
    log_print_string("Device booted\n");

    lorawan_stack_init(devEui, appEui, appKey, &on_join_completed, NULL);

    sched_register_task(&read_sensor_task);
    timer_post_task_delay(&read_sensor_task, 60 * TIMER_TICKS_PER_SEC);
}
