/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(blinky, LOG_LEVEL_INF); // Set the module name and log level

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   (250)

/* The devicetree node identifier for the "led0" alias. */
// #define LED0_NODE DT_ALIAS(led0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */

const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), led_gpios);

static struct k_timer led_timer;

void led_timer_handler(struct k_timer *dummy)
{
    gpio_pin_toggle_dt(&led);
}

int main(void)
{
	int ret;
	bool led_state = true;

    gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);

    // Initialize and start a periodic timer
    k_timer_init(&led_timer, led_timer_handler, NULL);
    k_timer_start(&led_timer, K_MSEC(1000), K_MSEC(1000));

	return 0;
}
