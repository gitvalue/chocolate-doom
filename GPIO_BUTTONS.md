# GPIO Button Configuration for Chocolate Doom

This document describes the GPIO button support for controlling Chocolate Doom on Raspberry Pi using hardware buttons.

## Hardware Setup

### GPIO Pin Assignments

The following GPIO pins are configured for button input:

| GPIO Pin (BCM) | WiringPi Pin | Action | Keyboard Key |
|---|---|---|---|
| GPIO 17 | 0 | Step Forward | UP Arrow |
| GPIO 27 | 2 | Step Backward | DOWN Arrow |
| GPIO 22 | 3 | Turn Left | LEFT Arrow |
| GPIO 18 | 1 | Turn Right | RIGHT Arrow |
| GPIO 15 | 16 | Fire | CTRL |
| GPIO 14 | 15 | Use Item | SPACE |

### Wiring Requirements

Each button should be wired between a GPIO pin and GND (ground) with:
- **Pull-up resistor**: 10kΩ resistor between the GPIO pin and 3.3V (optional, WiringPi enables internal pull-ups)
- **Button debouncing**: The software includes 20ms debounce filtering

**Circuit diagram for each button:**
```
     3.3V
      |
    10kΩ (optional)
      |
    GPIO Pin ----o Button o---- GND
```

## Software Configuration

### Enabling GPIO Support

GPIO support is compiled in if the WiringPi library is available. The feature can be enabled/disabled by compiling with or without WiringPi.

To check if GPIO support is available:
```bash
grep "ENABLE_GPIO\|wiringPi" /home/dmitry/Documents/chocolate-doom/config.h
```

### Button Mapping

The GPIO button mappings are defined in `src/i_gpio.c`. To change the button assignments, edit the `gpio_buttons[]` array:

```c
static gpio_button_t gpio_buttons[] = {
    { 0, KEY_UPARROW, -1, 0 },      // GPIO17 -> Forward
    { 2, KEY_DOWNARROW, -1, 0 },    // GPIO27 -> Backward
    { 3, KEY_LEFTARROW, -1, 0 },    // GPIO22 -> Turn Left
    { 1, KEY_RIGHTARROW, -1, 0 },   // GPIO18 -> Turn Right
    { 16, KEY_RCTRL, -1, 0 },       // GPIO15 -> Fire
    { 15, ' ', -1, 0 },             // GPIO14 -> Use Item
};
```

### Debounce Configuration

The debounce time can be adjusted in `src/i_gpio.c`:

```c
#define DEBOUNCE_MS 20  // Change this value to adjust debounce time
```

- Lower values: More responsive but may register multiple presses
- Higher values: More stable but slightly higher latency

## Usage

1. Wire the buttons to the GPIO pins as described above
2. Ensure WiringPi is installed: `sudo apt-get install wiringpi`
3. Compile the project: `cd /home/dmitry/Documents/chocolate-doom && make`
4. Run Chocolate Doom with GPIO support: `/home/dmitry/Documents/chocolate-doom/src/chocolate-doom`

## Troubleshooting

### GPIO Initialization Failed

If you see "GPIO: Failed to initialize WiringPi", check:
1. WiringPi is installed: `which gpio`
2. Running with sudo (required for GPIO access): `sudo /home/dmitry/Documents/chocolate-doom/src/chocolate-doom`
3. GPIO pins are not in use by other applications

### Buttons Not Responding

1. Check GPIO pin connections with: `gpio readall`
2. Test button manually: Press the button and monitor GPIO pin state
3. Verify debounce time is not too high
4. Check for short circuits or loose connections

### Incorrect Button Behavior

If buttons map to wrong actions:
1. Verify GPIO pin numbers in `src/i_gpio.c`
2. Cross-check BCM to WiringPi pin mapping
3. Ensure no GPIO conflicts with other hardware

## Implementation Details

### Key Classes and Functions

- **`i_gpio.h`**: Header file with GPIO interface declarations
- **`i_gpio.c`**: Implementation of GPIO button reading and event posting
  - `I_InitGPIO()`: Initialize GPIO pins at startup
  - `I_ReadGPIO()`: Poll buttons and post key events (called each frame)
  - `I_ShutdownGPIO()`: Cleanup on exit

### Event System Integration

GPIO buttons generate standard DOOM key events through the `D_PostEvent()` function, making them compatible with all game keybindings and menus.

### Performance

- GPIO polling happens once per frame (~35 times per second at 35 FPS)
- Debounce filtering prevents false key repeats
- Minimal CPU overhead (~1% on modern Raspberry Pi)

## References

- WiringPi Documentation: http://wiringpi.com/
- Raspberry Pi GPIO Documentation: https://www.raspberrypi.com/documentation/computers/raspberry-pi.html
- Chocolate Doom: https://www.chocolate-doom.org/

## License

This GPIO support module is part of Chocolate Doom and follows the same GPL v2 license.
