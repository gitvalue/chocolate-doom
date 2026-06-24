#!/bin/bash
#
# GPIO Button Test Script
# Tests GPIO button configuration for Chocolate Doom
#

echo "=== Chocolate Doom GPIO Button Test ==="
echo ""

# Check if WiringPi is installed
if ! command -v gpio &> /dev/null; then
    echo "ERROR: WiringPi is not installed!"
    echo "Install it with: sudo apt-get install wiringpi"
    exit 1
fi

echo "✓ WiringPi is installed"
echo ""

# Check if running as root (required for GPIO access)
if [ "$EUID" -ne 0 ]; then 
    echo "WARNING: Not running as root. GPIO access may fail."
    echo "Run with: sudo $0"
    echo ""
fi

# GPIO Pin mapping (WiringPi numbering)
declare -A gpio_pins=(
    [0]="GPIO17 (Forward)"
    [2]="GPIO27 (Backward)"
    [3]="GPIO22 (Turn Left)"
    [1]="GPIO18 (Turn Right)"
    [16]="GPIO15 (Fire)"
    [15]="GPIO14 (Use Item)"
)

echo "GPIO Pin Status (WiringPi numbering):"
echo "Pin | GPIO  | Action       | State"
echo "----|-------|--------------|-------"

for pin in 0 2 3 1 16 15; do
    state=$(gpio read $pin 2>/dev/null)
    if [ $? -eq 0 ]; then
        state_str="HIGH (not pressed)"
        if [ "$state" -eq 0 ]; then
            state_str="LOW  (PRESSED)"
        fi
        printf "%3d | %-5s | %-12s | %s\n" $pin "${gpio_pins[$pin]%(*}" "${gpio_pins[$pin]##*(}" "$state_str"
    else
        printf "%3d | %-5s | %-12s | ERROR\n" $pin "${gpio_pins[$pin]%(*}" "${gpio_pins[$pin]##*(}"
    fi
done

echo ""
echo "Pin States:"
echo "  HIGH (1) = Button NOT pressed"
echo "  LOW  (0) = Button PRESSED"
echo ""

echo "Real-time GPIO Monitoring"
echo "Press Ctrl+C to stop"
echo "Press your buttons to see state changes:"
echo ""

# Monitor GPIO pins in real-time
while true; do
    state0=$(gpio read 0 2>/dev/null)
    state1=$(gpio read 1 2>/dev/null)
    state2=$(gpio read 2 2>/dev/null)
    state3=$(gpio read 3 2>/dev/null)
    state16=$(gpio read 16 2>/dev/null)
    state15=$(gpio read 15 2>/dev/null)
    
    echo -ne "\rGPIO Pins [17:$state0 18:$state1 27:$state2 22:$state3 15:$state16 14:$state15]  "
    sleep 0.1
done
