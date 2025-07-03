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
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), button_gpios);

#define DEBOUNCE_DELAY_MS 50

static struct gpio_callback button_cb;

static struct k_timer led_timer;

static const uint32_t blink_intervals[] = {1000, 500, 250}; // ms
static int current_rate = 0;

void led_timer_handler(struct k_timer *dummy)
{
    gpio_pin_toggle_dt(&led);
}

void button_pressed_cb(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    static int64_t last_press_time = 0;
    int64_t now = k_uptime_get();

    if (now - last_press_time < DEBOUNCE_DELAY_MS) {
        return;
    }
    last_press_time = now;

    current_rate = (current_rate + 1) % ARRAY_SIZE(blink_intervals);
    k_timer_start(&led_timer,
                  K_MSEC(blink_intervals[current_rate]),
                  K_MSEC(blink_intervals[current_rate]));
}


int main(void)
{
	int ret;
	bool led_state = true;

    gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&button, GPIO_INPUT);

    gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
    gpio_init_callback(&button_cb, button_pressed_cb, BIT(button.pin));
    gpio_add_callback(button.port, &button_cb);

    // Initialize and start a periodic timer
    k_timer_init(&led_timer, led_timer_handler, NULL);
    k_timer_start(&led_timer,
                  K_MSEC(blink_intervals[current_rate]),
                  K_MSEC(blink_intervals[current_rate]));

    LOG_INF("System ready. Press button to change LED blink rate.");

	return 0;
}
