//
// ST7735S TFT display output for the SDL video backend.
//

#ifndef __I_TFT_ST7735S__
#define __I_TFT_ST7735S__

#include "SDL.h"

#include "doomtype.h"

void I_TFT_ST7735S_Init(void);
void I_TFT_ST7735S_Shutdown(void);
void I_TFT_ST7735S_Update(const pixel_t *screen, const SDL_Color *palette);

#endif
