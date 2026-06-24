//
// ST7735S TFT display output for the SDL video backend.
//

#include "i_tft_st7735s.h"

#include <stdlib.h>
#include <string.h>

#include "i_video.h"

#ifdef HAVE_TFT_ST7735S_WPI

#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "st7735s.h"

#define WPI_SPI_BUFFER_SIZE  4096
#define WPI_SPI_CHANNEL         0
#define PIN_COMMUNICATION_MODE  9
#define PIN_RESET               8
#define PIN_CHIP_SELECT        10

#define TFT_WIDTH             160
#define TFT_HEIGHT            128
#define TFT_FRAME_HEIGHT      100
#define TFT_FRAME_Y_OFFSET    ((TFT_HEIGHT - TFT_FRAME_HEIGHT) / 2)

static lcd_ptr_t tft_lcd_settings = NULL;
static boolean tft_initialized = false;
static unsigned char *tft_framebuffer = NULL;

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

static unsigned short RGB565(SDL_Color color)
{
    return ((color.r & 0xf8) << 8)
         | ((color.g & 0xfc) << 3)
         |  (color.b >> 3);
}

void I_TFT_ST7735S_Init(void)
{
    if (tft_initialized)
    {
        return;
    }

    if (wiringPiSetup() == -1)
    {
        return;
    }

    if (wiringPiSPISetup(WPI_SPI_CHANNEL, 15000000) == -1)
    {
        return;
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
        return;
    }

    lcd_setSettingsActive(tft_lcd_settings);

    if (lcd_initialize() < LCD_OK
     || lcd_setSleepMode(LCD_SLEEP_OUT) < LCD_OK
     || lcd_setMemoryAccessControl(LCD_MADCTL_MV) < LCD_OK
     || lcd_setInterfacePixelFormat(LCD_PIXEL_FORMAT_565) < LCD_OK
     || lcd_setGammaPredefined(LCD_GAMMA_PREDEFINED_3) < LCD_OK
     || lcd_setDisplayInversion(LCD_INVERSION_OFF) < LCD_OK
     || lcd_setTearingEffectLine(LCD_TEARING_OFF) < LCD_OK
     || lcd_setDisplayMode(LCD_DISPLAY_ON) < LCD_OK)
    {
        I_TFT_ST7735S_Shutdown();
        return;
    }

    tft_framebuffer = malloc(TFT_WIDTH * TFT_HEIGHT * 2);

    if (tft_framebuffer == NULL)
    {
        I_TFT_ST7735S_Shutdown();
        return;
    }

    memset(tft_framebuffer, 0, TFT_WIDTH * TFT_HEIGHT * 2);
    tft_initialized = true;
}

void I_TFT_ST7735S_Shutdown(void)
{
    free(tft_framebuffer);
    tft_framebuffer = NULL;

    if (tft_lcd_settings != NULL)
    {
        digitalWrite(PIN_CHIP_SELECT, HIGH);
        lcd_deleteSettings(tft_lcd_settings);
        tft_lcd_settings = NULL;
    }

    lcd_setSettingsActive(NULL);
    tft_initialized = false;
}

void I_TFT_ST7735S_Update(const pixel_t *screen, const SDL_Color *palette)
{
    int x, y;

    if (!tft_initialized || screen == NULL || palette == NULL)
    {
        return;
    }

    memset(tft_framebuffer, 0, TFT_WIDTH * TFT_HEIGHT * 2);

    for (y = 0; y < TFT_FRAME_HEIGHT; ++y)
    {
        const pixel_t *src_row;
        unsigned char *dst_row;

        src_row = screen + (y * 2 * SCREENWIDTH);
        dst_row = tft_framebuffer
                + ((y + TFT_FRAME_Y_OFFSET) * TFT_WIDTH * 2);

        for (x = 0; x < TFT_WIDTH; ++x)
        {
            int src_x;
            unsigned short rgb565;

            src_x = SCREENWIDTH - 1 - (x * 2);
            rgb565 = RGB565(palette[src_row[src_x]]);
            dst_row[x * 2] = rgb565 >> 8;
            dst_row[x * 2 + 1] = rgb565 & 0xff;
        }
    }

    if (lcd_setWindowPosition(0, 0, TFT_WIDTH - 1, TFT_HEIGHT - 1) < LCD_OK
     || lcd_activateMemoryWrite() < LCD_OK)
    {
        return;
    }

    lcd_framebuffer_send(tft_framebuffer, TFT_WIDTH * TFT_HEIGHT * 2,
                         WPI_SPI_BUFFER_SIZE);
}

#else

void I_TFT_ST7735S_Init(void)
{
}

void I_TFT_ST7735S_Shutdown(void)
{
}

void I_TFT_ST7735S_Update(const pixel_t *screen, const SDL_Color *palette)
{
    (void) screen;
    (void) palette;
}

#endif
