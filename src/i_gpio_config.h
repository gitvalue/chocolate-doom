//
// GPIO Button Configuration Header
// 
// This file allows customizing GPIO pin assignments without editing i_gpio.c
// Uncomment and modify the pin assignments below to match your hardware setup
//

#ifndef __I_GPIO_CONFIG__
#define __I_GPIO_CONFIG__

// GPIO Button Configuration (WiringPi pin numbers)
// To customize, uncomment the desired configuration below

// === DEFAULT CONFIGURATION ===
// GPIO17 (pin 0) -> Forward (UP arrow)
// GPIO27 (pin 2) -> Backward (DOWN arrow)
// GPIO22 (pin 3) -> Turn Left (LEFT arrow)
// GPIO18 (pin 1) -> Turn Right (RIGHT arrow)
// GPIO15 (pin 16) -> Fire (CTRL)
// GPIO14 (pin 15) -> Use Item (SPACE)

#define GPIO_PIN_FORWARD    2
#define GPIO_PIN_BACKWARD   0
#define GPIO_PIN_LEFT       3
#define GPIO_PIN_RIGHT      29
#define GPIO_PIN_FIRE       27
#define GPIO_PIN_USE_ITEM   28
#define GPIO_PIN_PAUSE 26

// === ALTERNATIVE: NUMERIC KEYPAD LAYOUT ===
// Uncomment below to use alternative layout
// #define GPIO_PIN_FORWARD    8    // Up = 8
// #define GPIO_PIN_BACKWARD   2    // Down = 2
// #define GPIO_PIN_LEFT       4    // Left = 4
// #define GPIO_PIN_RIGHT      6    // Right = 6
// #define GPIO_PIN_FIRE       5    // Fire = 5
// #define GPIO_PIN_USE_ITEM   0    // Use = 0

// === ALTERNATIVE: WASD LAYOUT ===
// Uncomment below to use WASD-style layout
// #define GPIO_PIN_FORWARD    26   // W
// #define GPIO_PIN_BACKWARD   20   // S
// #define GPIO_PIN_LEFT       11   // A
// #define GPIO_PIN_RIGHT      27   // D
// #define GPIO_PIN_FIRE       10   // Fire
// #define GPIO_PIN_USE_ITEM   12   // Use

// Debounce time in milliseconds
// Lower values = faster response, higher chance of bounce
// Higher values = more stable, slightly higher latency
#define DEBOUNCE_MS 20

// Enable GPIO support (automatically detected if WiringPi is available)
// Set to 1 to enable, 0 to disable
#define ENABLE_GPIO 1

#endif // __I_GPIO_CONFIG__
