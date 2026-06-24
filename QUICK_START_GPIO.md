# Quick Start: GPIO Button Setup for Chocolate Doom

## 5-Minute Setup

### 1. Hardware Wiring
Connect buttons to these GPIO pins (active LOW - button connects pin to GND):
- GPIO17 → Forward (UP)
- GPIO27 → Backward (DOWN)
- GPIO22 → Turn Left (LEFT)
- GPIO18 → Turn Right (RIGHT)
- GPIO15 → Fire (CTRL)
- GPIO14 → Use Item (SPACE)

Optional: Add 10kΩ pull-up resistors between each pin and 3.3V for stability.

### 2. Software Installation
```bash
# Install WiringPi
sudo apt-get install wiringpi

# Navigate to project
cd /home/dmitry/Documents/chocolate-doom

# Rebuild (already compiled, but shown for reference)
make -j4

# Test GPIO buttons
sudo bash test_gpio_buttons.sh

# Run Chocolate Doom with GPIO support
sudo src/chocolate-doom
```

### 3. In-Game Controls
- **UP Arrow** - Move forward
- **DOWN Arrow** - Move backward
- **LEFT/RIGHT Arrows** - Turn left/right
- **CTRL** - Fire weapon
- **SPACE** - Use object/activate

## Customization

To change button mappings, edit `src/i_gpio_config.h`:

```c
#define GPIO_PIN_FORWARD    0    // Change to your WiringPi pin number
#define GPIO_PIN_BACKWARD   2
#define GPIO_PIN_LEFT       3
#define GPIO_PIN_RIGHT      1
#define GPIO_PIN_FIRE       16
#define GPIO_PIN_USE_ITEM   15
```

Alternative preset configurations are included (commented out).

## BCM to WiringPi Pin Reference

| BCM GPIO | WiringPi |
|----------|----------|
| 17 | 0 |
| 18 | 1 |
| 27 | 2 |
| 22 | 3 |
| 14 | 15 |
| 15 | 16 |

Full reference: `gpio readall`

## Troubleshooting

**"GPIO: Failed to initialize WiringPi"**
- Run with sudo: `sudo src/chocolate-doom`
- Check WiringPi: `gpio readall`

**Buttons not responding**
- Check connections: `sudo bash test_gpio_buttons.sh`
- Verify debounce time in `src/i_gpio_config.h`
- Test pin directly: `gpio read <pin_number>`

**Wrong button actions**
- Verify pin mappings in `src/i_gpio_config.h`
- Recompile after changes: `cd /home/dmitry/Documents/chocolate-doom && make`

## Files Reference

- `src/i_gpio.h` - GPIO interface header
- `src/i_gpio.c` - GPIO implementation (compiled with HAVE_TFT_ST7735S_WPI flag)
- `src/i_gpio_config.h` - User-configurable pin mappings
- `GPIO_BUTTONS.md` - Full documentation
- `test_gpio_buttons.sh` - GPIO testing utility

## Performance

- Polling happens every frame (~28ms at default FPS)
- 20ms debounce filter prevents button bounce
- Minimal CPU impact (~1%)

Enjoy playing Doom with hardware buttons! 🎮
