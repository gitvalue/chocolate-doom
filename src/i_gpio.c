//
// Copyright(C) 2026
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//    GPIO button input interface for hardware buttons on Raspberry Pi
//    Uses WiringPi library for GPIO access
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "config.h"
#include "doomkeys.h"
#include "d_event.h"
#include "i_gpio.h"
#include "i_gpio_config.h"

#ifdef HAVE_TFT_ST7735S_WPI
#include <wiringPi.h>
#endif

// GPIO button configuration
typedef struct
{
    int gpio_pin;           // WiringPi pin number
    int key_code;           // DOOM key code
    int last_state;         // Last read state (for debouncing)
    unsigned long last_time; // Last change time (for debouncing)
} gpio_button_t;

// Debounce time in milliseconds
#define DEBOUNCE_MS 20

// GPIO pin assignments (WiringPi numbering)
// BCM GPIO -> WiringPi: 17->0, 27->2, 22->3, 18->1, 15->16, 14->15
static gpio_button_t gpio_buttons[] = {
    // Forward (UP arrow)
    { GPIO_PIN_FORWARD, KEY_UPARROW, -1, 0 },
    // Backward (DOWN arrow)
    { GPIO_PIN_BACKWARD, KEY_DOWNARROW, -1, 0 },
    // Turn left (LEFT arrow)
    { GPIO_PIN_LEFT, KEY_LEFTARROW, -1, 0 },
    // Turn right (RIGHT arrow)
    { GPIO_PIN_RIGHT, KEY_RIGHTARROW, -1, 0 },
    // Fire (CTRL)
    { GPIO_PIN_FIRE, KEY_RCTRL, -1, 0 },
    // Use item (SPACE)
    { GPIO_PIN_USE_ITEM, KEY_ENTER, -1, 0 },
    { GPIO_PIN_PAUSE, KEY_ESCAPE, -1, 0 }
};

#define NUM_BUTTONS (sizeof(gpio_buttons) / sizeof(gpio_buttons[0]))

static boolean gpio_initialized = false;

// Get milliseconds elapsed since reference time
static unsigned long get_millis(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);
}

// Initialize GPIO pins
void I_InitGPIO(void)
{
#ifdef HAVE_TFT_ST7735S_WPI
    int i;

    if (gpio_initialized)
    {
        return;
    }

    // Initialize WiringPi
    if (wiringPiSetup() == -1)
    {
        fprintf(stderr, "GPIO: Failed to initialize WiringPi\n");
        return;
    }

    // Configure all button pins as inputs with pull-up resistors
    for (i = 0; i < NUM_BUTTONS; i++)
    {
        pinMode(gpio_buttons[i].gpio_pin, INPUT);
        pullUpDnControl(gpio_buttons[i].gpio_pin, PUD_DOWN);
        gpio_buttons[i].last_state = -1;
        gpio_buttons[i].last_time = 0;
    }

    gpio_initialized = true;
    printf("GPIO: Initialized %lu button pins\n", (unsigned long)NUM_BUTTONS);
#else
    printf("GPIO: Support not compiled in\n");
#endif
}

// Poll GPIO buttons and generate key events
void I_ReadGPIO(void)
{
#ifdef HAVE_TFT_ST7735S_WPI
    
    printf("Here1\n");

    int i;
    int current_state;
    unsigned long current_time;
    event_t event;

    if (!gpio_initialized)
    {
        return;
    }
    
     printf("Here2\n");

    current_time = get_millis();

    for (i = 0; i < NUM_BUTTONS; i++)
    {
        gpio_button_t *btn = &gpio_buttons[i];

        // Read current pin state (active low - 0 means pressed)
        current_state = digitalRead(btn->gpio_pin);
        printf("current_state=%d for btn=%d\n", current_state, btn->gpio_pin);

        // Skip if state hasn't changed
        if (current_state == btn->last_state)
        {
            continue;
        }

        // Debounce: ensure minimum time has passed since last state change
        if (current_time - btn->last_time < DEBOUNCE_MS)
        {
            continue;
        }
        
        printf("Here3\n");

        // State changed and debounce time passed
        btn->last_state = current_state;
        btn->last_time = current_time;

        // Generate key event (0 = pressed, 1 = released)
        memset(&event, 0, sizeof(event_t));
        event.type = (current_state == 1) ? ev_keydown : ev_keyup;
        event.data1 = btn->key_code;

        D_PostEvent(&event);
    }
#endif
}

// Shutdown GPIO
void I_ShutdownGPIO(void)
{
#ifdef HAVE_TFT_ST7735S_WPI
    int i;
    
    if (!gpio_initialized)
    {
        return;
    }

    // Set all pins back to input state before exiting
    for (i = 0; i < NUM_BUTTONS; i++)
    {
        pinMode(gpio_buttons[i].gpio_pin, INPUT);
    }

    gpio_initialized = false;
    printf("GPIO: Shutdown complete\n");
#endif
}
