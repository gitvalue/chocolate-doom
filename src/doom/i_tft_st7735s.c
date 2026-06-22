//
// ST7735S TFT display support for Raspberry Pi / WiringPi.
//

#include "i_tft_st7735s.h"

#ifdef HAVE_TFT_ST7735S_WPI

#include <math.h>
#include <stdlib.h>

#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "st7735s.h"

#define WPI_SPI_BUFFER_SIZE  4096
#define WPI_SPI_CHANNEL         0
#define PIN_COMMUNICATION_MODE  9
#define PIN_RESET               8
#define PIN_CHIP_SELECT        10

typedef struct
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} tft_color_t;

static lcd_ptr_t tft_lcd_settings = NULL;
static boolean tft_initialized = false;

void lcd_delay(unsigned long int milliseconds)
{
    delay(milliseconds);
}

void lcd_digitalWrite(unsigned short int pin, unsigned char value)
{
    digitalWrite(pin, value);
}

void lcd_spiWrite(unsigned char *buffer, size_t length)
{
    wiringPiSPIDataRW(WPI_SPI_CHANNEL, buffer, length);
}

boolean I_TFT_ST7735S_IsInitialized(void)
{
    return tft_initialized;
}

boolean I_TFT_ST7735S_Init(void)
{
    if (tft_initialized)
    {
        return true;
    }

    if (wiringPiSetup() == -1)
    {
        return false;
    }

    if (wiringPiSPISetup(WPI_SPI_CHANNEL, 15000000) == -1)
    {
        return false;
    }

    pinMode(PIN_COMMUNICATION_MODE, OUTPUT);
    pinMode(PIN_RESET, OUTPUT);
    pinMode(PIN_CHIP_SELECT, OUTPUT);

    digitalWrite(PIN_CHIP_SELECT, LOW);

    tft_lcd_settings = lcd_createSettings(128, 160, 0, 0,
                                          PIN_COMMUNICATION_MODE,
                                          PIN_RESET);

    if (tft_lcd_settings == NULL)
    {
        digitalWrite(PIN_CHIP_SELECT, HIGH);
        return false;
    }

    lcd_setSettingsActive(tft_lcd_settings);

    if (lcd_initialize() < LCD_OK
     || lcd_setSleepMode(LCD_SLEEP_OUT) < LCD_OK
     || lcd_setMemoryAccessControl(LCD_MADCTL_DEFAULT) < LCD_OK
     || lcd_setInterfacePixelFormat(LCD_PIXEL_FORMAT_666) < LCD_OK
     || lcd_setGammaPredefined(LCD_GAMMA_PREDEFINED_3) < LCD_OK
     || lcd_setDisplayInversion(LCD_INVERSION_OFF) < LCD_OK
     || lcd_setTearingEffectLine(LCD_TEARING_OFF) < LCD_OK
     || lcd_setDisplayMode(LCD_DISPLAY_ON) < LCD_OK)
    {
        I_TFT_ST7735S_Shutdown();
        return false;
    }

    tft_initialized = true;

    return true;
}

void I_TFT_ST7735S_Shutdown(void)
{
    if (tft_lcd_settings != NULL)
    {
        digitalWrite(PIN_CHIP_SELECT, HIGH);
        lcd_deleteSettings(tft_lcd_settings);
        tft_lcd_settings = NULL;
    }

    lcd_setSettingsActive(NULL);
    tft_initialized = false;
}

boolean I_TFT_ST7735S_Demo(void)
{
    unsigned short int width, height;
    unsigned char pixel_size;
    unsigned char *framebuffer;
    size_t length_framebuffer;
    size_t address;
    tft_color_t palette[256];
    unsigned char formula;
    unsigned short int x, y;
    size_t i, p;

    if (!tft_initialized || lcd_settings == NULL)
    {
        return false;
    }

    width = lcd_settings->width;
    height = lcd_settings->height;

    switch (lcd_settings->interface_pixel_format)
    {
        case LCD_PIXEL_FORMAT_565:
            pixel_size = 2;
            break;

        case LCD_PIXEL_FORMAT_666:
            pixel_size = 3;
            break;

        default:
            return false;
    }

    length_framebuffer = width * height * pixel_size;
    framebuffer = malloc(length_framebuffer);

    if (framebuffer == NULL)
    {
        return false;
    }

    if (lcd_setWindowPosition(0, 0, width - 1, height - 1) < LCD_OK
     || lcd_activateMemoryWrite() < LCD_OK)
    {
        free(framebuffer);
        return false;
    }

    for (i = 128; i < 1024; i++)
    {
        for (p = 0; p < 256; p++)
        {
            palette[p].red = ((i % 512) - 128) / 1.8;
            palette[p].green = 128 + 127 * sin((3.14159 * p / 64.0) + 1);
            palette[p].blue = 128 + 127 * sin((3.14159 * p / 128.0) + 1);
        }

        for (x = 0; x < width; x++)
        {
            for (y = 0; y < height; y++)
            {
                formula = 128 + 127 * sin(
                    3.14159
                    * (x - width / (2 + 2 * sin(3.14159
                                                * ((i % 512) - 128) / 384)))
                    * (y - height / (2 + 2 * sin(3.14159
                                                 * ((i % 512) - 128) / 384)))
                    / ((i % 512) * 10.0));

                address = (y * width + x) * pixel_size;

                switch (lcd_settings->interface_pixel_format)
                {
                    case LCD_PIXEL_FORMAT_565:
                        framebuffer[address] = (palette[formula].red & 0xf8)
                                             | ((palette[formula].green >> 5)
                                                & 0x07);
                        framebuffer[address + 1] =
                            ((palette[formula].green << 3) & 0xe0)
                          | ((palette[formula].blue >> 3) & 0x1f);
                        break;

                    case LCD_PIXEL_FORMAT_666:
                        framebuffer[address] = palette[formula].red;
                        framebuffer[address + 1] = palette[formula].green;
                        framebuffer[address + 2] = palette[formula].blue;
                        break;

                    default:
                        free(framebuffer);
                        return false;
                }
            }
        }

        if (lcd_framebuffer_send(framebuffer, length_framebuffer,
                                 WPI_SPI_BUFFER_SIZE) < LCD_OK)
        {
            free(framebuffer);
            return false;
        }
    }

    free(framebuffer);

    return true;
}

boolean I_TFT_ST7735S_RunDemo(void)
{
    if (!I_TFT_ST7735S_Init())
    {
        return false;
    }

    if (!I_TFT_ST7735S_Demo())
    {
        I_TFT_ST7735S_Shutdown();
        return false;
    }

    I_TFT_ST7735S_Shutdown();

    return true;
}

#else

boolean I_TFT_ST7735S_Init(void)
{
    return false;
}

boolean I_TFT_ST7735S_IsInitialized(void)
{
    return false;
}

boolean I_TFT_ST7735S_Demo(void)
{
    return false;
}

boolean I_TFT_ST7735S_RunDemo(void)
{
    return false;
}

void I_TFT_ST7735S_Shutdown(void)
{
}

#endif
